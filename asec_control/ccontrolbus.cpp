/*
 * ccontrolbus.cpp
 *
 *  Created on: 23.10.2008
 *      Author: livid
 */

#include "ccontrolbus.h"

CControlBus::CControlBus(QString log_file_name, QString description, QString code, bool *success)
{
	if(!QDBusConnection::sessionBus().isConnected())
	{
		*success=false;
		emit bus_error(trUtf8("CControlBus(): No DBus connection!"));
		return;
	}

	if (!(QDBusConnection::sessionBus().interface()->isValid()))
	{
		*success=false;
		emit bus_error(trUtf8("CControlBus(): No DBus session interface!"));
		return;
	}

	QMutexLocker locker(&file_mutex);
	// Открыть файл, инициализировать все, что нужно и тд и тп
	data_file=new QFile(log_file_name);
	if (!data_file->open(QIODevice::WriteOnly))
	{
		*success=false;
		emit bus_error(trUtf8("CControlBus(): Could not open file!"));
		return;
	}
	description="/* This file is a data file for ASEC\n"
		" * Experiment description (UTF-8):\n"
		" * "+description+"\n"
		" * Data format: Name:Value;Name:Value;...\n"
		" * Newline separates measurement cycles\n"
		" * \n"
		" * Code:\n";
	// Добавлять в описание код эксперимента
	description+=" * "+code.replace("\n","\n * ")+"\n";
	// и список загруженных модулей

	description+=" * Loaded modules:\n";
	QStringList list = QDBusConnection::sessionBus().interface()->registeredServiceNames().value().filter(QRegExp("^ru.pp.livid.asec.(.*)"));
	//for each service in list
	for(int k=0;k<list.count();++k)
	{
		QDBusInterface iface(list.value(k),"/","ru.pp.livid.asec.help");
		if (iface.isValid())
		{
			description+=" * "+list.value(k)+"\n";
			QDBusReply<QString> reply=iface.call("module_description");
			if(reply.isValid())
				description+=" * "+reply.value().replace("\n","\n * ")+"\n";
		}
		//TODO: описание модуля, экспортируемое самим модулем - в _модулях_
	}


	description+=" */\n";
	data_file->write(description.toUtf8());

	last_call="";
	stopped=true;
	*success=true;
}

CControlBus::~CControlBus()
{
	if(!stopped)
	{
		bool success;
		stop(&success);
	}
	QMutexLocker locker(&file_mutex);
	QMutexLocker locker_result(&result_row_mutex);
	// Закрыть файл, вычистить все лишнее и тд и тп.
	data_file->write(result_row.join(";").toUtf8());
	data_file->close();
	emit new_row(result_row);
	result_row.clear();
	delete data_file;
}

void CControlBus::stop(bool *success)
{
	//Так же послать всем сервисам команду stop.
	if (!(QDBusConnection::sessionBus().isConnected()))
	{
		emit bus_error(trUtf8("stop(): No DBus connection!"));
		*success=false;
		return;
	}

	if (!(QDBusConnection::sessionBus().interface()->isValid()))
	{
		emit bus_error(trUtf8("stop(): No DBus session interface!"));
		*success=false;
		return;
	}

	QStringList list = QDBusConnection::sessionBus().interface()->registeredServiceNames().value().filter(QRegExp("^ru.pp.livid.asec.(.*)"));
	//for each service in list
	for(int k=0;k<list.count();++k)
	{
		QDBusInterface iface(list.at(k),"/","ru.pp.livid.asec.flow");

		if(iface.isValid())
		{
			iface.call("stop");
		}
	}

	reply_wait.quit();

	stopped=true;
	*success=true;
}

QDBusError CControlBus::call(QString function, QString service, QList<QScriptValue> arguments)
{
	/* Сделать сброс result_row в выходной файл по условию вызова
	 * установочного модуля после измерительного, очистка, сигнал
	 *
	 * Определять флаг по названию, как то set_ или mes_
	 * Новая строка данных БУДЕТ начата, если полсе измерительной функции вызвана установочная.
	 */

	stopped=false;

	//Don;t call if data_file=0, throw error.
	if (data_file==NULL)
		return QDBusError(QDBusError::Other,"Experiment was not started or stopped before end of script!");

	file_mutex.lock();
	result_row_mutex.lock();
	if(last_call=="mes" && function.left(function.indexOf("_"))=="set")
	{
		data_file->write(result_row.join(";").toUtf8());
		data_file->write(QString("\n").toUtf8());
		emit new_row(result_row);
		result_row.clear();
	}
	result_row_mutex.unlock();
	file_mutex.unlock();

	//читаем список функций на интерфейсе exports
	QDBusInterface exports(service,"/","ru.pp.livid.asec.exports");
	QDBusInterface flow(service,"/","ru.pp.livid.asec.flow");

	if(exports.lastError().isValid())
		return exports.lastError();

	if(flow.lastError().isValid())
		return flow.lastError();

	QVariantList argumentlist;

	for(int m=0;m<arguments.count();++m)
		argumentlist<<arguments[m].toVariant();

	reply.clear();

	connect(&flow,SIGNAL(finished(QStringList)),SLOT(reply_call(QStringList)));

	if(flow.lastError().isValid())
		return flow.lastError();

	exports.callWithArgumentList(QDBus::NoBlock,function,argumentlist);

	if(exports.lastError().isValid())
		return exports.lastError();

	reply_wait.exec();//Run local event loop

	//Проверка на предмет ошибки, возвращенной вместо списка результатов.

	if (reply.count()==0)
		return QDBusError(QDBusError::NoReply,QString("Empty reply!"));

	if (reply.at(0)=="::ERROR::")
		return QDBusError(QDBusError::Other,reply.value(1));

	//Сохренение материала из возвращенного значения в result_row
	result_row_mutex.lock();
	result_row<<reply;
	result_row_mutex.unlock();

	last_call=function.left(function.indexOf('_'));

	return QDBusError(QDBusError::NoError,""); //no error, actually
}

