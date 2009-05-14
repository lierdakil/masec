#include "asec_temperature.h"
#include "adaptors.h"

vib_temperature::vib_temperature(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    QErrorMessage::qtHandler();


    new export_adaptor(this);
    new help_adaptor(this);
    new flow_adaptor(this);
    QDBusConnection connection = QDBusConnection::sessionBus();
    connection.registerService("ru.pp.livid.asec.temp");
    connection.registerObject("/", this);

    ui.gvTemp->setScene(new QGraphicsScene());
    ui.gbTest->setVisible(false);

    //	Current paradigm supposes fix_timer is run at program start and ended at termination
    //	connect(&temptl,SIGNAL(temp_set()),&fix_timer,SLOT(stop()));
    //	connect(&temptl,SIGNAL(timedout()),&fix_timer,SLOT(stop()));
    //	connect(&temptl,SIGNAL(stopped()),&fix_timer,SLOT(stop()));

    connect(&temptl,SIGNAL(temp_set()),this,SLOT(temp_set()));
    connect(&temptl,SIGNAL(stopped()),this,SLOT(stopped()));
    connect(&temptl,SIGNAL(newpoint(float,float,float)),this,SLOT(newpoint(float,float,float)));
    connect(&temptl,SIGNAL(error(QString)),this,SLOT(error(QString)));
    connect(&temptl,SIGNAL(newstatus(int)),this,SLOT(newstatus(int)));

    connect(&fix_timer, SIGNAL(timeout()), this, SLOT(fix_range()));

    QSettings f("settings.ini",QSettings::IniFormat);
    ui.sbt_test->setValue(f.value("TempCtrl/mantmout", 5).toDouble());

    try{
        tempctrl temp(qApp->property("tempid").toString());
        ui.sbRamp->setValue(temp.getramp());
        float P,I,D;
        temp.readPID(&P,&I,&D);
        ui.sbP_test->setValue(P);
        ui.sbI_test->setValue(I);
        ui.sbD_test->setValue(D);

        ui.sbMout_test->setValue(temp.getmout());
        ui.cbRange_test->setCurrentIndex(temp.getrange());

        ui.nlT->setProperty("value",temp.temp());
        ui.sbNewT_test->setValue(temp.getsetp());
    } catch (GPIBGenericException e) {
        qDebug()<<e.report();
    }
}

vib_temperature::~vib_temperature()
{
    QSettings f("settings.ini",QSettings::IniFormat);
    f.setValue("TempCtrl/mantmout", ui.sbt_test->value());
}

void vib_temperature::temp_set()
{
    QStringList data;
    data<<QString("Setpoint,K:%1").arg(temptl.setp);
    data<<QString("Temperature min,K:%1").arg(temptl.Tmin);
    data<<QString("Temperature max,K:%1").arg(temptl.Tmax);
    data<<QString("Ramp,K/min:%1").arg(temptl.ramp);
    data<<QString("Timeout,min:%1").arg(temptl.timeout);
    data<<QString("Stabilization time,min:%1").arg(temptl.settime);
    emit finished(data);
    ui.btStopTest->setEnabled(false);
    ui.btTest->setEnabled(true);
}

void vib_temperature::error(QString message)
{
    QStringList data;
    data<<trUtf8("::ERROR::");
    data<<message;
    emit finished(data);
    QErrorMessage::qtHandler()->showMessage(message);
    ui.btStopTest->setEnabled(false);
    ui.btTest->setEnabled(true);
}

void vib_temperature::newstatus(int status)
{
    switch(status)
    {
    case STATUS_RAMP:
        ui.lbStatus->setText(tr("Waiting for ramp to finish"));
        break;
    case STATUS_WAIT:
        ui.lbStatus->setText(tr("Waiting for temp to get into window"));
        break;
    case STATUS_WINDOW:
        ui.lbStatus->setText(tr("Temperature is in window"));
        break;
    case STATUS_FINISHED:
        ui.lbStatus->setText(tr("Temperature set"));
        break;
    case STATUS_TMOUT:
        ui.lbStatus->setText(tr("Timed out"));
        break;
    }
}

void vib_temperature::on_btSettings_clicked()
{
    wset.show();
}

void vib_temperature::newpoint(float time, float temp, float setpoint)
{
    static float lasttime;
    static float lasttemp;
    static float lastsetp;
    if(time>lasttime && lasttime!=0)
    {
        ui.gvTemp->scene()->addLine(lasttime,-lasttemp,time,-temp,QPen(Qt::red));
        ui.gvTemp->scene()->addLine(lasttime,-lastsetp,time,-setpoint,QPen(Qt::blue));
        ui.gvTemp->fitInView(ui.gvTemp->scene()->sceneRect());
    }
    ui.nlT->setProperty("value",temp);
    ui.nlTime->setProperty("value",time);
    lasttime=time;
    lasttemp=temp;
    lastsetp=setpoint;
}

void vib_temperature::stopped()
{
    //	QStringList data;
    //	data<<trUtf8("::ERROR::");
    //	data<<trUtf8("Stopped by user");
    //	emit finished(data);
    ui.btStopTest->setEnabled(false);
    ui.btTest->setEnabled(true);
}

void vib_temperature::fix_range()
{
    tempctrl *temp=(tempctrl*)(qApp->property("temp").toInt());
    switch(temp->getctrlmode())
    {
    case MOD_MANUAL:
        temp->setrange(ui.cbRange_test->currentIndex());
        break;
    case MOD_ZONE:
        temp->setrange(wset.getRange(temp->temp()));
        break;
    }
}

void vib_temperature::set_temp_zone(double temp,double ramp, double timeout)
{
    if (!qApp->property("tempid").toString().isEmpty())
    {
        if(temp<=wset.getMaxT())
        {
            ui.btStopTest->setEnabled(false);
            ui.btTest->setEnabled(false);
            delete ui.gvTemp->scene();
            ui.gvTemp->setScene(new QGraphicsScene());
            temptl.start_zone(temp,ramp,timeout,wset.getSettime(temp));
        } else {
            QStringList data;
            data<<trUtf8("::ERROR::");
            data<<trUtf8("Заданная температура выходит за рамки таблицы зон");
            emit finished(data);
        }
    } else {
        QStringList data;
        data<<trUtf8("::ERROR::");
        data<<trUtf8("Не установлен GPIB ID термоконтроллера.");
        emit finished(data);
    }
}

void vib_temperature::on_btTest_clicked()
{
    ui.btStopTest->setEnabled(true);
    ui.btTest->setEnabled(false);

    if (!qApp->property("tempid").toString().isEmpty())
    {
        delete ui.gvTemp->scene();
        ui.gvTemp->setScene(new QGraphicsScene());
        temptl.start_manual(
                ui.sbNewT_test->value(),
                ui.sbRamp->value(),
                ui.sbt_test->value(),
                wset.getSettime(ui.sbNewT_test->value()),
                ui.sbP_test->value(),
                ui.sbI_test->value(),
                ui.sbD_test->value(),
                ui.cbRange_test->currentIndex(),
                ui.sbMout_test->value());
    } else {
        QErrorMessage::qtHandler()->showMessage(trUtf8("Не установлен GPIB ID термоконтроллера."));
        ui.btStopTest->setEnabled(false);
        ui.btTest->setEnabled(true);
    }
}

void vib_temperature::on_btStopTest_clicked()
{
    temptl.stop();
}
