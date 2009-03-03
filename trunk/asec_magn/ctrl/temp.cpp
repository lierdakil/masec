/*
 * temp.cpp
 *
 *  Created on: 01.12.2008
 *      Author: kazei
 */

#include "temp.h"

tempctrl::tempctrl(QString GPIBID) : GPIBctrl(GPIBID)
{
	write("CSET 1, A, 1, 1, 2");
	write(QString("*SRE %1").arg(128));
}

tempctrl::~tempctrl()
{

}

void tempctrl::ctrlmode(int mode)
{
	if ((mode>=1)&&(mode<=6))
		write(QString("CMODE 1, %1").arg(mode));
}

int tempctrl::getctrlmode()
{
	return query("CMODE? 1").toInt();
}

float tempctrl::ramp(float value/*K/min*/)
{
	float result=value;
	if ((value>=0.1)&&(value<=100))
		write(QString("RAMP 1, 1, %1").arg(value));
	else
	{
		write(QString("RAMP 1, 0, 0.1"));
		result=0;
	}
	return result;//result==0 means result==INF;
}

void tempctrl::setzone(int zonenum, float toptemp, float P, float I, float D/*%*/, float mout, int range/*0-3*/)
{
	if ( (zonenum>=1)&&(zonenum<=10) && (P>=0.1)&&(P<=1000) && (I>=0.1)&&(I<=1000) && (D>=0)&&(D<=200) && (mout>=0)&&(mout<=100) && (range>=0)&&(range<=3) )
		write(QString("ZONE 1, %1, %2, %3, %4, %5, %6, %7").arg(zonenum).arg(toptemp).arg(P).arg(I).arg(D).arg(mout).arg(range));
}

void tempctrl::setpoint(float value)
{
	write(QString("SETP 1,%1").arg(value));
}

void tempctrl::setPID(float P, float I, float D)
{
	if ((P>=0.1)&&(P<=1000)&&(I>=0.1)&&(I<=1000)&&(D>=0)&&(D<=200))
		write(QString("PID 1, %1, %2, %3").arg(P).arg(I).arg(D));
}

float tempctrl::temp()/*Kelvin*/
{
	float temp;
	int status;
	bool tempok=true;
	bool statok=true;
	do {
		temp=query("KRDG? A").toFloat(&tempok);
		status=query("RDGST? A").toInt(&statok);
	} while(status!=0 || !tempok || !statok);
	return temp;
}

void tempctrl::readzone(int zonen, float* toptemp, float* P, float* I, float* D, float* mout, int* range)
{
	QStringList data=query(QString("ZONE? 1, %1").arg(zonen)).split(",");
	//iscanf(did,"%f,%f,%f,%f,%f,%d", toptemp, P, I, D, mout, range);
	*toptemp=data[0].toFloat();
	*P=data[1].toFloat();
	*I=data[2].toFloat();
	*D=data[3].toFloat();
	*mout=data[4].toFloat();
	*range=data[5].toInt();
}

float tempctrl::getramp()
{
	QStringList data=query("RAMP? 1").split(",");//enabled,value
	if (data[0].toInt()==1)
		return data[1].toFloat();
	else
		return 0;
}

void tempctrl::readPID(float* P, float* I, float* D)
{
	QStringList data = query("PID? 1").split(",");//P,I,D
	*P = data[0].toFloat();
	*I = data[1].toFloat();
	*D = data[2].toFloat();
}

void tempctrl::setrange(int range)
{
	write(QString("RANGE %1").arg(range));
}

void tempctrl::setmout(float mout)
{
	write(QString("MOUT 1,%1").arg(mout));
}

int tempctrl::getrange()
{
	return query("RANGE?").toInt();//Range
}

float tempctrl::getmout()
{
	return query("MOUT? 1").toFloat();//Manual Output
}

float tempctrl::getsetp()
{
	return query("SETP? 1").toFloat();//Setpoint
}

bool tempctrl::rampdone()
{
	return (query("*STB?").toInt() & 128)==128;
}

void tempctrl::rst()
{
	write("*RST");
}
