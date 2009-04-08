#ifndef CTRL_TEMP_H_
#define CTRL_TEMP_H_

#include <gpibctrl.h>
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
	void ctrlmode(int mode);
	int getctrlmode();
	float ramp(float value/*K/min*/);
	void setzone(int zonenum, float toptemp, float P, float I, float D/*%*/, float mout, int range/*0-3*/);
	void setpoint(float value);
	void setPID(float P, float I, float D);
	float temp();/*Kelvin*/
	void readzone(int zonen, float* toptemp, float* P, float* I, float* D, float* mout, int* range);
	float getramp();
	void readPID(float* P, float* I, float* D);
	void setrange(int range);
	void setmout(float mout);
	int getrange();
	float getmout();
	float getsetp();
	bool rampdone();
	void rst();
};

#endif /*CTRL_TEMP_H_*/
