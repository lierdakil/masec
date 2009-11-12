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

    //ui.graph->setScene(new QGraphicsScene());

    QErrorMessage::qtHandler();

    QSettings f("settings.ini",QSettings::IniFormat);

    ui.edGenId->setText(f.value("GPIB/genid","").toString());
    ui.edMulId->setText(f.value("GPIB/mulid","").toString());
    ui.edOscId->setText(f.value("GPIB/oscid","").toString());
    ui.sbOscVol->setValue(f.value("UPR/osc_vol",0.1).toDouble());
    ui.sbSm1->setValue(f.value("UPR/sm1",12).toInt());
    ui.sbSm2->setValue(f.value("UPR/sm2",24).toInt());
    ui.sbVPP->setValue(f.value("UPR/VPP",5).toDouble());

    thread.setParent(this);
    qRegisterMetaType<QList<qreal> >("QList<qreal>");

    connect(&thread,SIGNAL(finished(QStringList)),this,SIGNAL(finished(QStringList)));
    connect(&thread,SIGNAL(finished(QStringList)),this,SLOT(onfinished(QStringList)));
    /*connect(&thread,SIGNAL(path(QList<qreal>,QPen)),this,SLOT(path(QList<qreal>,QPen)),Qt::QueuedConnection);
    connect(&thread,SIGNAL(path(QByteArray,QPen)),this,SLOT(path(QByteArray,QPen)),Qt::QueuedConnection);
    connect(&thread,SIGNAL(line(qreal,qreal,qreal,qreal,QPen)),this,SLOT(line(qreal,qreal,qreal,qreal,QPen)),Qt::QueuedConnection);*/

    qRegisterMetaType<QVector<qreal> >("QVector<qreal>");
    connect(&thread,SIGNAL(path(QVector<qreal>,QVector<qreal>,QPen)),this,SLOT(path(QVector<qreal>,QVector<qreal>,QPen)),Qt::QueuedConnection);
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
    f.setValue("UPR/VPP",ui.sbVPP->value());
}

void vibupraut::measure(double startf, double stopf, QString filename)
{
    ui.btRun->setEnabled(false);
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
    thread.genvolpp=ui.sbVPP->value();
    //ui.graph->scene()->clear();
    ui.graph->detachItems(QwtPlotItem::Rtti_PlotItem,true);
    thread.start();
}

void vibupraut::path(QVector<qreal> xdata, QVector<qreal> ydata, QPen pen)
{
    //QPainterPath path;
    //path.moveTo(0,-data[0]);
    //for(int i=1;i<data.count();++i)
    //    path.lineTo(i,-data[i]);
    QwtPlotCurve *curve=new QwtPlotCurve;
    curve->setData(xdata,ydata);
    curve->setPen(pen);
    curve->attach(ui.graph);
    ui.graph->setAxisScale(QwtPlot::xBottom,xdata.first(),xdata.last());
    ui.graph->replot();
    //ui.graph->scene()->addPath(path,pen,Qt::NoBrush);
    //ui.graph->fitInView(ui.graph->scene()->sceneRect());
}

void vibupraut::marker(qreal x, qreal y, QPen pen)
{
    QwtPlotMarker mark;
    mark.setLinePen(pen);
    mark.setLineStyle(QwtPlotMarker::Cross);
    mark.setLabel(QwtText(QString("f = %1, V = %2").arg(x).arg(y),QwtText::AutoText));
    mark.setLabelAlignment(Qt::AlignRight | Qt::AlignBottom);
    mark.setValue(x,y);
    mark.attach(ui.graph);
}

/*void vibupraut::path(QByteArray data,QPen pen)
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
}*/

void vibupraut::on_btRun_clicked()
{
    bool ok=false;
    double sf=QInputDialog::getDouble(this,
                                      trUtf8("Начальная частота"),
                                      trUtf8("Введите, Hz"),
                                      100000,0.010,15000000,2,&ok);
    if(!ok) return;
    double ff=QInputDialog::getDouble(this,
                                      trUtf8("Конечная частота"),
                                      trUtf8("Введите, Hz"),
                                      200000,0.010,15000000,2,&ok);
    if(!ok) return;
    QString file=QInputDialog::getText(this,
                                       trUtf8("Имя файла"),
                                       trUtf8("Имя файла для сохранения "
                                              "резонансной кривой (оставьте "
                                              "пустым, чтобы не сохранять)"),
                                       QLineEdit::Normal,"",&ok);
    if(!ok) return;
    measure(sf,ff,file);
}

void vibupraut::onfinished(QStringList data)
{
    ui.lbStatus->setText(data.join("\n"));
    ui.btRun->setEnabled(true);
}
