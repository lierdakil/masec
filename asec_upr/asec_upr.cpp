#include "asec_upr.h"

vibupraut::vibupraut(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);

	new export_adaptor(this);
	new help_adaptor(this);
	QDBusConnection connection = QDBusConnection::sessionBus();
	connection.registerObject("/", this);
	connection.registerService("ru.pp.livid.asec.upr");

	QSettings f("settings.ini",QSettings::IniFormat);

	ui.edGenId->setText(f.value("GPIB/genid","").toString());
	ui.edMulId->setText(f.value("GPIB/mulid","").toString());
	ui.edOscId->setText(f.value("GPIB/oscid","").toString());
}

vibupraut::~vibupraut()
{
	QSettings f("settings.ini",QSettings::IniFormat);
	f.setValue("GPIB/genid", ui.edGenId->text());
	f.setValue("GPIB/mulid", ui.edMulId->text());
	f.setValue("GPIB/oscid", ui.edOscId->text());
}

QStringList vibupraut::measure(double startf, double stopf, QString filename)
{
	if (ui.graph->scene()!=0)
		delete ui.graph->scene();

	ui.graph->setScene(new QGraphicsScene(ui.graph));

	thread.startf=startf;
	thread.stopf=stopf;
	thread.filename=filename;
	thread.scene=ui.graph->scene();
	thread.oscid=ui.edOscId->text();
	thread.genid=ui.edGenId->text();
	thread.mulid=ui.edMulId->text();
	thread.wait();
	thread.start();
	thread.wait();
	return thread.result;
}

void MeasureThread::run()
{
	if (oscid.isEmpty() || genid.isEmpty() || mulid.isEmpty())
	{
		if(oscid.isEmpty())
			qWarning()<<"Oscilloscope GPIB ID is empty";
		if(genid.isEmpty())
			qWarning()<<"Generator GPIB ID is empty";
		if(mulid.isEmpty())
			qWarning()<<"Multimeter GPIB ID is empty";
		return;
	}

	result.clear();
	cmeasure mes(oscid,genid,mulid,startf,stopf,0.1,scene);

	if(!filename.isEmpty())
	{
		QFile f(filename);
		f.open(QFile::WriteOnly);
		QString buf;
		for(int i=0;i<mes.curve.count();i++)
		{
			buf.setNum(mes.curve.at(i).first);
			f.write(buf.toAscii());
			f.write("\t");
			buf.setNum(mes.curve.at(i).second);
			f.write(buf.toAscii());
			f.write("\n");
		}
		f.close();
	}

	result<<QString("First run start freq, Hz:%1").arg(mes.fsf);
	result<<QString("First run stop freq, Hz:%1").arg(mes.fff);
	result<<QString("Second run start freq, Hz:%1").arg(mes.ssf);
	result<<QString("Second run stop freq, Hz:%1").arg(mes.sff);
	result<<QString("Resonance freq, Hz:%1").arg(mes.rf);
	result<<QString("Resonance ampl, V:%1").arg(mes.ra);
	result<<QString("Antiresonance freq, Hz:%1").arg(mes.af);
	result<<QString("Antiresonance ampl, V:%1").arg(mes.aa);
}
