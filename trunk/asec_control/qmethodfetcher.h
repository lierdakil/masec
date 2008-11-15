/*
 * qmethodfetcher.h
 *
 *  Created on: 15.11.2008
 *      Author: Администратор
 */

#ifndef QMETHODFETCHER_H_
#define QMETHODFETCHER_H_

#include <QtXml>
#include <QObject>
#include <QtDBus>

class QMethodFetcher : public QObject
{
	Q_OBJECT

protected:
	virtual void new_method(QString name, int argcount) = 0;
	virtual void new_arg(QString name, QString type, QString direction) =0;

public:
	QString service;
	QString interface;

	QMethodFetcher(QString service, QString interface);

	void parse();

	~QMethodFetcher();

signals:
	void error(QString message);
};

#endif /* QMETHODFETCHER_H_ */
