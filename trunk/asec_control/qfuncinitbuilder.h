/*
 * qfuncinitbuilder.h
 *
 *  Created on: 15.11.2008
 *      Author: Администратор
 */

#ifndef QFUNCINITBUILDER_H_
#define QFUNCINITBUILDER_H_

#include "qmethodfetcher.h"
#include "QObject"
#include "QString"

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

	QFuncInitBuilder(QString service);
	~QFuncInitBuilder();

protected:
	void new_method(QString name, int argcount);
	void new_arg(QString name, QString type, QString direction);
};

#endif /* QFUNCINITBUILDER_H_ */
