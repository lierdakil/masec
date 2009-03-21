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
    return (queryInt("OPST?") & 2) == 2;
}

float magnctrl::getSetField()
{
    return queryFloat("SETF?");
}

float magnctrl::getSetCurrent()
{
    return queryFloat("SETI?");
}

float magnctrl::field()
{
    return queryFloat("RDGF?");
}

float magnctrl::current()
{
    return queryFloat("RDGI?");
}

bool magnctrl::isQuench()
{
    //TODO: count
    return (queryIntList("ERST?",',').at(1) & 32) == 32;
}

float magnctrl::flds()//in kG, yes?
{
    QVariantList data=queryVariantList("FLDS?",',',"int,float");
    //TODO: int units=data[0].toInt();
    //TODO: check units, yes?
    float value=data[1].toDouble();
    return value;
}

float magnctrl::rate()
{
    return queryFloat("RATE?");
}
