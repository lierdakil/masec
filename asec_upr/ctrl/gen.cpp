/*
 * gen.cpp
 *
 *  Created on: 01.12.2008
 *      Author: kazei
 */

#include "gen.h"

genctrl::genctrl(QString GPIBID) : GPIBctrl(GPIBID,"HEWLETT-PACKARD,33120A,0,10.0-5.0-1.0",15000)
{
	write("APPL:SIN 1, 5.0");
	write("SWE:TIME 10");
	write("SWE:SPAC LIN");
	write("TRIG:SOUR BUS");
}

genctrl::~genctrl()
{
	/**/
}

void genctrl::setfreq(double freq)
{
	write(QString("FREQ %1").arg(freq));
}

void genctrl::sweepoff()
{
	write("SWE:STAT OFF");
}

void genctrl::sweepon()
{
	write("SWE:STAT ON");
}

void genctrl::setsweep(float sf, float ff)
{
	write(QString("FREQ:STAR %1").arg(sf));
	write(QString("FREQ:STOP %1").arg(ff));
	write("SWE:STAT ON");
}

void genctrl::startsweep()
{
	write("*TRG");
	while(query("*OPC?")=="0")
	{}
}
