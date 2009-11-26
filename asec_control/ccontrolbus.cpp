/*
 * ccontrolbus.cpp
 *
 *  Created on: 23.10.2008
 *      Author: livid
 */

#include "ccontrolbus.h"

CControlBus::CControlBus(QString log_file_name, QString code, bool *success)
{
    reply_wait=NULL;
    if(!QDBusConnection::sessionBus().isConnected())
    {
        *success=false;
        emit bus_error(trUtf8("CControlBus(): No DBus connection!"));
        return;
    }

    if (!(QDBusConnection::sessionBus().interface()->isValid()))
    {
        *success=false;
        emit bus_error(trUtf8("CControlBus(): No DBus session interface!"));
        return;
    }

    QMutexLocker locker(&file_mutex);
    // Открыть файл, инициализировать все, что нужно и тд и тп
    data_file=new QFile(log_file_name);
    if (!data_file->open(QIODevice::WriteOnly))
    {
        *success=false;
        emit bus_error(trUtf8("CControlBus(): Could not open file!"));
        return;
    }
    QString description="# This file is a data file for ASEC\n"
                "# Data format: Name:Value;Name:Value;...\n"
                "# Newline separates measurement cycles\n"
                "# \n"
                "# Code:\n";
    // Добавлять в описание код эксперимента
    description+="# "+code.replace("\n","\n# ")+"\n";
    // и список загруженных модулей

    flow_interfaces.clear();
    description+="# Loaded modules:\n";
    QStringList list = QDBusConnection::sessionBus().interface()->registeredServiceNames().value().filter(QRegExp("^ru.pp.livid.asec.(.*)"));
    //for each service in list
    //for(int k=0;k<list.count();++k)
    foreach(QString service, list)
    {
        QDBusInterface iface(service,"/","ru.pp.livid.asec.help");

        if (iface.isValid())
        {
            description+="# "+service+"\n";
            QDBusReply<QString> reply=iface.call("module_description");
            if(reply.isValid())
                description+="# "+reply.value().replace("\n","\n# ")+"\n";
        }

        QDBusInterface* flow=new QDBusInterface(service,"/","ru.pp.livid.asec.flow");
        if(flow->isValid())
        {
            flow_interfaces << flow;
            connect(flow,SIGNAL(critical(QString,QString)),SLOT(critical_call(QString,QString)));
        } else
            delete flow;

        QDBusInterface* exports=new QDBusInterface(service,"/","ru.pp.livid.asec.exports");
        if(exports->isValid())
            exports_interfaces << exports;
        else
            delete exports;
    }


    description+="#\n";
    data_file->write(description.toUtf8());
    data_file->close();

    last_call="";
    stopped=true;
    *success=true;
}

CControlBus::~CControlBus()
{
    if(!stopped)
    {
        bool success;
        stop(&success);
    }

    foreach(QDBusInterface* flow, flow_interfaces)
    {
        flow->disconnect();
        delete flow;
    }

    foreach(QDBusInterface* exports, exports_interfaces)
        delete exports;

    QMutexLocker locker(&file_mutex);
    // Закрыть файл, вычистить все лишнее и тд и тп.
    if(data_file->isOpen())
        data_file->close();
    delete data_file;
}

void CControlBus::stop(bool *success)
{
    //Так же послать всем сервисам команду stop.
    if (!(QDBusConnection::sessionBus().isConnected()))
    {
        emit bus_error(trUtf8("stop(): No DBus connection!"));
        *success=false;
        return;
    }

    if (!(QDBusConnection::sessionBus().interface()->isValid()))
    {
        emit bus_error(trUtf8("stop(): No DBus session interface!"));
        *success=false;
        return;
    }

    //for each registered flow interface
    foreach(QDBusInterface* flow, flow_interfaces)
    {
        if(flow->isValid())
        {
            flow->call("stop");
        }
    }

    eventloop_mutex.lock();
    if(reply_wait!=NULL)
        if(reply_wait->isRunning())
            reply_wait->quit();
    eventloop_mutex.unlock();

    stopped=true;
    *success=true;
}

bool CControlBus::is_stopped()
{
    return stopped;
}

void CControlBus::reply_call(QStringList values)
{
    reply=values;
    eventloop_mutex.lock();
    if(reply_wait!=NULL)
        reply_wait->quit();
    eventloop_mutex.unlock();
}

void CControlBus::critical_call(QString module, QString message)
{
    QStringList reply_t;
    reply_t<<"::ERROR::";
    reply_t<<tr("There was critical error in module %1 with message '%2'").arg(module,message);
    reply_t<<"::UNRECOVERABLE::";
    reply=reply_t;
    eventloop_mutex.lock();
    if(reply_wait!=NULL)
        reply_wait->quit();
    eventloop_mutex.unlock();
}

