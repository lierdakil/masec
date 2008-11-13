#include "QTempTimeline.h"

QTempTimer::QTempTimer() : QObject()
{

}

void QTempTimer::start(QString tempid, float nsetp, float nramp, float ntimeout, float nsettime)
{
	setp=nsetp;
	ramp=nramp;
	timeout=ntimeout;
	settime=nsettime;

    temp=new tempctrl(tempid);
    ramp=temp->ramp(ramp);

    temp2=temp->temp();
    time=0;

    drawtime=0;
    QTimer::singleShot(int(60000*TIMESTEP),this,SLOT(draw_temp()));

    float setp0=temp->getsetp();
    temp->setpoint(setp);
    if(ramp>0)
    {
    	wait(fabs(setp-setp0)/ramp,SLOT(rampdone()));//wait until system dragged setpoint.
    }
    else
    {
    	wait(TIMESTEP,SLOT(step1()));
    }
}

void QTempTimer::wait(float min, const char* member)
{
	dt=min;
	time+=min;
	QTimer::singleShot(int(60000*min),this,member);
}

QTempTimer::~QTempTimer()
{

}

void QTempTimer::rampdone()
{
	if (temp->rampdone())
		wait(TIMESTEP,SLOT(step1()));
	else
		wait(TIMESTEP,SLOT(rampdone()));
}

bool QTempTimer::stable()
{
	//two consequent values of temperature are near setp
	//TODO: Maybe there's a better algorithm to detect that temperature is stable?
	return ( fabs(temp1-setp)<=0.5 && fabs(temp2-setp)<=0.5 );
}

void QTempTimer::draw_temp()
{
	drawtime+=TIMESTEP;
	emit newpoint(drawtime,temp->temp(),temp->getsetp());
	QTimer::singleShot(int(60000*TIMESTEP),this,SLOT(draw_temp()));
}

void QTempTimer::step1()
{
    temp1=temp2;
    temp2=temp->temp();

    if(time>=timeout)
    {
    	emit timedout();
    	return;
    }

    if ( stable() )
    	wait(settime,SLOT(step2()));//wait settime to check if temperature stabilized.
    else
    	wait(TIMESTEP,SLOT(step1()));//continue cycle
}

void QTempTimer::step2()
{
	if ( stable() )
	{
    	//we think that temperature is stable
		delete temp;
		emit finished();
	} else {
		wait(TIMESTEP,SLOT(step1()));//continue cycle
	}
}

void QTempTimer::stop()
{
	timeout=time;
}

