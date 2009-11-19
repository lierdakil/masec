/*
 * scripthread.cpp
 *
 *  Created on: 24.10.2008
 *      Author: livid
 */
#include "scriptthread.h"
#include <time.h>

QScriptValue ScriptThread::call(QScriptContext *context, QScriptEngine *engine)
{
    QString name=context->argument(0).toString();
    QString service=context->argument(1).toString();

    QList<QScriptValue> list;
    for(int i=2;i<context->argumentCount();i++)
    {
        list<<context->argument(i);
    }

    CControlBus* bus=(CControlBus*)(engine->parent());

    bus->is_paused=false;

    int call_result;

    //while call has not returned without error
    clock_t call_time=0;
    do
    {
        clock_t time1=clock();
        call_result=bus->call(name,service,list);
        call_time=clock()-time1;

        if(call_result==R_CALL_ERROR)
        {
            //pause execution
            bus->is_paused=true;
            do //wait until execution is resumed
            {
                //if user called stopped(), then stop execution
                if(bus->is_stopped())
                    return context->throwError("Stopped by User");
                //don't eat up all the resources
                sleep(1);//second
            } while(bus->is_paused);
        }
        else if(call_result==R_CALL_ERROR_UNRECOVERABLE)
            return context->throwError("There was an unrecoverable error!");
        else if(call_result==R_CALL_STOPPED)
            return context->throwError("Stopped by User");
    } while (call_result!=R_CALL_SUCCESS);
    int index=bus->function_names.indexOf(service+'.'+name);
    if(index<0)
        return context->throwError("Unknown function called!");

    //if(bus->function_mean_work_time[index]<0)
    bus->function_mean_work_time[index]=call_time;
    //else
    //    bus->function_mean_work_time[index]=
    //            (bus->function_mean_work_time[index]+call_time)/2;
    bus->function_num_calls[index]+=1;

    return QScriptValue();
}

QScriptValue ScriptThread::call_count(QScriptContext *context, QScriptEngine *engine)
{
    //service.func_name
    QString name=context->argument(1).toString()+'.'+context->argument(0).toString();

    QList<QScriptValue> list;
    for(int i=2;i<context->argumentCount();i++)
    {
        list<<context->argument(i);
    }

    CControlBus* bus=(CControlBus*)(engine->parent());

    //increment counter
    int index=bus->function_names.indexOf(name);
    if(index<0)
    {
        bus->function_names.append(name);
        bus->function_max_calls.append(1);
        bus->function_num_calls.append(0);
        bus->function_mean_work_time.append(-1);
    } else {
        bus->function_max_calls[index]+=1;
    }

    return QScriptValue();
}

void ScriptThread::run_wrapper(FunctionSignature call_function)
{
    QScriptEngine *engine=new QScriptEngine(bus);

    engine->globalObject().setProperty("call",engine->newFunction(call_function,2));
    engine->setProcessEventsInterval(100);

    bool success=true;
    QString init_functions=bus->init_functions(&success);
    if (success)
    {
        if (engine->canEvaluate(init_functions))
        {
            QScriptValue res = engine->evaluate(init_functions);;
            //exception handling
            if(engine->hasUncaughtException())
                emit bug(res.toString(),engine->uncaughtExceptionLineNumber());
            else if (engine->canEvaluate(code))
            {
                QScriptValue res = engine->evaluate(code);
                //exception handling
                if(engine->hasUncaughtException())
                    emit bug(res.toString(),engine->uncaughtExceptionLineNumber());
            }
            else
                emit bug("There was a syntax error: code incomplete");
        }
        else
            emit bug("There was a syntax error: init code incomplete");
    }
    delete engine;
}

void ScriptThread::run()
{
    QString error_msg;
    bool success=true;
    bus = new CControlBus(filename,code,&success);
    connect(bus,SIGNAL(bus_error(QString)), this, SIGNAL(error(QString)));
    connect(bus,SIGNAL(call_error(QString)), this, SIGNAL(error(QString)));
    connect(bus,SIGNAL(call_error(QString)), this, SIGNAL(paused()));
    connect(bus,SIGNAL(update_time(int,int,int,int)), this, SIGNAL(update_time(int,int,int,int)));
    if (success)
    {
        run_wrapper(call_count);
        run_wrapper(call);
    }
    /* For a record, methods are (should be?) automagically
         * disconnected when sender or receiver is deleted, so
         * there's no need to explicitly disconnect it here.
         */
    disconnect(bus,SIGNAL(bus_error(QString)), this, SIGNAL(error(QString)));
    disconnect(bus,SIGNAL(call_error(QString)), this, SIGNAL(error(QString)));
    disconnect(bus,SIGNAL(call_error(QString)), this, SIGNAL(paused()));
    disconnect(bus,SIGNAL(update_time(int,int,int,int)), this, SIGNAL(update_time(int,int,int,int)));

    CControlBus *bus_tmp=bus;
    bus=0;
    delete bus_tmp;
}

/* ATTENTION: ScriptThread::stop() is called from GUI thread,
 * NOT from ScriptThread. Should be VERY careful with this.
 */
void ScriptThread::stop()
{
    bool success=true;
    if(bus!=0)
        bus->stop(&success);
}

/* ATTENTION: ScriptThread::resume() is called from GUI thread,
 * NOT from ScriptThread. Should be VERY careful with this.
 */
void ScriptThread::resume()
{
    if(bus!=0)
        bus->is_paused=false;
}