int CControlBus::call(QString function, QString service, QList<QScriptValue> arguments)
{
    /* Сделать сброс result_row в выходной файл по условию вызова
         * установочного модуля после измерительного, очистка, сигнал
         *
         * Определять флаг по названию, как то set_ или mes_
         * Новая строка данных БУДЕТ начата, если полсе измерительной функции вызвана установочная.
         */

    int time_max=0,time_value=0,calls_max=0,calls_value=0;
    for(int i=0; i<function_mean_work_time.count(); ++i)
    {
        if(function_mean_work_time.at(i)<0)
        {
            time_max=0;
            time_value=-1;
            break;
        }
        calls_max+=function_max_calls.at(i);
        calls_value+=function_num_calls.at(i);
        time_max+=function_mean_work_time.at(i)*function_max_calls.at(i);
        time_value+=function_mean_work_time.at(i)*function_num_calls.at(i);
    }
    emit update_time(calls_value,calls_max,time_value/60000, time_max/60000);

    stopped=false;

    //Don;t call if data_file=0, throw error.
    if (data_file==NULL)
    {
        emit call_error(trUtf8("Experiment was not started or stopped before end of script!"));
        return R_CALL_ERROR;
    }

    //читаем список функций на интерфейсе exports
    QDBusInterface* exports=0;//(service,"/","ru.pp.livid.asec.exports")
    QDBusInterface* flow=0;
    //find corresponding flow interface among registered
    foreach(QDBusInterface* flow_i, flow_interfaces)
    {
        if(flow_i->isValid() && flow_i->service()==service)
        {
            flow=flow_i;
            break;
        }
    }

    foreach(QDBusInterface* exports_i, exports_interfaces)
    {
        if(exports_i->isValid() && exports_i->service()==service)
        {
            exports=exports_i;
            break;
        }
    }

    if(exports==0)//No corresponding flow interface found
    {
        emit call_error(tr("Unable to find exports interface on %1").arg(service));
        return R_CALL_ERROR;
    }

    if(flow==0)//No corresponding flow interface found
    {
        emit call_error(tr("Unable to find flow interface on %1").arg(service));
        return R_CALL_ERROR;
    }

    if(exports->lastError().isValid())
    {
        emit call_error(tr("Error on interface exports with message '%1'"
                                "on line %2").arg(exports->lastError().message()).arg(__LINE__));
        return R_CALL_ERROR;
    }

    if(flow->lastError().isValid())
    {
        emit call_error(tr("Error on interface flow with message '%1'"
                                "on line %2").arg(flow->lastError().message()).arg(__LINE__));
        return R_CALL_ERROR;
    }

    QVariantList argumentlist;

    for(int m=0;m<arguments.count();++m)
        argumentlist<<arguments[m].toVariant();

    reply.clear();

    //connect signal of finishing called module to handler of this
    if(!connect(flow,SIGNAL(finished(QStringList)),this,SLOT(reply_call(QStringList))))
    {
        emit call_error(tr("Could not connect reply_call"));
        return R_CALL_ERROR;
    }

    if(flow->lastError().isValid())
    {
        emit call_error(flow->lastError().message());
        return R_CALL_ERROR;
    }

    exports->callWithArgumentList(QDBus::Block,function,argumentlist);

    if(exports->lastError().isValid())
    {
        emit call_error(service+": "+exports->lastError().message());
        return R_CALL_ERROR;
    }

    //creates event loop which will wait for reply from
    //flow interface
    eventloop_mutex.lock();
    if(reply_wait==NULL)
        reply_wait=new QEventLoop(this);
    else {
        emit call_error(tr("Tried to create event loop, but it's already there!"));
        eventloop_mutex.unlock();
        return R_CALL_ERROR;
    }
    eventloop_mutex.unlock();
    //should we throw error if loop already initialized?

    reply_wait->exec();//Run local event loop

    //after event loop finished, we do not need it anymore
    eventloop_mutex.lock();
    QEventLoop *ptemp=reply_wait;
    reply_wait=0;
    delete ptemp;
    eventloop_mutex.unlock();
    //Cheat with temporary holder is used in order to signalize
    //other threads and to avoid race conditions

    //since local event loop finished, reply is here, so disconect
    if(!disconnect(flow,SIGNAL(finished(QStringList)),this,SLOT(reply_call(QStringList))))
    {
        emit call_error(tr("Could not disconnect reply_call"));
        return R_CALL_ERROR;
    }

    //Check if loop was stopped by user
    if(stopped)
    {
        return R_CALL_STOPPED;
    }

    //Check if we received error in results' stead
    if (reply.count()==0)
    {
        emit call_error(trUtf8("Empty reply received!"));
        return R_CALL_ERROR;
    }

    if (reply.at(0)=="::ERROR::")
    {
        emit call_error(reply.at(1));
        if(reply.count()>=3)//since it's an optional parameter
            if(reply.at(2)=="::UNRECOVERABLE::")
                return R_CALL_ERROR_UNRECOVERABLE;
        //else if not unrecoverable
        return R_CALL_ERROR;
    }

    if (reply.at(0)=="::SUCCESS::" && reply.count()==1)
        //Функции, возвращаюющие такой ответ, не относятся ни к измерительным,
        //ни к установочным в идеальном понимании, поэтому они должны
        //выходить здесь.
        //По стандарту, такие функции должны возвращать ТОЛЬКО элемент ::SUCCESS::
    {
        return R_CALL_SUCCESS;
    }

    //Сохренение материала из возвращенного значения в файл
    file_mutex.lock();
    {
        while(!data_file->open(QIODevice::Append | QIODevice::Text))
        {
            if(QMessageBox::warning(0,tr("Failed to open data file"),
                                    tr("Program was unable to open data file. Retry?"),
                                    QMessageBox::Retry||QMessageBox::Abort,
                                    QMessageBox::Retry)==QMessageBox::Abort)
            {
                emit call_error(tr("Failed to open data file. Abort by user."));
                file_mutex.unlock();
                return R_CALL_ERROR_UNRECOVERABLE;
            }
        }
        if(last_call=="mes" && function.left(function.indexOf("_"))=="set") //если началась новая "строка"
            data_file->write(QString("\n").toUtf8());//Перейдем на новую строку
        else //Иначе идет дописка к текущей строке
            data_file->write(";");//поэтому нужно дописать разделитель
        data_file->write(reply.join(";").toUtf8());//Сохраняем ответ
        data_file->close();
    }
    file_mutex.unlock();

    last_call=function.left(function.indexOf('_'));

    return R_CALL_SUCCESS;
}

