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
	QMutex file_mutex;
	QMutex result_row_mutex;
	QEventLoop reply_wait;
	QStringList reply;

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

public slots:
	void reply_call(QStringList values)
	{
		qDebug()<<2;
		reply=values;
		reply_wait.quit();
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
