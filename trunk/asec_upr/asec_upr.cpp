#include "asec_upr.h"
#include "adaptors.h"
#include <QInputDialog>

vibupraut::vibupraut(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    new export_adaptor(this);
    new help_adaptor(this);
    new flow_adaptor(this);
    QDBusConnection connection = QDBusConnection::sessionBus();
    connection.registerObject("/", this);
    connection.registerService("ru.pp.livid.asec.upr");

    ui.graph->setScene(new QGraphicsScene());

    QErrorMessage::qtHandler();

    QSettings f("settings.ini",QSettings::IniFormat);

    ui.edGenId->setText(f.value("GPIB/genid","").toString());
    ui.edMulId->setText(f.value("GPIB/mulid","").toString());
    ui.edOscId->setText(f.value("GPIB/oscid","").toString());
    ui.sbOscVol->setValue(f.value("UPR/osc_vol",0.1).toDouble());
    ui.sbSm1->setValue(f.value("UPR/sm1",12).toInt());
    ui.sbSm2->setValue(f.value("UPR/sm2",24).toInt());

    thread.setParent(this);
    qRegisterMetaType<QList<qreal> >("QList<qreal>");

    connect(&thread,SIGNAL(finished(QStringList)),this,SIGNAL(finished(QStringList)));
    connect(&thread,SIGNAL(path(QList<qreal>,QPen)),this,SLOT(path(QList<qreal>,QPen)),Qt::QueuedConnection);
    connect(&thread,SIGNAL(path(QByteArray,QPen)),this,SLOT(path(QByteArray,QPen)),Qt::QueuedConnection);
    connect(&thread,SIGNAL(line(qreal,qreal,qreal,qreal,QPen)),this,SLOT(line(qreal,qreal,qreal,qreal,QPen)),Qt::QueuedConnection);
}

vibupraut::~vibupraut()
{
    QSettings f("settings.ini",QSettings::IniFormat);
    f.setValue("GPIB/genid", ui.edGenId->text());
    f.setValue("GPIB/mulid", ui.edMulId->text());
    f.setValue("GPIB/oscid", ui.edOscId->text());
    f.setValue("UPR/osc_vol", ui.sbOscVol->value());
    f.setValue("UPR/sm1",ui.sbSm1->value());
    f.setValue("UPR/sm2",ui.sbSm2->value());
}

void vibupraut::measure(double startf, double stopf, QString filename)
{
    thread.wait();
    thread.startf=startf;
    thread.stopf=stopf;
    thread.filename=filename;
    thread.oscid=ui.edOscId->text();
    thread.genid=ui.edGenId->text();
    thread.mulid=ui.edMulId->text();
    thread.volts1=ui.sbOscVol->value();
    thread.sm1=ui.sbSm1->value();
    thread.sm2=ui.sbSm2->value();
    ui.graph->scene()->clear();
    thread.start();
}

void vibupraut::path(QList<qreal> data,QPen pen)
{
    QPainterPath path;
    path.moveTo(0,-data[0]);
    for(int i=1;i<data.count();++i)
        path.lineTo(i,-data[i]);
    ui.graph->scene()->addPath(path,pen,Qt::NoBrush);
    ui.graph->fitInView(ui.graph->scene()->sceneRect());
}

void vibupraut::path(QByteArray data,QPen pen)
{
    QPainterPath path;
    path.moveTo(0,-data[0]);
    for(int i=1;i<data.count();++i)
        path.lineTo(i,-data[i]);
    ui.graph->scene()->addPath(path,pen,Qt::NoBrush);
    ui.graph->fitInView(ui.graph->scene()->sceneRect());
}

void vibupraut::line(qreal x1, qreal y1,qreal x2, qreal y2, QPen pen)
{
    ui.graph->scene()->addLine(x1,y1,x2,y2,pen);
    ui.graph->fitInView(ui.graph->scene()->sceneRect());
}

void vibupraut::on_btRun_clicked()
{
    double sf=QInputDialog::getDouble(this,trUtf8("Начальная частота"),trUtf8("Введите, Hz"),100000);
    double ff=QInputDialog::getDouble(this,trUtf8("Конечная частота"),trUtf8("Введите, Hz"),200000);
    QString file=QInputDialog::getText(this,trUtf8("Имя файла"),trUtf8("Имя файла для сохранения резонансной кривой (оставьте пустым, чтобы не сохранять)"));
    measure(sf,ff,file);
}
