/*
 * vol.cpp
 *
 *  Created on: 01.12.2008
 *      Author: kazei
 */

#include "vol.h"

volctrl::volctrl(QString GPIBID) : GPIBctrl(GPIBID)
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
	QStringList list = query("READ?").split(",");
	for (int i=0;i<list.count();i++)
	{
		res+=list[i].toDouble();
	}
	res/=list.count();
	return res;
}
