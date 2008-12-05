#include "asec_temperature.h"
#include "adaptors.h"

vib_temperature::vib_temperature(QWidget *parent)
: QWidget(parent)
{
	ui.setupUi(this);

	QErrorMessage::qtHandler();

	QDBusConnection connection = QDBusConnection::sessionBus();
	connection.registerService("ru.pp.livid.asec.temp");
	connection.registerObject("/", this);
	new export_adaptor(this);
	new help_adaptor(this);
	new flow_adaptor(this);

	ui.gvTemp->setScene(new QGraphicsScene());

//	Current paradigm supposes fix_timer is run at program start and ended at termination
//	connect(&temptl,SIGNAL(temp_set()),&fix_timer,SLOT(stop()));
//	connect(&temptl,SIGNAL(timedout()),&fix_timer,SLOT(stop()));
//	connect(&temptl,SIGNAL(stopped()),&fix_timer,SLOT(stop()));

	connect(&temptl,SIGNAL(temp_set()),this,SLOT(temp_set()));
	connect(&temptl,SIGNAL(timedout()),this,SLOT(timedout()));
	connect(&temptl,SIGNAL(stopped()),this,SLOT(stopped()));
	connect(&temptl,SIGNAL(newpoint(float,float,float)),this,SLOT(newpoint(float,float,float)));

	connect(&fix_timer, SIGNAL(timeout()), this, SLOT(fix_range()));
}

vib_temperature::~vib_temperature()
{

}

void vib_temperature::temp_set()
{
	QStringList data;
	data<<QString("Setpoint,K:%1").arg(temptl.setp);
	data<<QString("Temperature 1,K:%1").arg(temptl.temp1);
	data<<QString("Temperature 2,K:%1").arg(temptl.temp2);
	data<<QString("Ramp,K/min:%1").arg(temptl.ramp);
	data<<QString("Timeout,min:%1").arg(temptl.timeout);
	data<<QString("Stabilization time,min:%1").arg(temptl.settime);
	emit finished(data);
}

void vib_temperature::timedout()
{
	QStringList data;
	data<<trUtf8("::ERROR::");
	data<<trUtf8("Exceeded temperature setup timeout");
	emit finished(data);
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
}

void vib_temperature::fix_range()
{
	tempctrl *temp=(tempctrl*)(qApp->property("temp").toInt());
	switch(temp->getctrlmode())
	{
	case MOD_ZONE:
		temp->setrange(wset.getRange(temp->temp()));
		break;
	case MOD_MANUAL:
		temp->setrange(wset.getRangeManual());
		break;
	}
}

void vib_temperature::set_temp(double temp,double ramp, double timeout)
{
	if (!wset.tempid.isEmpty())
	{
		result_var.clear();
		delete ui.gvTemp->scene();
		ui.gvTemp->setScene(new QGraphicsScene());
		tempctrl *tempctl=(tempctrl*)(qApp->property("temp").toInt());
		tempctl->ctrlmode(MOD_ZONE);
		temptl.start(wset.tempid,temp,ramp,timeout,wset.getSettime(temp));
	}
	else
	{
		QStringList data;
		data<<trUtf8("::ERROR::");
		data<<trUtf8("Не установлен GPIB ID термоконтроллера.");
		emit finished(data);
	}
}
