/*
 * qhelpindexbuilder.h
 *
 *  Created on: 15.11.2008
 *      Author: Администратор
 */

#ifndef QHELPINDEXBUILDER_H_
#define QHELPINDEXBUILDER_H_

#include "qmethodfetcher.h"

class QHelpIndexBuilder : public QMethodFetcher
{
	Q_OBJECT

public:
	QStringList index;
	QString method;
	QString method_type;
	QString object;
	int method_args;

	QHelpIndexBuilder(QString service);
	~QHelpIndexBuilder();

	void new_method(QString name, int argcount);
	void new_arg(QString name, QString type, QString direction);
};

#endif /* QHELPINDEXBUILDER_H_ */
