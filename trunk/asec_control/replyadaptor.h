/*
 * WARNING! THIS IS DEPRECATED AND WILL BE REMOVED SOON!
 * replyadaptor.h
 *
 *  Created on: 17.11.2008
 *      Author: kazei
 */

#ifndef REPLYADAPTOR_H_
#define REPLYADAPTOR_H_

#include <QtCore>
#include <QtDBus>

class ReplyAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", QString("ru.pp.livid.asec.reply"))

public:
	ReplyAdaptor(QObject *b):QDBusAbstractAdaptor(b)
	{
		QDBusConnection::sessionBus().registerService("ru.pp.livid.asec");
		QDBusConnection::sessionBus().registerObject("/", b);
	}

public slots:
	void reply(QStringList data)
	{
		qDebug()<<1;
		emit reply_call(data);
	}
signals:
	void reply_call(QStringList values);
};

#endif /* REPLYADAPTOR_H_ */
