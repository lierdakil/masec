#include "QTempTimeline.h"
#include <time.h>
#define TIMESTEP 0.01

QTempTimer::QTempTimer() : QObject()
{
    temp=NULL;
}

void QTempTimer::raiseError(QString message)
{
    is_stopped=true;
    delete temp;
    temp=0;
    emit error(message);
}

void QTempTimer::start_manual(float nsetp, float nramp, float ntimeout, float nsettime, float P, float I, float D, int range, double mout)
{
    try{
        temp=new tempctrl(qApp->property("tempid").toString());
        temp->ctrlmode(MOD_MANUAL);
        temp->setPID(P,I,D);
        temp->setrange(range);
        temp->setmout(mout);
        start(nsetp, nramp, ntimeout, nsettime);
    } catch (GPIBGenericException e) {
        raiseError(e.report());
    } catch (std::exception e) {
        raiseError(QString::fromUtf8("Caught unexpected exception '%1'").arg(e.what()));
    }
}

void QTempTimer::start_zone(float nsetp, float nramp, float ntimeout, float nsettime)
{
    try{
        temp=new tempctrl(qApp->property("tempid").toString());
        temp->ctrlmode(MOD_ZONE);
        start(nsetp, nramp, ntimeout, nsettime);
    } catch (GPIBGenericException e) {
        raiseError(e.report());
    } catch (std::exception e) {
        raiseError(QString::fromUtf8("Caught unexpected exception '%1'").arg(e.what()));
    }
}

void QTempTimer::start(float nsetp, float nramp, float ntimeout, float nsettime)
{
    //this function is called from within try...catch
    setp=nsetp;
    ramp=nramp;
    timeout=ntimeout;
    settime=nsettime;

    is_stopped=false;
    stop_requested=false;

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
    //only called from within try...catch
    time=(clock()-startclock)/60000.0f;
    QTimer::singleShot(int(60000*min),this,member);
}

QTempTimer::~QTempTimer()
{

}

void QTempTimer::rampdone()
{
    if (is_stopped)
        return;

    try{
        if (temp->rampdone())
            wait(TIMESTEP,SLOT(step1()));
        else
            wait(TIMESTEP,SLOT(rampdone()));
    } catch (GPIBGenericException e) {
        raiseError(e.report());
    } catch (std::exception e) {
        raiseError(QString::fromUtf8("Caught unexpected exception '%1'").arg(e.what()));
    }
}

bool QTempTimer::stable()
{
    //only called directly from within try...catch
    //two consequent values of temperature are near setp
    //TODO: Maybe there's a better algorithm to detect that temperature is stable?
    return ( fabs(temp1-setp)<=0.5 && fabs(temp2-setp)<=0.5 );
}

void QTempTimer::draw_temp()
{
    if(is_stopped)
        return;

    try{
        drawtime=(clock()-startclock)/60000.0f;
        emit newpoint(drawtime,temp->temp(),temp->getsetp());
        QTimer::singleShot(int(60000*TIMESTEP),this,SLOT(draw_temp()));
    } catch (GPIBGenericException e) {
        raiseError(e.report());
    } catch (std::exception e) {
        raiseError(QString::fromUtf8("Caught unexpected exception '%1'").arg(e.what()));
    }
}

void QTempTimer::step1()
{
    if(is_stopped)
        return;

    try{
        temp1=temp2;
        temp2=temp->temp();

        if(time>=timeout)
        {
            raiseError(trUtf8("Exceeded temperature setup timeout"));
        } else if(stop_requested) {
            is_stopped=true;
            delete temp;
            temp=0;
            emit stopped();
        } else if ( stable() ) {
            wait(settime,SLOT(step2()));//wait settime to check if temperature stabilized.
        } else {
            wait(TIMESTEP,SLOT(step1()));//continue cycle
        }
    } catch (GPIBGenericException e) {
        raiseError(e.report());
    } catch (std::exception e) {
        raiseError(QString::fromUtf8("Caught unexpected exception '%1'").arg(e.what()));
    }
}

void QTempTimer::step2()
{
    if(is_stopped)
        return;

    try{
        if ( stable() )
        {
            //we think that temperature is stable
            is_stopped=true;
            delete temp;
            temp=0;
            emit temp_set();
        } else {
            wait(TIMESTEP,SLOT(step1()));//continue cycle
        }
    } catch (GPIBGenericException e) {
        raiseError(e.report());
    } catch (std::exception e) {
        raiseError(QString::fromUtf8("Caught unexpected exception '%1'").arg(e.what()));
    }
}

void QTempTimer::stop()
{
    stop_requested=true;
}

