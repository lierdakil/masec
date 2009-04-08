/*
 * vol.cpp
 *
 *  Created on: 01.12.2008
 *      Author: kazei
 */

#include "vol.h"

volctrl::volctrl(QString GPIBID) : GPIBctrl(GPIBID,"HEWLETT-PACKARD,34401A,0,11-5-2",30000)
{
    write("*RST");
    write("SAMP:COUN 5");
    write("SYST:BEEP:STAT OFF");
    write("FUNC \"VOLT:AC\"");
    write("VOLT:AC:RANG:AUTO ON");
}

volctrl::~volctrl()
{

}

double volctrl::acquire()
{
    double res=0;
    QList<float> list = queryFloatList("READ?",',',5);
    for (int i=0;i<list.count();i++)
        res+=list[i];
    res/=list.count();
    return res;
}
