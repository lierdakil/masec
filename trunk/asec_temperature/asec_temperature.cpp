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
	ReplyInterface reply;
	reply.data<<QString("Setpoint,K:%1").arg(temptl.setp);
	reply.data<<QString("Temperature 1,K:%1").arg(temptl.temp1);
	reply.data<<QString("Temperature 2,K:%1").arg(temptl.temp2);
	reply.data<<QString("Ramp,K/min:%1").arg(temptl.ramp);
	reply.data<<QString("Timeout,min:%1").arg(temptl.timeout);
	reply.data<<QString("Stabilization time,min:%1").arg(temptl.settime);
}

void vib_temperature::timedout()
{
	ReplyInterface reply;
	reply.data<<trUtf8("::ERROR::");
	reply.data<<trUtf8("Exceeded temperature setup timeout");
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

void vib_temperature::set_temp(double temp,double ramp, double timeout)
{
	if (!wset.tempid.isEmpty())
	{
		result_var.clear();
		ui.gvTemp->scene()->clear();
		temptl.start(wset.tempid,temp,ramp,timeout,wset.getSettime(temp));
	}
	else
	{
		ReplyInterface reply;
		reply.data<<trUtf8("::ERROR::");
		reply.data<<trUtf8("Не установлен GPIB ID термоконтроллера.");
	}
}
