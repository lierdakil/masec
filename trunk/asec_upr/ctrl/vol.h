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
		//iprintf(did,"CALC:FUNC AVER\n");
		write("SAMP:COUN 5");
		write("SYST:BEEP:STAT OFF");
		//iprintf(did,"CALC:STAT OFF\n");
		write("FUNC \"VOLT:AC\"");
		write("VOLT:AC:RANG:AUTO ON");
	}

	~volctrl()
	{

	}
	double acquire()
	{
		double res=0;
		//iprintf(did,"CALC:STAT ON\n");
		QStringList list = query("READ?").split(",");
		for (int i=0;i<list.count();i++)
		{
			res+=list[i].toDouble();
		}
		res/=list.count();
		//ipromptf(did,"CALC:AVER:AVER?\n","%e\n",&res);
		//iprintf(did,"CALC:STAT OFF\n");
		return res;
	}
};

#endif /*CTRL_VOL_H_*/
