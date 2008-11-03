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

	connect(&temptl,SIGNAL(finished()),this,SLOT(finished()));
}

vib_temperature::~vib_temperature()
{

}

void vib_temperature::finished()
{
	result_var<<QString("Setpoint,K:%1").arg(temptl.setp);
	result_var<<QString("Temperature 1,K:%1").arg(temptl.temp1);
	result_var<<QString("Temperature 2,K:%1").arg(temptl.temp2);
	result_var<<QString("Ramp,K/min:%1").arg(temptl.ramp);
	result_var<<QString("Timeout,min:%1").arg(temptl.timeout);
	result_var<<QString("Stabilization time,min:%1").arg(temptl.settime);
	can_return=true;
}

void vib_temperature::on_btSettings_clicked()
{
	wset.show();
}
//TODO: connect TimeLine.timedout to return error
//TODO: connect TimeLine.newpoint to draw graphic

QStringList vib_temperature::result()
{
	while(result_var.count()==0 && !can_return)
	{
		qApp->processEvents();//TODO:Maybe there's a better way?
	}
	return result_var;
}

QStringList vib_temperature::set_temp(double temp,double ramp, double timeout)
{
	if (!wset.tempid.isEmpty())
	{
		result_var.clear();
		can_return=false;
		temptl.start(wset.tempid,temp,ramp,timeout,wset.getSettime(temp));
		return result();
	}
	return trUtf8("::ERROR::;Не установлен GPIB ID термоконтроллера.").split(";");
}
