/*
 * QMagnTimeline.cpp
 *
 *  Created on: 03.03.2009
 *      Author: kazei
 */

#include "QMagnTimeline.h"
#include <time.h>
#define TIMESTEP 0.5

QMagnTimeline::QMagnTimeline() {

}

QMagnTimeline::~QMagnTimeline() {

}

void QMagnTimeline::wait(double sec, const char* member)
{
	time=(clock()-startclock)/1000.0f;
	QTimer::singleShot(int(1000*min),this,member);
}

void QMagnTimeline::start(float field /*kG*/)
{
	magnctrl *magn=(magnctrl*)(qApp->property("magn").toInt());

	float current_old = magn->getSetCurrent(); //Amper
	float ramp_rate = magn->rate(); //Amper per second
	float flds = magn->flds(); //kG per Amper
	float current_new = field/flds; //kG / kG * A = Amper
	int ramp_time = fabs(current_old-current_new)/ramp_rate;// A/A*s = seconds

	time=0;
    drawtime=0;
    QTimer::singleShot(int(1000*TIMESTEP),this,SLOT(draw_temp()));
    QTimer::singleShot(int(1000*TIMESTEP),this,SLOT(check_quench()));

	startclock=clock();

	magn->setField(field);
	wait(ramp_time,SLOT(rampdone()));
}

void QMagnTimeline::draw_field()
{
	magnctrl *magn=(magnctrl*)(qApp->property("magn").toInt());
	drawtime=(clock()-startclock)/1000.0f;
	emit newpoint(drawtime,magn->field);
	if(!is_stopped)
		QTimer::singleShot(int(1000*TIMESTEP),this,SLOT(draw_field()));
}

void QMagnTimeline::check_quench()
{
	magnctrl *magn=(magnctrl*)(qApp->property("magn").toInt());
	if (magn->isQuench())
	{
		is_stopped=true;
		emit quench();
	} else if (!is_stopped) {
		QTimer::singleShot(int(1000*TIMESTEP),this,SLOT(check_quench()));
	}
}

void QMagnTimeline::rampdone()
{
	magnctrl *magn=(magnctrl*)(qApp->property("magn").toInt());
	if (!is_stopped)
	{
		if (magn->isRampDone())
			wait(TIMESTEP,SLOT(checkcurr()));
		else
			wait(TIMESTEP,SLOT(rampdone()));
	} else {
		emit stopped();
	}
}

void QMagnTimeline::checkcurr()
{
	magnctrl *magn=(magnctrl*)(qApp->property("magn").toInt());

	if(is_stopped)
	{
		emit stopped();
	} else if(fabs(magn->current()-magn->getSetCurrent())<=0.001) {//TODO: What is epsilon?
		is_stopped=true;
		emit field_set(magn->field());
	} else {
		wait(TIMESTEP,SLOT(checkcurr()));
	}
}

void QMagnTimeline::stop()
{
	is_stopped=true;
}
