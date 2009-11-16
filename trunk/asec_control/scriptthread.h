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

typedef QScriptValue (*FunctionSignature)(QScriptContext *, QScriptEngine *);

class ScriptThread : public QThread
{
    Q_OBJECT

public:
    static QScriptValue call(QScriptContext *context, QScriptEngine *engine);
    static QScriptValue call_count(QScriptContext *context, QScriptEngine *engine);
    QString code;
    QString filename;
    CControlBus *bus;
    void run();
    void run_wrapper(FunctionSignature call_function);
    void stop();
    void resume();
signals:
    void bug(QString message, int BugLine=-1);
    void error(QString error);
    void paused();
    void update_time(int max,int value);
};

#endif /* SCRIPTTHREAD_H_ */
