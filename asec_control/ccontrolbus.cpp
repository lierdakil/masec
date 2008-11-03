/*
 * ccontrolbus.cpp
 *
 *  Created on: 23.10.2008
 *      Author: livid
 */

#include "ccontrolbus.h"

CControlBus::CControlBus(QString log_file_name, QString description, QString code, QString *error)
{
	QMutexLocker locker(&mutex);
	// Открыть файл, инициализировать все, что нужно и тд и тп
	data_file=new QFile(log_file_name);
	if (!data_file->open(QIODevice::WriteOnly))
	{
		*error = trUtf8("Could not open file!");
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
	if (QDBusConnection::sessionBus().isConnected())
	{
		if (QDBusConnection::sessionBus().interface()->isValid())
		{
			if (QDBusConnection::sessionBus().interface()->isValid())
			{
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
			}
		}
		else
			*error=trUtf8("::ERROR::stop::No DBus session interface!");
			return;
	}
	else
	{
		*error=trUtf8("::ERROR::stop::No DBus connection!");
		return;
	}
	description+=" */\n";
	data_file->write(description.toUtf8());

	last_call="";
	stopped=true;
}

CControlBus::~CControlBus()
{
	if(!stopped)
		stop();//TODO: Error handler?
	QMutexLocker locker(&mutex);
	// Закрыть файл, вычистить все лишнее и тд и тп.
	data_file->write(result_row.join(";").toUtf8());
	data_file->close();
	emit new_row(result_row);
	result_row.clear();
	delete data_file;
}

QString CControlBus::stop() //returns error
{
	//Так же послать всем сервисам команду stop.
	if (QDBusConnection::sessionBus().isConnected())
	{
		if (QDBusConnection::sessionBus().interface()->isValid())
		{
			QStringList list = QDBusConnection::sessionBus().interface()->registeredServiceNames().value().filter(QRegExp("^ru.pp.livid.asec.(.*)"));
			//for each service in list
			for(int k=0;k<list.count();++k)
			{
				QDBusInterface iface(list.at(k),"/","ru.pp.livid.asec.exports");

				if(iface.isValid())
				{
					iface.call("stop");
				}
			}

			stopped=true;
			return QString();
		}
		else
			return trUtf8("::ERROR::stop::No DBus session interface!");
	}
	else
		return trUtf8("::ERROR::stop::No DBus connection!");
}

QDBusError CControlBus::call(QString function, QString service, QList<QScriptValue> arguments)
{
	/* Сделать сброс result_row в выходной файл по условию вызова
	 * установочного модуля после измерительного, очистка, сигнал
	 *
	 * Определять флаг по названию, как то set_ или mes_
	 * Новая строка данных БУДЕТ начата, если полсе измерительной функции вызвана установочная.
	 */
	QMutexLocker locker(&mutex);

	stopped=false;

	//Don;t call if data_file=0, throw error.
	if (data_file==NULL)
		return QDBusError(QDBusError::Other,"Experiment was not started or stopped before end of script!");

	if(last_call=="mes" && function.left(function.indexOf("_"))=="set")
	{
		data_file->write(result_row.join(";").toUtf8());
		data_file->write(QString("\n").toUtf8());
		emit new_row(result_row);
		result_row.clear();
	}

	//читаем список функций на интерфейсе exports
	QDBusInterface iface(service,"/","ru.pp.livid.asec.exports");

	if(iface.isValid())
	{
		QVariantList list;

		for(int m=0;m<arguments.count();++m)
			list<<arguments[m].toVariant();

		QDBusReply<QStringList> reply=iface.callWithArgumentList(QDBus::BlockWithGui,function,list);

		if(stopped)
			return QDBusError(QDBusError::Other,"Stopped by User");

		if (iface.lastError().isValid())
			return iface.lastError();

		//Проверка на предмет ошибки, возвращенной вместо списка результатов.
		if (!reply.isValid())
			return QDBusError(QDBusError::NoReply,QString("Invalid reply!"));

		if (reply.value().count()==0)
			return QDBusError(QDBusError::NoReply,QString("Empty reply!"));

		if (reply.value().at(0)=="::ERROR::")
			return QDBusError(QDBusError::Other,reply.value().value(1));

		//Сохренение материала из возвращенного значения в result_row
		result_row<<reply.value();

		last_call=function.left(function.indexOf('_'));

		return iface.lastError(); //no error, actually
	}
	else
		return iface.lastError(); //invalid interface
}

QString CControlBus::init_functions()
{
	//прочитать список экспортируемых функций и сгенерировать QScript код их вызова

	//Получить список сервисов и отсортировать по ^ru.pp.livid.asec.(.*)
	if (QDBusConnection::sessionBus().isConnected())
	{
		if (QDBusConnection::sessionBus().interface()->isValid())
		{
			QStringList list = QDBusConnection::sessionBus().interface()->registeredServiceNames().value().filter(QRegExp("^ru.pp.livid.asec.(.*)"));
			QString result;
			QString code="%4.%1 = function(%3) { call(\"%1\", \"%2\", %3); }; ";
			//for each service in list
			for(int k=0;k<list.count();++k)
			{
				QString object(list.at(k));
				object.remove("ru.pp.livid.asec.");
				result.append(QString("var %1 = {};").arg(object));

				QFuncInitBuilder i(list[k]);
				result+=i.init;
////				QDBusInterface iface(list.at(k),"/","ru.pp.livid.asec.exports");
////
////				if(iface.isValid())
////				{
////					QString service=list.at(k);
////					QString object(service);
////					object.remove("ru.pp.livid.asec.");
////					result.append(QString("var %1 = {};").arg(object));
////
////					//for each method on interface...
////					for (int i = mo->methodOffset(); i < mo->methodCount(); i++)
////					{
////						QMetaMethod mm = mo->method(i);
////						QString signature(mm.signature());
////						QString name=signature.left(signature.indexOf('('));
////
////						QString typeName=QString::fromAscii(mm.typeName());
////
////						if (typeName=="QStringList") //if not, it's not export function, is it?
////						{
////							QStringList args;
////							for(int n=0;n<mm.parameterNames().count();++n)
////								args<<mm.parameterNames().value(n);
////
////							result.append(code.arg(name,service,args.join(","),object));
////						}
////					}
//				} else {
//					return trUtf8("::ERROR::init_functions::Could not connect to %1%2 on %3!").arg(iface.service(),iface.path(),iface.interface());
//				}
			}

			qDebug()<<result;

			return result;
		}
		else
			return trUtf8("::ERROR::init_functions::No DBus session interface!");
	}
	else
		return trUtf8("::ERROR::init_functions::No DBus connection!");
}

QString CControlBus::get_help(QString item)
{
	//получить справку об экспортируемой функции
	if (item=="Introduction")
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
	else
	{
		//QString desc = "%1(%2) exported by %3";
		QString name=item.left(item.indexOf("("));
		QString service=name.left(name.lastIndexOf("."));
		name.remove(service+".");

		QDBusInterface iface("ru.pp.livid.asec."+service,"/","ru.pp.livid.asec.help");
		if(iface.isValid())
		{
			QDBusReply<QString> desc_long = iface.call(name);
			if (desc_long.isValid())
				return desc_long.value();
			else
				return trUtf8("No help for this item found");
		} else
			return trUtf8("::ERROR::get_help::Could not connect to %1%2 on %3").arg(iface.service(),iface.path(),iface.interface());
	}
	//По идее, сюда невозможно попасть, все выходы происходят раньше
	return trUtf8("::ERROR::get_help::Reached end of the function!");
}

QStringList CControlBus::build_help_index()
{
	//прочитать список экспортируемых функций

	//Получить список сервисов и отсортировать по ^ru.pp.livid.asec.(.*)
	if (QDBusConnection::sessionBus().interface()->isValid())
	{
		QStringList list = QDBusConnection::sessionBus().interface()->registeredServiceNames().value().filter(QRegExp("^ru.pp.livid.asec.(.*)"));
		QStringList result("Introduction");

		//for each service in list
		for(int k=0;k<list.count();++k)
		{
			QHelpIndexBuilder i(list[k]);
			result<<i.index;
			//QFuncInitBuilder i(list[k]);
			//result<<i.init;
		}

		return result;
	}
	else
		return QStringList(trUtf8("::ERROR::build_help_index::No DBus connection!"));
}
