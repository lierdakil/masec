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
	tempctrl(QString GPIBID);
	~tempctrl();
	void ctrlmode(char mode);
	float ramp(float value/*K/min*/);
	void setzone(char zonenum, float toptemp, float P, float I, float D/*%*/, float mout, char range/*0-3*/);
	void setpoint(float value);
	void setPID(float P, float I, float D);
	float temp();/*Kelvin*/
	void readzone(char zonen, float* toptemp, float* P, float* I, float* D, float* mout, int* range);
	float getramp();
	void readPID(float* P, float* I, float* D);
	void setrange(char range);
	void setmout(float mout);
	char getrange();
	float getmout();
	float getsetp();
	bool rampdone();
};

//TODO: many error handlers

#endif /*CTRL_TEMP_H_*/
