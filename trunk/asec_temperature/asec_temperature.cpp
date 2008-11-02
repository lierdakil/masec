#include "asec_temperature.h"

vib_temperature::vib_temperature(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    QDBusConnection connection = QDBusConnection::sessionBus();
    connection.registerService("ru.pp.livid.asec.temp");
    connection.registerObject("/", this);
    new export_adaptor(this);
    new help_adaptor(this);

    connect(&temptl,SIGNAL(reached(float,float,float)),this,SLOT(cont(float,float,float)));
}

vib_temperature::~vib_temperature()
{

}

void vib_temperature::cont(float temp1, float temp2, float setp) //TODO: Name is misleading, so call it something better
{
        QDBusInterface iface("ru.pp.livid.vib.con","/Main","ru.pp.livid.iface");
        result_var<<QString("Setpoint,K:%1").arg(setp);
        result_var<<QString("Temperature 1,K:%1").arg(temp1);
        result_var<<QString("Temperature 2,K:%1").arg(temp2);
}

void vib_temperature::on_btSettings_clicked()
{
	wset.show();
}
//TODO: connect TimeLine.finished to return error

QStringList vib_temperature::result()
{
    while(result_var.count()==0)
    {
        qApp->processEvents();//TODO:Maybe there's a better way?
    }
    return result_var;
}