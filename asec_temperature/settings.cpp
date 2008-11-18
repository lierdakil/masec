#include "settings.h"

double settings::getSettime(double temp)
{
	//TODO: Разобраться, ибо что-то здесь не так.
	for (int n=0;n<10;n++)
	{
		double ct = ui_settings.tbTempCtrl->findChild<QDoubleSpinBox*>(QString("sbTemp_%1").arg(n))->value();
		if (temp<=ct)
			return ui_settings.tbTempCtrl->findChild<QDoubleSpinBox*>(QString("sbTime_%1").arg(n))->value();
	}
	//qFatal("getSettime failed!");
	return 0;
}

void settings::read_settings()
{
	QSettings f("settings.ini",QSettings::IniFormat);
	tempid = f.value("GPIB/tempid","").toString();

	ui_settings.edTempId->setText(tempid);

	if (!tempid.isEmpty())
	{
		tempctrl temp((char*)tempid.toAscii().data());

		for (int n=0;n<=9;n++)
		{
			float Temp, P, I, D, mout;
			char range;
			temp.readzone(n+1,&Temp,&P,&I,&D,&mout,&range);
			float time=f.value(QString("TempCtrl/Time_%1").arg(n),0).toDouble();
			ui_settings.tbTempCtrl->findChild<QDoubleSpinBox*>(QString("sbTemp_%1").arg(n))->setValue(Temp);
			ui_settings.tbTempCtrl->findChild<QDoubleSpinBox*>(QString("sbP_%1").arg(n))->setValue(P);
			ui_settings.tbTempCtrl->findChild<QDoubleSpinBox*>(QString("sbI_%1").arg(n))->setValue(I);
			ui_settings.tbTempCtrl->findChild<QDoubleSpinBox*>(QString("sbD_%1").arg(n))->setValue(D);
			ui_settings.tbTempCtrl->findChild<QDoubleSpinBox*>(QString("sbMout_%1").arg(n))->setValue(mout);
			ui_settings.tbTempCtrl->findChild<QComboBox*>(QString("cbRange_%1").arg(n))->setCurrentIndex(range);
			ui_settings.tbTempCtrl->findChild<QDoubleSpinBox*>(QString("sbTime_%1").arg(n))->setValue(time);
		}

		ui_settings.sbRamp->setValue(temp.getramp());
		float P,I,D;
		temp.readPID(&P,&I,&D);
		ui_settings.sbP_test->setValue(P);
		ui_settings.sbI_test->setValue(I);
		ui_settings.sbD_test->setValue(D);

		ui_settings.sbMout_test->setValue(temp.getmout());
		ui_settings.cbRange_test->setCurrentIndex(temp.getrange());

		ui_settings.nlT_test->setProperty("value",temp.temp());
		ui_settings.sbNewT_test->setValue(temp.getsetp());
	}
	else
	{
		ui_settings.tbTempCtrl->setEnabled(false);
		ui_settings.tbTempTest->setEnabled(false);
	}
}

settings::settings(QWidget *parent)
	: QDialog(parent)
{
	QErrorMessage::qtHandler();
	ui_settings.setupUi(this);

	read_settings();

	connect(ui_settings.btStopTest,SIGNAL(clicked()),&test,SLOT(stop()));
	connect(&test,SIGNAL(finished()),this,SLOT(finished()));
	connect(&test,SIGNAL(timedout()),this,SLOT(finished()));
	connect(&test,SIGNAL(newpoint(float,float,float)),this,SLOT(newpoint(float,float,float)));
	ui_settings.gvGraph->setScene(new QGraphicsScene());
}

settings::~settings()
{

}

void settings::accept()
{
	tempid = ui_settings.edTempId->text();
	QSettings f("settings.ini",QSettings::IniFormat);
	f.setValue("GPIB/tempid", tempid);

	if (!tempid.isEmpty() && ui_settings.gbZones->isEnabled())
	{
		tempctrl temp((char*)tempid.toAscii().data());
		QSettings f("settings.ini",QSettings::IniFormat);

		for (int n=0;n<=9;n++)
		{
			float t=ui_settings.tbTempCtrl->findChild<QDoubleSpinBox*>(QString("sbTemp_%1").arg(n))->value();
			float p=ui_settings.tbTempCtrl->findChild<QDoubleSpinBox*>(QString("sbP_%1").arg(n))->value();
			float i=ui_settings.tbTempCtrl->findChild<QDoubleSpinBox*>(QString("sbI_%1").arg(n))->value();
			float d=ui_settings.tbTempCtrl->findChild<QDoubleSpinBox*>(QString("sbD_%1").arg(n))->value();
			float mout=ui_settings.tbTempCtrl->findChild<QDoubleSpinBox*>(QString("sbMout_%1").arg(n))->value();
			int range=ui_settings.tbTempCtrl->findChild<QComboBox*>(QString("cbRange_%1").arg(n))->currentIndex();
			float time=ui_settings.tbTempCtrl->findChild<QDoubleSpinBox*>(QString("sbTime_%1").arg(n))->value();
			temp.setzone(n+1,t,p,i,d,mout,range);
			f.setValue(QString("TempCtrl/Time_%1").arg(n),time);
		}
	}

	QDialog::accept();
}

void settings::finished()
{
	ui_settings.btStopTest->setEnabled(false);
	ui_settings.btTest->setEnabled(true);
}

void settings::newpoint(float time, float temp, float setpoint)
{
	static float lasttime;
	static float lasttemp;
	static float lastsetp;
	if(time>lasttime)
	{
		ui_settings.gvGraph->scene()->addLine(lasttime,lasttemp,time,temp,QPen(Qt::red));
		ui_settings.gvGraph->scene()->addLine(lasttime,lastsetp,time,setpoint,QPen(Qt::red));
		ui_settings.gvGraph->fitInView(ui_settings.gvGraph->scene()->sceneRect());
	}
	lasttime=time;
	lasttemp=temp;
	lastsetp=setpoint;
}

void settings::on_btStopTest_clicked()
{
	test.stop();
}

void settings::on_btTest_clicked()
{
	if (ui_settings.edTempId->text().isEmpty())
		return;
	tempctrl *temp = new tempctrl(ui_settings.edTempId->text());

	ui_settings.nlT_test->setProperty("value",temp->temp());

	temp->ctrlmode(MOD_MANUAL);
	temp->setPID(ui_settings.sbP_test->value(),ui_settings.sbI_test->value(),ui_settings.sbD_test->value());
	temp->setrange(ui_settings.cbRange_test->currentIndex());
	temp->setmout(ui_settings.sbMout_test->value());

	ui_settings.btStopTest->setEnabled(true);
	ui_settings.btTest->setEnabled(false);

	ui_settings.gvGraph->scene()->clear();

	delete temp;
	test.start(
			ui_settings.edTempId->text(),
			ui_settings.sbNewT_test->value(),
			ui_settings.sbRamp->value(),
			ui_settings.sbt_test->value(),
			getSettime(ui_settings.sbNewT_test->value())
			);
}