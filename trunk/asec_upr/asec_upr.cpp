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

    ui.graph->setScene(new QGraphicsScene());

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

void vibupraut::measure(double startf, double stopf, QString filename)
{
    thread.startf=startf;
    thread.stopf=stopf;
    thread.filename=filename;
    thread.oscid=ui.edOscId->text();
    thread.genid=ui.edGenId->text();
    thread.mulid=ui.edMulId->text();
    ui.graph->scene()->clear();
    thread.view=ui.graph;
    thread.wait();
    thread.start();
}

void MeasureThread::run()
{
#ifdef DEBUG
	view->scene()->addLine(0,0,5,5,Qt::SolidLine);
	sleep(60);
	ReplyInterface reply;
	reply.data<<QString("First run start freq, Hz:%1").arg(startf);
	reply.data<<QString("First run stop freq, Hz:%1").arg(stopf);
	reply.data<<QString("Second run start freq, Hz:%1").arg(startf);
	reply.data<<QString("Second run stop freq, Hz:%1").arg(startf);
	reply.data<<QString("Resonance freq, Hz:%1").arg((startf+stopf)/2);
	reply.data<<QString("Resonance ampl, V:%1").arg(5);
	reply.data<<QString("Antiresonance freq, Hz:%1").arg((startf+stopf)/2);
	reply.data<<QString("Antiresonance ampl, V:%1").arg(2);
#else
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

    cmeasure mes(oscid,genid,mulid,startf,stopf,0.1,view);

    if(!filename.isEmpty())
    {
        QFile f(filename);
        f.open(QFile::WriteOnly);
        QString buf;
        for(int i=0;i<mes.curve.count();i++)
        {
            buf.setNum(mes.curve.at(i).first);
            qDebug()<<buf;
            f.write(buf.toAscii());
            f.write("\t");
            buf.setNum(mes.curve.at(i).second);
            qDebug()<<buf;
            f.write(buf.toAscii());
            f.write("\n");
        }
        f.close();
    }

    ReplyInterface reply;
    reply.data<<QString("First run start freq, Hz:%1").arg(mes.fsf);
    reply.data<<QString("First run stop freq, Hz:%1").arg(mes.fff);
    reply.data<<QString("Second run start freq, Hz:%1").arg(mes.ssf);
    reply.data<<QString("Second run stop freq, Hz:%1").arg(mes.sff);
    reply.data<<QString("Resonance freq, Hz:%1").arg(mes.rf);
    reply.data<<QString("Resonance ampl, V:%1").arg(mes.ra);
    reply.data<<QString("Antiresonance freq, Hz:%1").arg(mes.af);
    reply.data<<QString("Antiresonance ampl, V:%1").arg(mes.aa);
    //reply is sent upon destruction
#endif
}