QString CControlBus::init_functions(bool *success)
{
	//прочитать список экспортируемых функций и сгенерировать QScript код их вызова

	//Получить список сервисов и отсортировать по ^ru.pp.livid.asec.(.*)
	if (QDBusConnection::sessionBus().isConnected())
	{
		if (QDBusConnection::sessionBus().interface()->isValid())
		{
			QStringList list = QDBusConnection::sessionBus().interface()->registeredServiceNames().value().filter(QRegExp("^ru.pp.livid.asec.(.+)"));
			QString result;
			//for each service in list
			for(int k=0;k<list.count();++k)
			{
				QString object(list.at(k));
				object.remove("ru.pp.livid.asec.");
				result.append(QString("var %1 = {};").arg(object));

				QFuncInitBuilder i(list[k]);
				result+=i.init;
			}
			*success=true;
			return result;
		}
		else
		{
			*success=false;
			emit bus_error(trUtf8("init_functions(): No DBus session interface!"));
			return QString();
		}
	}
	else
	{
		*success=false;
		emit bus_error(trUtf8("init_functions(): No DBus connection!"));
		return QString();
	}
}

QString CControlBus::get_help(QString item,bool *success)
{
	//получить справку об экспортируемой функции
	if (item=="Introduction")
	{
		*success=true;
		return trUtf8(
				"<p><b>Краткая информация о скриптовом языке</b></p>"
				"<p>Код следует синтаксису ECMAScript. Microsoft JScript и различные реализации JavaScript так же следуют этому стандарту.</p>"
				"<p>В целом синтаксис схож с синтаксисом C++. Объявление переменных производится оператором <span style=\" font-weight:600;\">var</span>:</p>"
				"<p><code>var i;</code></p>"
				"<p>Циклы строятся по полной аналогии с C++:</p>"
				"<p><code>"
				"for(var i=0; i&lt;max; i++)<br/>"
				"{<br/>"
				"&nbsp;&nbsp;//сделать что-то<br/>"
				"}"
				"</code></p>"
				"<p>На данный момент ни одна функция не возвращает значений.</p>"
		);
	}
	else
	{
		QString name=item.left(item.indexOf("("));
		QString service=name.left(name.lastIndexOf("."));
		name.remove(service+".");

		QDBusInterface iface("ru.pp.livid.asec."+service,"/","ru.pp.livid.asec.help");
		if(iface.isValid())
		{
			QDBusReply<QString> desc_long = iface.call(name);
			if (desc_long.isValid()) {
				*success=true;
				return desc_long.value();
			} else {
				*success=true; // Normal, really.
				return trUtf8("No help for this item found");
			}
		} else {
			*success=false;
			return (trUtf8("get_help(): Could not connect to %1%2 on %3").arg(iface.service(),iface.path(),iface.interface()));
		}
	}
	//По идее, сюда невозможно попасть, все выходы происходят раньше
	*success=false;
	return (trUtf8("get_help(): Reached end of the function!"));
}

QStringList CControlBus::build_help_index(bool *success)
{
	//прочитать список экспортируемых функций

	//Получить список сервисов и отсортировать по ^ru.pp.livid.asec.(.*)
	if (!(QDBusConnection::sessionBus().interface()->isValid()))
	{
		*success=false;
		return QStringList(trUtf8("build_help_index(): No DBus connection!"));
	}

	QStringList list = QDBusConnection::sessionBus().interface()->registeredServiceNames().value().filter(QRegExp("^ru.pp.livid.asec.(.+)"));
	QStringList result("Introduction");

	//for each service in list
	for(int k=0;k<list.count();++k)
	{
		QHelpIndexBuilder i(list[k]);
		result<<i.index;
	}

	*success=true;
	return result;
}
