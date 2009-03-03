#include "QTempTimeline.h"
#include <time.h>
#define TIMESTEP 0.01

QTempTimer::QTempTimer() : QObject()
{

}

void QTempTimer::start(float nsetp, float nramp, float ntimeout, float nsettime)
{
	setp=nsetp;
	ramp=nramp;
	timeout=ntimeout;
	settime=nsettime;

	is_stopped=false;

	tempctrl *temp=(tempctrl*)(qApp->property("temp").toInt());

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
	tempctrl *temp=(tempctrl*)(qApp->property("temp").toInt());
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
	tempctrl *temp=(tempctrl*)(qApp->property("temp").toInt());
	drawtime=(clock()-startclock)/60000.0f;
	emit newpoint(drawtime,temp->temp(),temp->getsetp());
	if(!is_stopped)
		QTimer::singleShot(int(60000*TIMESTEP),this,SLOT(draw_temp()));
}

void QTempTimer::step1()
{
	tempctrl *temp=(tempctrl*)(qApp->property("temp").toInt());
    temp1=temp2;
    temp2=temp->temp();

    if(time>=timeout)
    {
    	//delete temp;
    	is_stopped=true;
    	emit timedout();
    } else if(is_stopped) {
    	emit stopped();
    } else if ( stable() ) {
    	wait(settime,SLOT(step2()));//wait settime to check if temperature stabilized.
    } else {
    	wait(TIMESTEP,SLOT(step1()));//continue cycle
    }
}

void QTempTimer::step2()
{
	//tempctrl *temp=(tempctrl*)(qApp->property("temp").toInt());
	if ( stable() )
	{
    	//we think that temperature is stable
//		delete temp;
		is_stopped=true;
		emit temp_set();
	} else {
		wait(TIMESTEP,SLOT(step1()));//continue cycle
	}
}

void QTempTimer::stop()
{
	is_stopped=true;
}

