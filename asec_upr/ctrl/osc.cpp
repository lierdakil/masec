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
    read_array(13);//rubbish
    QByteArray data = read_array(2500);
    read();//This actually reads one char -- a separator.
    //Not shure why wasn't this needed with SICL, but with VISA it's required
    for (int i=0; i<data.count(); i++)
        data[i]=(data.at(i)>=0?data.at(i):-data.at(i));//abs
    return data;
}

void oscctrl::wait(QString state)
{
    while ( query("TRIG:STATE?").split(" ").at(1) != state ) //TODO: error handler
    {

    }
}


double oscctrl::getch1()
{
    bool success;
    QString reply=query("CH1:VOL?");
    double vol = reply.split(" ").at(1).toDouble(&success);//TODO: error handler
    if (success)
        return vol;
    else
        return -1;
}

void oscctrl::setch1(double vol)
{
    write(QString("CH1:VOL %1").arg(vol));
}
