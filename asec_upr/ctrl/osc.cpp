/*
 * osc.cpp
 *
 *  Created on: 01.12.2008
 *      Author: kazei
 */
#include "osc.h"
#include "measure_time.h"

oscctrl::oscctrl(QString GPIBID) : GPIBctrl(GPIBID,"TEKTRONIX,TDS 2012,0,CF:91.1CT FV:v2.12 TDS2CM:CMV:v1.04")
{
    write("ACQ:MOD PEAK");
    write("ACQ:STOPA RUNSTOP");
    write("TRIG:MAI:EDGE:SOU EXT5");
    write("TRIG:MAI:EDGE:SLO FALL");
    write("TRIG:MAI:EDGE:COUP DC");
    write("TRIG:MAI:MOD AUTO");
    write("TRIG:MAI:TYP EDGE");
    write("TRIG:MAI:LEV 1");
    write(QString("HOR:MAI:SEC %1").arg((double)MEASURE_TIME/10));
    write(QString("HOR:MAI:POS %1").arg((double)MEASURE_TIME/2));
    write("CH1:POS 0");
    write("DAT INIT");
}

oscctrl::~oscctrl()
{

}

QByteArray oscctrl::readcurve()
{
    write("DAT:STAR 1");
    write("DAT:STOP 2500");
    write("DAT:ENC SRI");
    write("CURV?");
    readArray(13);//rubbish
    QByteArray data = readArray(2500);
    readString();//clears the rest of the buffer
    //Not shure why wasn't this needed with SICL, but with VISA it's required
    for (int i=0; i<data.count(); i++)
        data[i]=(data.at(i)>=0?data.at(i):-data.at(i));//abs
    return data;
}

double oscctrl::ymul()
{
    return queryFloat("WFMP:YMU?",":WFMPRE:YMULT");
}

void oscctrl::wait(QString state)
{
    QStringList reply;
    do
    {
        //sleep?
    } while ( queryString("TRIG:STATE?",":TRIGGER:STATE") != state );
}


double oscctrl::getch1()
{
    return queryFloat("CH1:VOL?",":CH1:VOLTS");
}

void oscctrl::setch1(double vol)
{
    write(QString("CH1:VOL %1").arg(vol));
}

void oscctrl::trig_mode(Mode mode)
{
    switch(mode) {
        case Normal:
        write("TRIG:MAI:MOD NORM");
        break;

        case Auto:
        write("TRIG:MAI:MOD AUTO");
        break;
    }
}
