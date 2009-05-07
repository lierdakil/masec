#ifndef VIB_CONTROL_H
#define VIB_CONTROL_H

#include <QtGui>
#include <QtCore>
#include <QErrorMessage>
#include <QTimer>
#include "ui_asec_control.h"
#include "scriptthread.h"

class vib_control : public QWidget
{
    Q_OBJECT

public:
    vib_control(QWidget *parent = 0);
    ~vib_control();
    bool paused;
    QMutex mutex;
    ScriptThread scriptthread;
    Ui::vib_controlClass ui;
//    QList<graphic_window*> graphics;

public slots:
        void on_btRun_clicked();
        void on_btResume_clicked();
        void on_btStop_clicked();
        void on_btSaveCode_clicked();
        void on_btLoadCode_clicked();
        void on_tabWidget_currentChanged(int index);
        void on_cbFunction_currentIndexChanged(QString item);
        void script_started();
        void script_paused();
        void script_finished();
        void script_error(QString error);
        void script_bug(QString message, int BugLine);
};

#endif // VIB_CONTROL_H
