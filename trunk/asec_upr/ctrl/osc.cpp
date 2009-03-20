/*
 * osc.cpp
 *
 *  Created on: 01.12.2008
 *      Author: kazei
 */
#include "osc.h"

oscctrl::oscctrl(QString GPIBID) : GPIBctrl(GPIBID,"TEKTRONIX,TDS 2012,0,CF:91.1CT FV:v2.12 TDS2CM:CMV:v1.04")
{
    write("ACQ:MOD PEAK");
    write("ACQ:STOPA RUNSTOP");
    write("TRIG:MAI:EDGE:SOU EXT");
    write("TRIG:MAI:EDGE:SLO FALL");
    write("TRIG:MAI:EDGE:COUP DC");
    write("TRIG:MAI:MOD NORM");
    write("TRIG:MAI:TYP EDGE");
    write("HOR:MAI:SEC 1");
    write("HOR:MAI:POS 5");
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

void oscctrl::wait(QString state)
{
    QStringList reply;
    do
    {
        //sleep?
    } while ( queryString("TRIG:STATE?",":TRIG:STATE") != state );
}


double oscctrl::getch1()
{
    return queryFloat("CH1:VOL?",":CH1:VOL");
}

void oscctrl::setch1(double vol)
{
    write(QString("CH1:VOL %1").arg(vol));
}
