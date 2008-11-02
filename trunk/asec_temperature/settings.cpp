#include "settings.h"

double settings::getSettime(double temp)
{
	for (int n=9;n>=0;n--)
	{
		double ct = ui_settings.tbTempCtrl->findChild<QDoubleSpinBox*>(QString("sbTemp_%1").arg(n))->value();
		if (temp>=ct)
			return ui_settings.tbTempCtrl->findChild<QDoubleSpinBox*>(QString("sbTime_%1").arg(n))->value();
	}
	//TODO: Generate runtime exception
	return 0;
}

void settings::read_settings()
{
	QSettings f("settings.ini",QSettings::IniFormat);
	tempid = f.value("GPIB/tempid","").toString();
	
	ui_settings.edTempId->setText(tempid);
	
	//iface = new QDBusInterface("ru.pp.livid.vib.con","/Settings","ru.pp.livid.iface");

	//QDBusReply<QString> tempid = iface->call("TempID");
		
	if (!tempid.isEmpty())
	{
		/* ��������� ��������� � ����� � ����������� ����������� */
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
	connect(&test,SIGNAL(stateChanged(QTimeLine::State)),this,SLOT(test_stateChange(QTimeLine::State)));
}

settings::~settings()
{
	//delete iface;
}

void settings::accept()
{
	//QDBusReply<QString> tempid = iface->call("TempID");
	tempid = ui_settings.edTempId->text();
	QSettings f("settings.ini",QSettings::IniFormat);
	f.setValue("GPIB/tempid", tempid);
	
	if (!tempid.isEmpty() && ui_settings.gbZones->isEnabled())
	{
		/* �������� ���������� � ����� � ����������� ����������� */
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

void settings::on_btTest_clicked()
{	
	if (ui_settings.edTempId->text().isEmpty())
		return;
	tempctrl *temp = new tempctrl((char*)ui_settings.edTempId->text().toAscii().data());
	
	ui_settings.nlT_test->setProperty("value",temp->temp());
	
	temp->ctrlmode(MOD_MANUAL);
	temp->setPID(ui_settings.sbP_test->value(),ui_settings.sbI_test->value(),ui_settings.sbD_test->value());
	temp->setrange(ui_settings.cbRange_test->currentIndex());
	temp->setmout(ui_settings.sbMout_test->value());
	temp->ramp( ui_settings.sbRamp->value() );
	
	test.start(temp,ui_settings.sbNewT_test->value(),int(ui_settings.sbt_test->value()*1000), getSettime(ui_settings.sbNewT_test->value()),ui_settings.gvGraph);
}

void settings::test_stateChange(QTimeLine::State newState)
{
	switch(newState)
	{
	case QTimeLine::NotRunning:
		ui_settings.btTest->setEnabled(true);
		ui_settings.btStopTest->setEnabled(false);
		break;
	case QTimeLine::Paused:
		qWarning()<<"Impossible timer state?!";
		break;
	case QTimeLine::Running:
		ui_settings.btTest->setEnabled(false);
		ui_settings.btStopTest->setEnabled(true);
		break;
	}
}