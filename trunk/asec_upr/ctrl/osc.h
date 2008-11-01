#ifndef CTRL_OSC_H_
#define CTRL_OSC_H_

#include "gpib.h"

class oscctrl: public GPIBctrl
{
public:
	oscctrl(QString GPIBID) : GPIBctrl(GPIBID)
	{
		/*--------------------Oscilloscope initialization
		ACQuire:MODe PEAKdetect
		ACQ:STOPAfter SEQuence
		DATa INIT
		//HORizontal //?
		//----------end initializaton*/
		write("ACQ:MOD PEAK");
		write("ACQ:STOPA RUNSTOP");
		write("TRIG:MAI:EDGE:SOU EXT");
		write("TRIG:MAI:EDGE:SLO FALL");
		write("TRIG:MAI:EDGE:COUP DC");
		write("TRIG:MAI:MOD NORM");
		write("TRIG:MAI:TYP EDGE");
		write("HOR:MAI:SEC 1");
		write("HOR:MAI:POS 5");
		write("CH1:VOL 0.1");
		write("CH1:POS 0");
		write("DAT INIT");
		//TODO: Try to unite in one request
	}

	~oscctrl()
	{

	}

	QByteArray readcurve()
	{
		write("DAT:STAR 1");
		write("DAT:STOP 2500");
		write("DAT:ENC SRI");
		write("CURV?");
		int len=13;
		QByteArray rubbish=read_array(&len);
		len=2500;
		QByteArray data = read_array(&len);
		for (int i=0; i<len; i++)
			data[i]=( data.at(i)>=0?data.at(i):-data.at(i));//abs
		return data;
	}

	void wait(QString state)
	{
		while ( !query("TRIG:STATE?").contains(state,Qt::CaseInsensitive) )
		{
			//Sleep(10);
		}
	}

	double setch1(unsigned char max)//TODO: Razobrat'sa, 4to ono delaet
	{
		double newv = max*0.001;
		query(QString("CH1:VOL %1").arg(newv));
		//51 div = newv V;
		//1 div = newv/51 V
		return newv/51;
	}
};

#endif /*CTRL_OSC_H_*/
