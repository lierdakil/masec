#include "QTempTimeline.h"

QTempTimeline::QTempTimeline() : QTimeLine()
{
    setCurveShape(QTimeLine::LinearCurve);
    setDirection(QTimeLine::Forward);
    connect(this,SIGNAL(valueChanged(qreal)),this,SLOT(vC(qreal)));
    connect(this,SIGNAL(finished()),this,SLOT(finish()));
}

void QTempTimeline::start(tempctrl *tempctl, float newtemp, int timeout, double settime, QGraphicsView *view)
{
    temp=tempctl;

    first=true;
    waittime=settime;
    waiting=0;

    setDuration(timeout);
    setUpdateInterval(60);

    if (view->scene()==0)
    {
        scene=new QGraphicsScene();
        view->setScene(scene);
    }
    else
        scene=view->scene();

    QList<QGraphicsItem*> L = scene->items();

    while( ! L.empty() ) {
        //myscene->removeItem( L.first() );
        delete L.first();
        L.removeFirst();
    }

    scene->update(scene->sceneRect());

    view->repaint();

    setp=newtemp;

    setCurrentTime(0);

    temp2=temp->temp();
    t0=0;

    h=(setp-temp2)*2;
    if(h==0) h=1;
    scene->setSceneRect(t0,temp2,timeout,h);
    h=2*temp2+h;

    view->fitInView(scene->sceneRect());

    scene->addLine(t0,h-temp2,timeout+t0,h-temp2,Qt::SolidLine);
    scene->addLine(t0,h-setp,timeout+t0,h-setp,Qt::DashLine);

    temp->setpoint(setp);
    setp2=temp->getsetp();
    QTimeLine::start();
}

QTempTimeline::~QTempTimeline()
{

}


void QTempTimeline::vC(qreal value)
{
    if (currentTime()==0)
        return;
    temp1=temp2;
    temp2=temp->temp();
    setp1=setp2;
    setp2=temp->getsetp();
    dt=updateInterval();
    t0=currentTime();
    scene->addLine(t0,h-temp1,t0+dt,h-temp2,Qt::SolidLine);
    scene->addLine(t0,h-setp1,t0+dt,h-setp2,Qt::DotLine);

    if (waiting>0)
    {
        waiting-=dt/1000.0;
        return;
    }

    if (temp->rampdone())
    {
        if ( fabs(temp1-setp)<=0.5 && fabs(temp2-setp)<=0.5 )
        {
            if (first)
            {
                waiting=waittime;
                first=false;
            }
            else
            {
                stop();
                delete temp;
                emit reached(temp1,temp2,setp);
            }
        } else {
            first = true;
        }
    }
}

void QTempTimeline::finish()
{
    //QDBusInterface iface("ru.pp.livid.vib.con","/Main","ru.pp.livid.iface");
    //iface->call("err");
    //TODO: Check if this gets called
    delete temp;
}

