/*
 * magn.h
 *
 *  Created on: 03.03.2009
 *      Author: kazei
 */

#ifndef MAGN_H_
#define MAGN_H_

#include <gpib.h>
#include <QString>
#include <QStringList>

class magnctrl : GPIBctrl
{
public:
	magnctrl(QString GPIBID);
	~magnctrl();

	void setField(float value);
	bool isRampDone();
	float getSetField();
	float getSetCurrent();
	float field();
	float current();
	bool isQuench();
	//TODO: check for all status registers, e.g. make a QString status()
	float flds();//in kG, yes?
	float rate();// ramp rate
};

#endif /* MAGN_H_ */
