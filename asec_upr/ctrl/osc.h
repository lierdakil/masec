#ifndef CTRL_OSC_H_
#define CTRL_OSC_H_

#include <gpib.h>
#include <QString>
#include <QByteArray>

class oscctrl: public GPIBctrl
{
public:
	oscctrl(QString GPIBID) : GPIBctrl(GPIBID)
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
		write("CH1:VOL 0.1");
		write("CH1:POS 0");
		write("DAT INIT");
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
		read_array(13);//rubbish
		QByteArray data = read_array(2500);
		for (int i=0; i<data.count(); i++)
			data[i]=(data.at(i)>=0?data.at(i):-data.at(i));//abs
		return data;
	}

	void wait(QString state)
	{
		while ( query("TRIG:STATE?").split(" ").at(1) != state )
		{

		}
	}

	double setch1(unsigned char max)
	//TODO: COMMENT: explain what it does
	{
		double newv = max*0.002;
		write(QString("CH1:VOL %1").arg(newv));
		//51 div = newv V;
		//1 div = newv/51 V
		return newv/51;
	}
};

#endif /*CTRL_OSC_H_*/
