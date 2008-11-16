/*
 * scripthread.cpp
 *
 *  Created on: 24.10.2008
 *      Author: livid
 */
#include "scriptthread.h"

QScriptValue ScriptThread::call(QScriptContext *context, QScriptEngine *engine)
{
	QString name=context->argument(0).toString();
	QString service=context->argument(1).toString();

	QList<QScriptValue> list;
	for(int i=2;i<context->argumentCount();i++)
	{
		list<<context->argument(i);
	}

	QDBusError error=(((CControlBus*)(engine->parent()))->call(name,service,list));

	if (error.isValid())
		return context->throwError(error.message());
	else
		return QScriptValue();
}

/* TODO: Переделать к чертовой бабушке все, что касается графиков. Такое ощущение, что я курил много и долго.
 * Идея такая: можно сделать событие в Bus, сообщающее, что пара возвращенных значений появилась. После
 * этого отсылать сообщение непосредственно окну графика с координатами.
 * Создание графика тоже упрощается.
 * Минус - список графиков придется тоже хранить в Bus... Либо создавать отдельный класс, который будет
 * за этим следить (что наверное аккуратней), который будет спрашивать у Bus, есть ли то, что нужно.
 * Да, и прикрутить-таки Qwt!!!
 */
//QScriptValue ScriptThread::make_graphic(QScriptContext *context, QScriptEngine *engine)
//{
//	//make_graphic string title, string X_name, string Y_name
//
//	for (int i=0;i<((vib_control*)(engine->parent()))->graphics.count();i++)
//	{
//		if (((vib_control*)(engine->parent()))->graphics.at(i)->title()==
//			context->argument(0).toString())
//		{
//			delete ((vib_control*)(engine->parent()))->graphics.at(i);
//			((vib_control*)(engine->parent()))->graphics.removeAt(i);
//			i--;
//		}
//	}
//
//	((vib_control*)(engine->parent()))->graphics<<new graphic_window(context->argument(0).toString(),
//	context->argument(1).toString(),
//	context->argument(2).toString());
//
//	return QScriptValue();
//}

void ScriptThread::run()
{
	QString error_msg;
	bool success=true;
	bus = new CControlBus(filename,description,code,&success);
	connect(bus,SIGNAL(bus_error(QString)), this, SIGNAL(error(QStirng)));
	if (success)
	{
		QScriptEngine *engine=new QScriptEngine(bus);

		engine->globalObject().setProperty("call",engine->newFunction(call,2));
		//make_graphic
		//engine.globalObject().setProperty("make_graphic",engine.newFunction(make_graphic,3));
		engine->setProcessEventsInterval(100);

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
	disconnect(bus,SIGNAL(bus_error(QString)), this, SIGNAL(error(QStirng)));
	delete bus;
}

// ВЫЗЫВАЕТСЯ ИЗ ПОТОКА ИНТЕРФЕЙСА!!!!!!!!!!!
void ScriptThread::stop()
{
	bool success=true;
	bus->stop(&success);
}

