/*
 * scriptthread.h
 *
 *  Created on: 24.10.2008
 *      Author: livid
 */

#ifndef SCRIPTTHREAD_H_
#define SCRIPTTHREAD_H_

#include <QThread>
#include <QtScript>
#include <QString>
#include "ccontrolbus.h"
//#include "cgraphiccontrol.h"

class ScriptThread : public QThread
{
	Q_OBJECT

public:
	static QScriptValue call(QScriptContext *context, QScriptEngine *engine);
    QString code;
    QString filename;
    QString description;
    CControlBus *bus;
	void run();
	void stop();
	void resume();
signals:
	void bug(QString message, int BugLine=-1);
	void error(QString error);
	void paused();
};

#endif /* SCRIPTTHREAD_H_ */
