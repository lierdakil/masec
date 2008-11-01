#include "vib_temperature.h"

vib_temperature::vib_temperature(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
	
	new vt_adaptor(this);	
    QDBusConnection connection = QDBusConnection::sessionBus();
    connection.registerObject(OBJECT, this);
    connection.registerService(SERVICE);
	
	connect(&temptl,SIGNAL(reached(float,float,float)),this,SLOT(cont(float,float,float)));
}

vib_temperature::~vib_temperature()
{
	QDBusInterface iface("ru.pp.livid.vib.con","/Main","ru.pp.livid.iface");
	iface.call("unreg_service",SERVICE);
}

void vib_temperature::cont(float temp1, float temp2, float setp)
{
	QDBusInterface iface("ru.pp.livid.vib.con","/Main","ru.pp.livid.iface");
	iface.call("save_float","Setpoint",setp,"K");
	iface.call("save_float","Temperature1",temp1,"K");
	iface.call("save_float","Temperature2",temp2,"K");
	iface.call("cont");
}

void vib_temperature::on_btSettings_clicked()
{
	wset.show();
}
