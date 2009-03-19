/*
 * magn.cpp
 *
 *  Created on: 03.03.2009
 *      Author: kazei
 */

#include "magn.h"

magnctrl::magnctrl(QString GPIBID) : GPIBctrl(GPIBID,"LSCI,MODEL625,6251143,1.1/1.0")
{

}

magnctrl::~magnctrl()
{

}

void magnctrl::setField(float value)
{
	write(QString("SETF %1").arg(value));
}

bool magnctrl::isRampDone()
{
	return (query("OPST?").toInt() & 2) == 2;
}

float magnctrl::getSetField()
{
	return query("SETF?").toFloat();
}

float magnctrl::getSetCurrent()
{
	return query("SETI?").toFloat();
}

float magnctrl::field()
{
	return query("RDGI?").toFloat();
}

float magnctrl::current()
{
	return query("RDGI?").toFloat();
}

bool magnctrl::isQuench()
{
	return (query("ERST?").split(",").at(1).toInt() & 32) == 32;
}

float magnctrl::flds()//in kG, yes?
{
	QStringList data=query("FLDS?").split(",");
        //TODO: int units=data[0].toInt();
	//TODO: check units, yes?
	float value=data[1].toFloat();
	return value;
}

float magnctrl::rate()
{
	return query("RATE?").toFloat();
}
