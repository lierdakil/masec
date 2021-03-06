#include "asec_control.h"

vib_control::vib_control(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    //QErrorMessage::qtHandler();
    QDBusConnection::sessionBus();

    QErrorMessage::qtHandler();

    connect(&scriptthread,SIGNAL(started()),this,SLOT(script_started()));
    connect(&scriptthread,SIGNAL(finished()),this,SLOT(script_finished()));
    connect(&scriptthread,SIGNAL(bug(QString,int)),this,SLOT(script_bug(QString,int)));
    connect(&scriptthread,SIGNAL(error(QString)),this,SLOT(script_error(QString)));
    connect(&scriptthread,SIGNAL(paused()),this,SLOT(script_paused()));
    connect(&scriptthread,SIGNAL(update_time(int,int,int,int)),this,SLOT(update_time(int,int,int,int)));
    connect(&progressTimer,SIGNAL(timeout()),this,SLOT(progressTime()));

    ui.progressBar->setVisible(false);
    progressTimer.setInterval(60000);
}

vib_control::~vib_control()
{

}

void vib_control::on_btRun_clicked()
{
    QFileDialog fd(this,
                   trUtf8("Сохранить результат"),
                   "",
                   trUtf8("Файл результата эксперимента (*.red);;Все файлы (*.*)"));
    fd.setAcceptMode(QFileDialog::AcceptSave);
    fd.setDirectory("data");
    if(fd.exec())
    {
        ui.lbStatus->clear();
        scriptthread.code=ui.code->textEdit()->toPlainText();
        scriptthread.filename=fd.selectedFiles().first();
//        scriptthread.description=QInputDialog::getText(this,
//                                                       trUtf8("Описание эксперимента"),
//                                                       trUtf8("Краткое описание проводимого эксперимента"));
        scriptthread.start();
    }
}

void vib_control::on_btStop_clicked()
{
    scriptthread.stop();
}


void vib_control::script_started()
{
    ui.btRun->setEnabled(false);
    ui.btStop->setEnabled(true);
    ui.code->setBugLine(-1);
    ui.code->repaint();
    ui.progressBar->setVisible(true);
    ui.progressBar->setMaximum(0);
    time_elapsed=-1;
    time_max=0;
    progressTimer.start();
}

void vib_control::script_paused()
{
    ui.btResume->setEnabled(true);
    progressTimer.stop();
}

void vib_control::on_btResume_clicked()
{
    scriptthread.resume();
    ui.btResume->setEnabled(false);
    progressTimer.start();
}

void vib_control::script_finished()
{
    ui.btStop->setEnabled(false);
    ui.btRun->setEnabled(true);
    ui.btResume->setEnabled(false);
    ui.progressBar->setVisible(false);
    progressTimer.stop();
}

void vib_control::script_error(QString error)
{
    if(!error.isEmpty())
        QMessageBox::critical(NULL,trUtf8("Ошибка!"),error);
}

void vib_control::script_bug(QString message, int BugLine)
{
    ui.lbStatus->setText(message);
    ui.lbStatus->setToolTip(message);
    ui.lbStatus->setCursorPosition(0);
    ui.code->setBugLine(BugLine);
    ui.code->repaint();
}

void vib_control::on_btSaveCode_clicked()
{
    QFileDialog fd(0,trUtf8("Сохранить скрипт"),"",trUtf8("Скрипт управления (*.fcs)"));
    fd.setAcceptMode(QFileDialog::AcceptSave);
    fd.setDirectory("scripts");
    fd.setDefaultSuffix("fcs");
    if(fd.exec())
    {
        QFile f(fd.selectedFiles().first());
        if(f.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            f.write(ui.code->textEdit()->toPlainText().toUtf8());
            f.close();
        }
    }
}

void vib_control::on_btLoadCode_clicked()
{
    QFileDialog fd(0,trUtf8("Загрузить скрипт"),"",trUtf8("Скрипт управления (*.fcs)"));
    fd.setAcceptMode(QFileDialog::AcceptOpen);
    fd.setDirectory("scripts");
    if(fd.exec())
    {
        QFile f(fd.selectedFiles().first());
        if(f.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            ui.code->textEdit()->setPlainText(QString::fromUtf8(f.readAll()));
            f.close();
        }
    }
}

void vib_control::on_tabWidget_currentChanged(int index)
{
    if (index==0)
    {
        ui.cbFunction->clear();
    }
    if (index==1)
    {
        bool success=true;
        QStringList items=CControlBus::build_help_index(&success);
        if (success)
            ui.cbFunction->addItems(items);
        else
            script_error(items[0]);
    }
}

void vib_control::on_cbFunction_currentIndexChanged(QString item)
{
    if(item.isNull())
        return;
    bool success;
    QString help = CControlBus::get_help(item, &success);
    if(success)
        ui.tbHelp->setHtml(help);
    else
        script_error(help);
}

QString vib_control::minstotime(int mins)
{
    if(mins<60)
        return trUtf8("%1 m").arg(mins);
    else
        return trUtf8("%1 h %2 m").arg((mins)/60).arg(
                (mins)%60);
}

void vib_control::update_time(int calls, int max_calls, int time, int max_time)
{
    if(max_time>0 && time>0)
    {
        ui.progressBar->setMaximum(max_calls);
        ui.progressBar->setValue(calls);

        ui.lbStatus->setText(trUtf8("Left %1 of %2").arg(
                minstotime(max_time-time)).arg(
                minstotime(max_time)));
        this->time_elapsed=time;
        this->time_max=max_time;
    }
}

void vib_control::progressTime()
{
    if(time_max>0)
    {
        time_elapsed++;
        ui.lbStatus->setText(trUtf8("Left %1 of %2").arg(
                minstotime(time_max-time_elapsed)).arg(
                minstotime(time_max)));
    } else {
        ui.lbStatus->setText(trUtf8("Estimating time..."));;
    }
}
