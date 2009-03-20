#ifndef CTRL_GEN_H_
#define CTRL_GEN_H_

#include <gpibctrl.h>

class genctrl: public GPIBctrl
{
public:
	genctrl(QString GPIBID);
	~genctrl();
	void setfreq(double freq);
	void sweepoff();
	void sweepon();
	void setsweep(float sf, float ff);
	void startsweep();
};

#endif /*CTRL_GEN_H_*/
