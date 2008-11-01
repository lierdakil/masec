#ifndef CTRL_TEMP_H_
#define CTRL_TEMP_H_

#include <ctrl_gpib.h>

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
		iprintf(did, "CSET 1, A, 1, 1, 2\n");
		iprintf(did,"*SRE %d\n", 128);
	}
	
	~tempctrl()
	{
		
	}
	
	void ctrlmode(char mode)
	{
		if ((mode>=1)&&(mode<=6))
			iprintf(did,"CMODE 1, %d\n",mode);
	}
	
	void ramp(float value/*K/min*/)
	{
		if ((value>=0.1)&&(value<=100))
			iprintf(did,"RAMP 1, 1, %f\n",value);
		else
			iprintf(did,"RAMP 1, 0, 0.1\n");
	}
	
	void setzone(char zonen, float toptemp, float P, float I, float D/*%*/, float mout=0, char range=3/*0-3*/)
	{
		if ( (zonen>=1)&&(zonen<=10) && (P>=0.1)&&(P<=1000) && (I>=0.1)&&(I<=1000) && (D>=0)&&(D<=200) && (mout>=0)&&(mout<=100) && (range>=0)&&(range<=3) )
			iprintf(did, "ZONE 1, %d, %f, %f, %f, %f, %f, %d\n", zonen, toptemp, P, I, D, mout, range);
	}
	
	void setpoint(float value)
	{
		iprintf(did,"SETP 1,%f\n",value);
	}
	
	void setPID(float P, float I, float D)
	{
		if ((P>=0.1)&&(P<=1000)&&(I>=0.1)&&(I<=1000)&&(D>=0)&&(D<=200))
			iprintf(did, "PID 1, %f, %f, %f\n",P,I,D);
	}
	
	float temp()/*Kelvin*/
	{
		float temp=-1;
		int status;
		do {
		ipromptf(did,"KRDG? A\n","%f\n",&temp);
		ipromptf(did,"RDGST? A\n","%d\n",&status);
		} while(status!=0 || temp<0);
		return temp;
	}
	
	void readzone(char zonen, float* toptemp, float* P, float* I, float* D, float* mout, char* range)
	{
		iprintf(did,"ZONE? 1, %d\n",zonen);
		iscanf(did,"%f,%f,%f,%f,%f,%d\n", toptemp, P, I, D, mout, range);
	}
	
	float getramp()
	{
		float ramp;
		char e;
		ipromptf(did,"RAMP? 1\n","%d,%f\n",&e,&ramp);
		if (e==1)
			return ramp;
		else
			return 0;
	}
	
	void readPID(float* P, float* I, float* D)
	{
		ipromptf(did,"PID? 1\n","%f,%f,%f\n",P,I,D);
	}
	
	void setrange(char range)
	{
		iprintf(did,"RANGE %d\n",range);
	}
	
	void setmout(float mout)
	{
		iprintf(did,"MOUT 1,%f\n",mout);
	}
	
	char getrange()
	{
		int res;
		ipromptf(did,"RANGE?\n","%d\n",&res);
		return res;
	}
	
	float getmout()
	{
		float res;
		ipromptf(did,"MOUT? 1\n","%f\n",&res);
		return res;	
	}
	
	float getsetp()
	{
		float res;
		ipromptf(did,"SETP? 1\n","%f\n",&res);
		return res;	
	}
	
	bool rampdone()
	{
		int res;
		ipromptf(did,"*STB?\n","%d\n",&res);
		return (res & 128)==128;
	}
};

#endif /*CTRL_TEMP_H_*/
