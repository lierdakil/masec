#ifndef CTRL_VOL_H_
#define CTRL_VOL_H_

#include "gpib.h"
#include <QStringList>

class volctrl: public GPIBctrl
{
public:
	volctrl(QString GPIBID) : GPIBctrl(GPIBID)
	{
		write("*RST");
		write("SAMP:COUN 5");//TODO: Meybe change sample count?
		write("SYST:BEEP:STAT OFF");
		write("FUNC \"VOLT:AC\"");
		write("VOLT:AC:RANG:AUTO ON");
	}

	~volctrl()
	{

	}

	double acquire()
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
};

#endif /*CTRL_VOL_H_*/
