#ifndef CTRL_TEMP_H_
#define CTRL_TEMP_H_

#include <gpib.h>
#include <QString>
#include <QStringList>

#define MOD_MANUAL 1
#define MOD_ZONE 2
#define MOD_OPEN 3
#define MOD_AUTO_PID 4
#define MOD_AUTO_PI 5
#define MOD_AUTO_P 6

class tempctrl: public GPIBctrl
{
public:
	tempctrl(char* GPIBID) : GPIBctrl(GPIBID)
	{
                write("CSET 1, A, 1, 1, 2");
                write(QString("*SRE %1").arg(128));
	}
	
	~tempctrl()
	{
		
	}
	
	void ctrlmode(char mode)
	{
		if ((mode>=1)&&(mode<=6))
                        write(QString("CMODE 1, %1").arg(mode));
	}
	
	void ramp(float value/*K/min*/)
	{
		if ((value>=0.1)&&(value<=100))
                        write(QString("RAMP 1, 1, %1").arg(value));
		else
                        write(QString("RAMP 1, 0, 0.1"));
	}
	
        void setzone(char zonenum, float toptemp, float P, float I, float D/*%*/, float mout=0, char range=3/*0-3*/)
	{
                if ( (zonenum>=1)&&(zonenum<=10) && (P>=0.1)&&(P<=1000) && (I>=0.1)&&(I<=1000) && (D>=0)&&(D<=200) && (mout>=0)&&(mout<=100) && (range>=0)&&(range<=3) )
                        write(QString("ZONE 1, %1, %2, %3, %4, %5, %6, %7").arg((int)zonenum).arg(toptemp).arg(P).arg(I).arg(D).arg(mout).arg((int)range));
	}
	
	void setpoint(float value)
	{
                write(QString("SETP 1,%1").arg(value));
	}
	
	void setPID(float P, float I, float D)
	{
		if ((P>=0.1)&&(P<=1000)&&(I>=0.1)&&(I<=1000)&&(D>=0)&&(D<=200))
                        write(QString("PID 1, %f, %f, %f").arg(P).arg(I).arg(D));
	}
	
	float temp()/*Kelvin*/
	{
		float temp=-1;
		int status;
		do {
                    temp=query("KRDG? A").toFloat();
                    status=query("RDGST? A\n").toInt();
                } while(status!=0 || temp<0);
                //TODO: Better make an error handler?
		return temp;
	}
	
	void readzone(char zonen, float* toptemp, float* P, float* I, float* D, float* mout, char* range)
	{
                QStringList data=query(QString("ZONE? 1, %1").arg(zonen)).split(",");
                //iscanf(did,"%f,%f,%f,%f,%f,%d\n", toptemp, P, I, D, mout, range);
                *toptemp=data[0].toFloat();
                *P=data[1].toFloat();
                *I=data[2].toFloat();
                *D=data[3].toFloat();
                *mout=data[4].toFloat();
                *range=data[5].toInt();
	}
	
	float getramp()
	{
                QStringList data=query("RAMP? 1").split(",");//enabled,value
                if (data[0].toInt()==1)
                        return data[1].toFloat();
		else
			return 0;
	}
	
	void readPID(float* P, float* I, float* D)
	{
                QStringList data = query("PID? 1\n").split(",");//P,I,D
                *P = data[0].toFloat();
                *I = data[1].toFloat();
                *D = data[2].toFloat();
	}
	
	void setrange(char range)
	{
                write(QString("RANGE %1").arg(range));
	}
	
	void setmout(float mout)
	{
                write(QString("MOUT 1,%1").arg(mout));
	}
	
	char getrange()
	{
                return query("RANGE?").toInt();//Range
	}
	
	float getmout()
	{
                return query("MOUT? 1").toFloat();//Manual Output
	}
	
	float getsetp()
	{
                return query("SETP? 1").toFloat();//Setpoint
	}
	
	bool rampdone()
	{
                return (query("*STB?\n").toInt() & 128)==128;
	}
};

//TODO: many error handlers
//TODO: maybe make overloaded write()s?

#endif /*CTRL_TEMP_H_*/