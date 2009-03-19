#include "QTempTimeline.h"
#include <time.h>
#define TIMESTEP 0.01

QTempTimer::QTempTimer() : QObject()
{
    temp=NULL;
}

void QTempTimer::start_manual(float nsetp, float nramp, float ntimeout, float nsettime, float P, float I, float D, int range, double mout)
{
    temp=new tempctrl(qApp->property("tempid").toString());
    temp->ctrlmode(MOD_MANUAL);
    temp->setPID(P,I,D);
    temp->setrange(range);
    temp->setmout(mout);
    start(nsetp, nramp, ntimeout, nsettime);
}

void QTempTimer::start_zone(float nsetp, float nramp, float ntimeout, float nsettime)
{
    temp=new tempctrl(qApp->property("tempid").toString());
    temp->ctrlmode(MOD_ZONE);
    start(nsetp, nramp, ntimeout, nsettime);
}

void QTempTimer::start(float nsetp, float nramp, float ntimeout, float nsettime)
{
    setp=nsetp;
    ramp=nramp;
    timeout=ntimeout;
    settime=nsettime;

    is_stopped=false;

    ramp=temp->ramp(ramp);

    temp2=temp->temp();
    time=0;

    drawtime=0;
    QTimer::singleShot(int(60000*TIMESTEP),this,SLOT(draw_temp()));

    float setp0=temp->getsetp();
    temp->setpoint(setp);
    startclock=clock();
    if(ramp>0)
    {
        wait(fabs(setp-setp0)/ramp,SLOT(rampdone()));//wait until system dragged setpoint.
    }
    else
    {
        wait(TIMESTEP,SLOT(step1()));
    }
}

void QTempTimer::wait(double min, const char* member)
{
    time=(clock()-startclock)/60000.0f;
    QTimer::singleShot(int(60000*min),this,member);
}

QTempTimer::~QTempTimer()
{

}

void QTempTimer::rampdone()
{
    if (!is_stopped)
    {
        if (temp->rampdone())
            wait(TIMESTEP,SLOT(step1()));
        else
            wait(TIMESTEP,SLOT(rampdone()));
    }
}

bool QTempTimer::stable()
{
    //two consequent values of temperature are near setp
    //TODO: Maybe there's a better algorithm to detect that temperature is stable?
    return ( fabs(temp1-setp)<=0.5 && fabs(temp2-setp)<=0.5 );
}

void QTempTimer::draw_temp()
{
    if(is_stopped)
        return;
    drawtime=(clock()-startclock)/60000.0f;
    emit newpoint(drawtime,temp->temp(),temp->getsetp());
    QTimer::singleShot(int(60000*TIMESTEP),this,SLOT(draw_temp()));
}

void QTempTimer::step1()
{
    temp1=temp2;
    temp2=temp->temp();

    if(time>=timeout)
    {
        delete temp;
        temp=0;
        is_stopped=true;
        emit timedout();
    } else if(is_stopped) {
        delete temp;
        temp=0;
        emit stopped();
    } else if ( stable() ) {
        wait(settime,SLOT(step2()));//wait settime to check if temperature stabilized.
    } else {
        wait(TIMESTEP,SLOT(step1()));//continue cycle
    }
}

void QTempTimer::step2()
{
    if ( stable() )
    {
        //we think that temperature is stable
        //		delete temp;
        is_stopped=true;
        delete temp;
        temp=0;
        emit temp_set();
    } else {
        wait(TIMESTEP,SLOT(step1()));//continue cycle
    }
}

void QTempTimer::stop()
{
    is_stopped=true;
}

