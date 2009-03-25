#include "asec_magn.h"
#include <QInputDialog>

asec_magn::asec_magn(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    QErrorMessage::qtHandler();

    QDBusConnection connection = QDBusConnection::sessionBus();
    connection.registerService("ru.pp.livid.asec.magn");
    connection.registerObject("/", this);
    new export_adaptor(this);
    new help_adaptor(this);
    new flow_adaptor(this);

    ui.gvField->setScene(new QGraphicsScene());

    connect(&magn_timer,SIGNAL(field_set(float,float,float)),this,SLOT(field_set(float,float,float)));
    connect(&magn_timer,SIGNAL(quench()),this,SLOT(quench()));
    connect(&magn_timer,SIGNAL(newpoint(float,float)),this,SLOT(newpoint(float,float)));

    QSettings f("settings.ini",QSettings::IniFormat);
    QString GPID = f.value("GPIB/magnid","").toString();
    if (!GPID.isEmpty())
    {
        ui.edGPID->setText(GPID);
        qApp->setProperty("magnid",GPID);
    }
}

asec_magn::~asec_magn()
{
    QSettings f("settings.ini",QSettings::IniFormat);
    f.setValue("GPIB/magnid", ui.edGPID->text());
}

void asec_magn::on_edGPID_returnPressed()
{
    QString GPID = ui.edGPID->text();
    qApp->setProperty("magnid",GPID);
}

void asec_magn::on_btSetField_clicked()
{
    float field=QInputDialog::getDouble(this,trUtf8("Магнитное поле"),trUtf8("Введите, Гс"),0);
    set_field(field);
}

void asec_magn::newpoint(float time, float field)
{
    static float lasttime;
    static float lastfield;
    if(time>lasttime && lasttime!=0)
    {
        ui.gvField->scene()->addLine(lasttime,-lastfield,time,-field,QPen(Qt::red));
        ui.gvField->fitInView(ui.gvField->scene()->sceneRect());
    }
    lasttime=time;
    lastfield=field;
}

void asec_magn::set_field(float field) //method to set field
{
    //TODO: check limits
    //Is there a reason for this? I mean, limits are set by hardware anyway.
    ui.btSetField->setEnabled(false);
    delete ui.gvField->scene();
    ui.gvField->setScene(new QGraphicsScene);
    magn_timer.start(field);
}

void asec_magn::field_set(float reqfield, float field, float settime/*minutes*/)//field reached requested value
{
    QStringList data;
    data<<QString("Requested Field, kG:%1").arg(reqfield);
    data<<QString("Current Field,K:%1").arg(field);
    data<<QString("Stabilization time,min:%1").arg(settime);
    emit finished(data);
    ui.btSetField->setEnabled(true);
}

void asec_magn::quench()
{
    QStringList data;
    data<<QString("::ERROR::");
    data<<trUtf8("Magnet quench detected!");
    data<<QString("::UNRECOVERABLE::");
    emit finished(data);
    QErrorMessage::qtHandler()->showMessage(trUtf8("Quench detected!"));
}
