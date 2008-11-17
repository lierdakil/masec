#ifndef CTRL_GEN_H_
#define CTRL_GEN_H_

#include "gpib.h"

class genctrl: public GPIBctrl
{
public:
	genctrl(QString GPIBID) : GPIBctrl(GPIBID)
	{
		write("APPL:SIN 1, 5.0");
		write("SWE:TIME 10");
		write("SWE:SPAC LIN");
		write("TRIG:SOUR BUS");
	}
	~genctrl()
	{
		/**/
	}
	void setfreq(double freq)
	{
		write(QString("FREQ %1").arg(freq));
	}
	void sweepoff()
	{
		write("SWE:STAT OFF");
	}
	void setsweep(float sf, float ff)
	{
		write(QString("FREQ:STAR %1").arg(sf));
		write(QString("FREQ:STOP %1").arg(ff));
		write("SWE:STAT ON");
	}
	void startsweep()
	{
		write("*TRG");
		while(query("*OPC?")=="0")
		{}
	}
};

#endif /*CTRL_GEN_H_*/
