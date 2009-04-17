/*
 * vib_control_adaptor.h
 *
 *  Created on: 21.10.2008
 *      Author: livid
 */

// 23.10.2008: All in all, this module is finished! Hooray!

#ifndef VIB_CONTROL_ADAPTOR_H_
#define VIB_CONTROL_ADAPTOR_H_

#include <QtCore>
#include <QtDBus>
#include <QScriptValue>
#include <QFile>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QMessageBox>
#include <QtScript>
#include "qhelpindexbuilder.h"
#include "qfuncinitbuilder.h"

//CControlBus::call return codes
#define R_CALL_SUCCESS 0
#define R_CALL_STOPPED 1
#define R_CALL_ERROR 2
#define R_CALL_ERROR_UNRECOVERABLE 3

class CControlBus : public QObject
{
    Q_OBJECT

private:
    QStringList result_row;
    QFile *data_file;
    QString last_call;
    QMutex file_mutex;
    QMutex result_row_mutex;
    QMutex eventloop_mutex;
    QEventLoop *reply_wait;
    QStringList reply;
    QList<QDBusInterface*> flow_interfaces;
    bool stopped;

signals:
    void new_row(QStringList row);//TODO: Obsolete
    void bus_error(QString error);
    void call_error(QString error);

public:
    //common name: ru.pp.livid.asec.* ; interface: ru.pp.livid.asec.exports

    CControlBus(QString log_file_name, QString code,  bool *success);
    ~CControlBus();

    //QScript adaptor
    QString init_functions(bool *success);

    //Help functions
    static QString get_help(QString item, bool *success);
    static QStringList build_help_index(bool *success);

    //Flow functions
    int call(QString function, QString service, QList<QScriptValue> arguments);
    void stop(bool *success);//To be called from another thread

    bool is_stopped();
    bool is_paused; //This variable is used as an invariant between static ScriptThread::call()
    //and ScriptThread::resume()

public slots:
    void reply_call(QStringList values);
    void critical_call(QString module, QString message);
};

#endif /* VIB_CONTROL_ADAPTOR_H_ */
