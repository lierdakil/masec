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
#include <QScriptValue>
#include <QFile>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QtScript>
#include "qhelpindexbuilder.h"
#include "qfuncinitbuilder.h"

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
	void bus_error(QString error);

public:
	//common name: ru.pp.livid.asec.* ; interface: ru.pp.livid.asec.exports

	CControlBus(QString log_file_name, QString description, QString code,  bool *success);
	~CControlBus();

	//QScript adaptor
	QString init_functions(bool *success);

	//Help functions
	static QString get_help(QString item, bool *success);
	static QStringList build_help_index(bool *success);

	//Flow functions
	QDBusError call(QString function, QString service, QList<QScriptValue> arguments);
	void stop(bool *success);//To be called from another thread

	QMutex reply_wait;
	QStringList reply;
};

class ReplyAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", QString("ru.pp.livid.asec.reply"))

private:
	CControlBus *bus;

public:
	ReplyAdaptor(CControlBus *b):QDBusAbstractAdaptor(b), bus(b)
	{
	}

public slots:

	void reply_call(QStringList values)
	{
		bus->reply=values;
		bus->reply_wait.unlock();
	}
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
