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
    magn=0;
}

QMagnTimeline::~QMagnTimeline() {

}

void QMagnTimeline::wait(double sec, const char* member)
{
    time=(clock()-startclock)/1000.0f;
    QTimer::singleShot(int(1000*sec),this,member);
}

void QMagnTimeline::raiseError(QString message)
{
    is_running=false;
    delete magn;
    magn=0;
    emit error(message);
}

void QMagnTimeline::start(float field /*kG*/)
{
    try{
        magn=new magnctrl(qApp->property("magnid").toString());

        float current_old = magn->getSetCurrent(); //Amper
        float ramp_rate = magn->rate(); //Amper per second
        float flds = magn->flds(); //kG per Amper
        float current_new = field/1000/flds; //kG / kG * A = Amper
        double ramp_time = fabs(current_old-current_new)/ramp_rate;// A/A*s = seconds

        time=0;
        drawtime=0;
        is_running=true;

        QTimer::singleShot(int(1000*TIMESTEP),this,SLOT(draw_field()));
        QTimer::singleShot(int(1000*TIMESTEP),this,SLOT(check_quench()));

        startclock=clock();

        magn->setField(field);
        wait(ramp_time,SLOT(rampdone()));
    }catch(GPIBGenericException e){
        raiseError(e.report());
    }catch(std::exception e) {
        raiseError(QString("Caught unexpected exception '%1'").arg(e.what()));
    }
}

void QMagnTimeline::draw_field()
{
    if(!is_running)
        return;

    try {
        drawtime=(clock()-startclock)/1000.0f;
        emit newpoint(drawtime,magn->field());
        QTimer::singleShot(int(1000*TIMESTEP),this,SLOT(draw_field()));
    }catch(GPIBGenericException e){
        raiseError(e.report());
    }catch(std::exception e) {
        raiseError(QString("Caught unexpected exception '%1'").arg(e.what()));
    }
}

void QMagnTimeline::check_quench()
{
    if(!is_running)
        return;

    try {
        if (magn->isQuench())
        {
            is_running=false;
            delete magn;
            magn=0;
            emit quench();
        } else {
            QTimer::singleShot(int(1000*TIMESTEP),this,SLOT(check_quench()));
        }
    }catch(GPIBGenericException e){
        raiseError(e.report());
    }catch(std::exception e) {
        raiseError(QString("Caught unexpected exception '%1'").arg(e.what()));
    }
}

void QMagnTimeline::rampdone()
{
    if (!is_running)
        return;

    try {
        if (magn->isRampDone())
            wait(TIMESTEP,SLOT(checkcurr()));
        else
            wait(TIMESTEP,SLOT(rampdone()));
    }catch(GPIBGenericException e){
        raiseError(e.report());
    }catch(std::exception e) {
        raiseError(QString("Caught unexpected exception '%1'").arg(e.what()));
    }
}

void QMagnTimeline::checkcurr()
{
    if(!is_running)
        return;

    try {
        if(stop_requested)
        {
            is_running=false;
            delete magn;
            magn=0;
            emit stopped();
        } else if(fabs(magn->current()-magn->getSetCurrent())<=1) {//TODO: What is epsilon?
            emit field_set(magn->getSetField(),magn->field(),(clock()-startclock)/60000.0f);
            is_running=false;
            delete magn;
            magn=0;
        } else {
            wait(TIMESTEP,SLOT(checkcurr()));
        }
    }catch(GPIBGenericException e){
        raiseError(e.report());
    }catch(std::exception e) {
        raiseError(QString("Caught unexpected exception '%1'").arg(e.what()));
    }
}

void QMagnTimeline::stop()
{
    stop_requested=true;
}
