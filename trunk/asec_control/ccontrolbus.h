/*
 * vib_control_adaptor.h
 *
 *  Created on: 21.10.2008
 *      Author: livid
 */

// 23.10.2008: All in all, this module is finished! Hooray!

#ifndef VIB_CONTROL_ADAPTOR_H_
#define VIB_CONTROL_ADAPTOR_H_

#include <QtCore>
#include <QtDBus>
#include <QtXml>
#include <QScriptValue>
#include <QFile>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QtScript>

class QMethodFetcher : public QObject
{
	Q_OBJECT

protected:
	virtual void new_method(QString name, int argcount) = 0;
	virtual void new_arg(QString name, QString type, QString direction) =0;

public:
	QString service;
	QString interface;

	QMethodFetcher(QString service, QString interface): service(service), interface(interface)
	{};

	void parse()
	{
		QDBusInterface iface(service,"/","org.freedesktop.DBus.Introspectable");
		if (!iface.isValid())
		{
			QDBusError err(iface.lastError());
			emit error(trUtf8("Cannot introspect object %1 at %2:\n  %3 (%4)\n").arg("/").arg(
					service).arg(err.name()).arg(err.message()));
		}

		QDBusReply<QString> xml = iface.call("Introspect");

		if (!xml.isValid()) {
			QDBusError err(xml.error());
			if (err.isValid()) {
				emit error(trUtf8("Call to object %1 at %2:\n  %3 (%4) failed\n").arg(
						"/").arg(service).arg(err.name()).arg(err.message()));
			} else {
				emit error(trUtf8("Invalid XML received from object %1 at %2\n").arg(
						"/").arg(service));
			}
		}

		QDomDocument doc;
		doc.setContent(xml);

		QDomElement node = doc.documentElement();//path = /, <node>
		QDomNodeList ifaces = node.elementsByTagName("interface");
		for(unsigned int i=0;i<ifaces.length();++i)
			if(ifaces.item(i).isElement())
				if(ifaces.item(i).toElement().attribute("name")==interface)
				{
					QDomNodeList methods=ifaces.item(i).toElement().elementsByTagName("method");
					for(unsigned int j=0;j<methods.length();++j)
						if(methods.item(j).isElement())
						{
							QDomElement method = methods.item(j).toElement();
							QDomNodeList args = method.elementsByTagName("arg");
							new_method(method.attribute("name"),args.count());
							for(unsigned int k=0;k<args.length();++k)
								if(args.item(k).isElement())
								{
									QDomElement arg=args.item(k).toElement();
									new_arg(
											arg.attribute("name"),
											arg.attribute("type"),
											arg.attribute("direction")
											);
								}
						}
				}
	};

	~QMethodFetcher(){};

signals:
	void error(QString message);
};

class QHelpIndexBuilder : public QMethodFetcher
{
	Q_OBJECT

public:
	QStringList index;
	QString method;
	QString method_type;
	QString object;
	int method_args;

	QHelpIndexBuilder(QString service) : QMethodFetcher(service, "ru.pp.livid.asec.exports")
	{
		object=service;
		object.replace("ru.pp.livid.asec.","");
		parse();
	};
	~QHelpIndexBuilder(){};

protected:
	void new_method(QString name, int argcount)
	{
		method=object+"."+name+"(";
		method_args=argcount;
	}

	void new_arg(QString name, QString type, QString direction)
	{
		method_args--;
		if(direction=="out" && name=="")
			method_type=type;
		else
		{
			method+=type+" "+name;
			if(method_args>0)
				method+=",";
			else
			{
				method+=")";
				if(method_type=="as")
					index<<method;
			}
		}
	}
};

class QFuncInitBuilder : public QMethodFetcher
{
	Q_OBJECT

public:
	QString init;
	QString method_name;
	QString method_args;
	QString method_type;
	QString object;
	int method_args_count;
	QString code;

	QFuncInitBuilder(QString service) : QMethodFetcher(service, "ru.pp.livid.asec.exports")
	{
		code="%4.%1 = function(%3) { call(\"%1\", \"%2\", %3); }; ";
		object=service;
		object.replace("ru.pp.livid.asec.","");
		parse();
	};
	~QFuncInitBuilder(){};

protected:
	void new_method(QString name, int argcount)
	{
		method_name=name;
		method_args="";
		method_args_count=argcount;
	}

	void new_arg(QString name, QString type, QString direction)
	{
		method_args_count--;
		if(direction=="out" && name=="")
			method_type=type;
		else
		{
			method_args+=name;
			if(method_args_count>0)
				method_args+=",";
			else if(method_type=="as")
				init+=code.arg(method_name,service,method_args,object);
		}
	}
};

class CControlBus : public QObject
{
	Q_OBJECT

private:
	QStringList result_row;
	QFile *data_file;
	QString last_call;
	bool stopped;
	QMutex mutex;

signals:
	void new_row(QStringList row);

public:
	//common name: ru.pp.livid.asec.* ; interface: ru.pp.livid.asec.exports

	CControlBus(QString log_file_name, QString description, QString code,  QString *error);
	~CControlBus();

	//QScript adaptor
	static QString init_functions();

	//Help functions
	static QString get_help(QString item);
	static QStringList build_help_index();

	//Flow functions
	QDBusError call(QString function, QString service, QList<QScriptValue> arguments);
	QString stop();//To be called from another thread
};

#endif /* VIB_CONTROL_ADAPTOR_H_ */

/* Пришел в голову такой вопрос: может, имеет смысл табнуть строки,
 * меньшие по длине, с начала?
 * Будут ли столбцы соблюдаться?
 * Пример: I от H, T
 * 	H	T	I
 * 		T	I
 * 		T	I
 * ...
 * В общем случае, наверное, не будут... но самый разумный код говорит что будут...
 */
