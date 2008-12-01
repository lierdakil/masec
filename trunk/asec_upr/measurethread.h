/*
 * measurethread.h
 *
 *  Created on: 18.11.2008
 *      Author: kazei
 */

#ifndef MEASURETHREAD_H_
#define MEASURETHREAD_H_

#include <QtCore>
#include "measure.h"

class MeasureThread : public QThread
{
	Q_OBJECT

public:
	double startf;
	double stopf;
	QString filename;
	QString oscid;
	QString genid;
	QString mulid;
	double volts1;
	int sm1, sm2;
	void run();
signals:
	void finished(QStringList data);
};

#endif /* MEASURETHREAD_H_ */