QString CControlBus::init_functions(bool *success)
{
    //прочитать список экспортируемых функций и сгенерировать QScript код их вызова

    //Получить список сервисов и отсортировать по ^ru.pp.livid.asec.(.*)
    if (QDBusConnection::sessionBus().isConnected())
    {
        if (QDBusConnection::sessionBus().interface()->isValid())
        {
            QStringList list = QDBusConnection::sessionBus().interface()->registeredServiceNames().value().filter(QRegExp("^ru.pp.livid.asec.(.+)"));
            QString result;
            //for each service in list
            for(int k=0;k<list.count();++k)
            {
                QString object(list.at(k));
                object.remove("ru.pp.livid.asec.");
                result.append(QString("var %1 = {};").arg(object));

                QFuncInitBuilder i(list[k]);
                result+=i.init;
            }
            *success=true;
            return result;
        }
        else
        {
            *success=false;
            emit bus_error(trUtf8("init_functions(): No DBus session interface!"));
            return QString();
        }
    }
    else
    {
        *success=false;
        emit bus_error(trUtf8("init_functions(): No DBus connection!"));
        return QString();
    }
}

QString CControlBus::get_help(QString item,bool *success)
{
    //получить справку об экспортируемой функции
    if (item=="Introduction")
    {
        *success=true;
        return trUtf8(
                "<p><b>Краткая информация о скриптовом языке</b></p>"
                "<p>Код следует синтаксису ECMAScript. Microsoft JScript и различные реализации"
                "JavaScript так же следуют этому стандарту.</p>"
                "<p>В целом синтаксис схож с синтаксисом C++. Объявление переменных производится"
                "оператором <span style=\" font-weight:600;\">var</span>:</p>"
                "<p><code>var i;</code></p>"
                "<p>Циклы строятся по полной аналогии с C++:</p>"
                "<p><code>"
                "for(var i=0; i&lt;max; i++)<br/>"
                "{<br/>"
                "&nbsp;&nbsp;//сделать что-то<br/>"
                "}"
                "</code></p>"
                "<p>На данный момент ни одна функция не возвращает значений.</p>"
                );
    }
    else
    {
        QString name=item.left(item.indexOf("("));
        QString service=name.left(name.lastIndexOf("."));
        name.remove(service+".");

        QDBusInterface iface("ru.pp.livid.asec."+service,"/","ru.pp.livid.asec.help");
        if(iface.isValid())
        {
            QDBusReply<QString> desc_long = iface.call(name);
            if (desc_long.isValid()) {
                *success=true;
                return desc_long.value();
            } else {
                *success=true; // Normal, really.
                return trUtf8("No help for this item found");
            }
        } else {
            *success=false;
            return (trUtf8("get_help(): Could not connect to %1%2 on %3").arg(iface.service(),iface.path(),iface.interface()));
        }
    }
    //По идее, сюда невозможно попасть, все выходы происходят раньше
    *success=false;
    return (trUtf8("get_help(): Reached end of the function!"));
}

QStringList CControlBus::build_help_index(bool *success)
{
    //прочитать список экспортируемых функций

    //Получить список сервисов и отсортировать по ^ru.pp.livid.asec.(.*)
    if (!(QDBusConnection::sessionBus().interface()->isValid()))
    {
        *success=false;
        return QStringList(trUtf8("build_help_index(): No DBus connection!"));
    }

    QStringList list = QDBusConnection::sessionBus().interface()->registeredServiceNames().value().filter(QRegExp("^ru.pp.livid.asec.(.+)"));
    QStringList result("Introduction");

    //for each service in list
    for(int k=0;k<list.count();++k)
    {
        QHelpIndexBuilder i(list[k]);
        result<<i.index;
    }

    *success=true;
    return result;
}
