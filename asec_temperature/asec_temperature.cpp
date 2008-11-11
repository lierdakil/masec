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

	ui.gvTemp->setScene(new QGraphicsScene());

	connect(&temptl,SIGNAL(finished()),this,SLOT(finished()));
	connect(&temptl,SIGNAL(timedout()),this,SLOT(timedout()));
	connect(&temptl,SIGNAL(newpoint(float,float,float)),this,SLOT(newpoint(float,float,float)));
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

void vib_temperature::timedout()
{
	result_var<<trUtf8("::ERROR::");
	result_var<<trUtf8("Exceeded temperature setup timeout");
	can_return=true;
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
	if(time>lasttime)
	{
		ui.gvTemp->scene()->addLine(lasttime,lasttemp,time,temp,QPen(Qt::red));
		ui.gvTemp->scene()->addLine(lasttime,lastsetp,time,setpoint,QPen(Qt::red));
		ui.gvTemp->fitInView(ui.gvTemp->scene()->sceneRect());
	}
	lasttime=time;
	lasttemp=temp;
	lastsetp=setpoint;
}

QStringList vib_temperature::result()
{
	while(!can_return)
	{
		qApp->processEvents(QEventLoop::WaitForMoreEvents);//TODO:Maybe there's a better way?
	}
	return result_var;
}

QStringList vib_temperature::set_temp(double temp,double ramp, double timeout)
{
	if (!wset.tempid.isEmpty())
	{
		result_var.clear();
		can_return=false;
		ui.gvTemp->scene()->clear();
		temptl.start(wset.tempid,temp,ramp,timeout,wset.getSettime(temp));
		return result();
	}
	return trUtf8("::ERROR::;Не установлен GPIB ID термоконтроллера.").split(";");
}
