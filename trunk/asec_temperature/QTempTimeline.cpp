#include "QTempTimeline.h"
#include <time.h>
#define TIMESTEP 500 //ms
#define do_step(st) step_num=st;QTimer::singleShot(TIMESTEP,this,SLOT(step()))

//Steps
#define STEP_RAMP 0 //Wait until ramp finishes
#define STEP_WAIT 1 //Wait until temperature stays in TEMP_WINDOW for TEMP_MEM points
#define STEP_TEST 2 //Test for settime if temperature stays in TEMP_WINDOW

#define TEMP_MEM 128 //points
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

    is_stopped=false;

    temps.clear();
    //times.clear();

    ramp=temp->ramp(ramp);

    temps.push_back(temp->temp());
    //times.push_back(clock());

    float setp0=temp->getsetp();
    if(setp0!=setp)
        temp->setpoint(setp);
    fstartclock=clock()/60000.0f;//in minutes

    if(ramp>0)
    {
        ramptime=fabs(setp-setp0)/ramp;
        do_step(STEP_RAMP);
    }
    else//RAMP OFF
    {
        ramptime=0;
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

        //remember last TEMP_MEM temperature values
        temps.push_back(ftemp);
        if(temps.count()>TEMP_MEM)
            temps.pop_front();

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
                do_step(STEP_WAIT);
            } else {
                //ramp is ongoing
                do_step(STEP_RAMP);
            }
            break;
        case STEP_WAIT:
            if( stable() ){
                //temperature has been in TEMP_WINDOW for TEMP_MEM steps
                testtime=ftime;
                do_step(STEP_TEST);
            } else {
                //continue to wait
                do_step(STEP_WAIT);
            }
            break;
        case STEP_TEST:
            if(fabs(ftemp-fsetp)>TEMP_WINDOW){
                //temperature is out of window
                //go to previous step
                do_step(STEP_WAIT);
            } else if ( (ftime-testtime) >=settime ){
                //temperature is stable for settime minutes
                //send signal that module finished in success
                is_stopped=true;
                delete temp;
                temp=0;
                emit temp_set();
            } else {
                //temperature is still in window, but settime did not elapse yet
                //resume testing
                do_step(STEP_TEST);
            }
            break;
        }
    } catch (GPIBGenericException e) {
        raiseError(e.report());
    } catch (std::exception e) {
        raiseError(QString::fromUtf8("Caught unexpected exception '%1'").arg(e.what()));
    }
}

bool QTempTimer::stable()
{
    //only called directly from within try...catch
    //two consequent values of temperature are near setp and
    //deriative by two last points <= 1/2
    //TODO: Maybe there's a better algorithm to detect that temperature is stable?
    if(temps.count()<TEMP_MEM)
        return false;

    bool stable=true;
    foreach(float t, temps)
    {
        stable = stable && (fabs(t-setp)<=TEMP_WINDOW);
    }
    return stable;
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

