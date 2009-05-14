#include "QTempTimeline.h"
#include <time.h>
#define TIMESTEP 500 //ms
#define do_step(st) step_num=st;QTimer::singleShot(TIMESTEP,this,SLOT(step()))

//Steps
#define STEP_RAMP 0 //Wait until ramp finishes
#define STEP_WAIT 1 //Test for settime if temperature stays in TEMP_WINDOW

//#define TEMP_MEM 128 //points
#define TEMP_WINDOW 0.25 //+-Kelvin

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
        if(temp->getctrlmode()!=MOD_MANUAL)
            temp->ctrlmode(MOD_MANUAL);
        float oldP, oldI, oldD;
        temp->readPID(&oldP,&oldI,&oldD);
        if(oldP!=P || oldI!=I || oldD != D)
            temp->setPID(P,I,D);
        if (temp->getrange()!=range)
            temp->setrange(range);
        if (temp->getmout()!=mout)
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
        if(temp->getctrlmode()!=MOD_ZONE)
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
    Tmin=-1;
    Tmax=-1;

    is_stopped=false;

    ramp=temp->ramp(ramp);

    float setp0=temp->getsetp();
    if(setp0!=setp)
        temp->setpoint(setp);
    fstartclock=clock()/60000.0f;//in minutes

    if(ramp>0 && setp!=setp0)
    {
        ramptime=fabs(setp-setp0)/ramp;
        emit newstatus(STATUS_RAMP);
        do_step(STEP_RAMP);
    }
    else//RAMP OFF
    {
        ramptime=0;
        testtime=0;
        emit newstatus(STATUS_WAIT);
        do_step(STEP_WAIT);
    }
}

QTempTimer::~QTempTimer()
{

}

void QTempTimer::step()
{
    if(is_stopped)
        return;

    try{
        float ftemp = temp->temp();//current temperature
        float fsetp = temp->getsetp();//current setpoint
        float ftime=clock()/60000.0f-fstartclock;//minutes elapsed from start
        //draw
        emit newpoint(ftime,ftemp,fsetp);

        //and corresponding times
        /*times.push_back(ftime);
        if(times.count()>TEMP_MEM)
            times.pop_front();*/

        //select action
        switch(step_num)
        {
        case STEP_RAMP:
            if(ftime>ramptime && temp->rampdone()) {
                //ramp finished
                //on to next step
                testtime=ftime;
                emit newstatus(STATUS_WAIT);
                do_step(STEP_WAIT);
            } else {
                //ramp is ongoing
                do_step(STEP_RAMP);
            }
            break;
        case STEP_WAIT:
            if(fabs(ftemp-fsetp)>TEMP_WINDOW){
                //temperature is out of window
                //reset timer
                testtime=ftime;
                emit newstatus(STATUS_WAIT);
                do_step(STEP_WAIT);
            } else if ( (ftime-testtime) >=settime ){
                //temperature is stable for settime minutes
                //send signal that module finished in success
                is_stopped=true;
                delete temp;
                temp=0;
                emit newstatus(STATUS_FINISHED);
                emit temp_set();
            } else if ( ftime > timeout ) {
                emit newstatus(STATUS_TMOUT);
                raiseError(tr("Temperature setup exceeded time limit"));
            } else {
                //temperature is in window, but settime did not elapse yet
                //resume testing
                if(Tmin>ftemp || Tmin<0) Tmin=ftemp;
                if(Tmax<ftemp || Tmax<0) Tmax=ftemp;
                emit newstatus(STATUS_WINDOW);
                do_step(STEP_WAIT);
            }
            break;
        }
    } catch (GPIBGenericException e) {
        raiseError(e.report());
    } catch (std::exception e) {
        raiseError(QString::fromUtf8("Caught unexpected exception '%1'").arg(e.what()));
    }
}

void QTempTimer::stop()
{
    if(!is_stopped)
    {
        is_stopped=true;
        delete temp;
        temp=0;
        emit stopped();
    }
}

