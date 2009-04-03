/*
 * temp.cpp
 *
 *  Created on: 01.12.2008
 *      Author: kazei
 */

#include "temp.h"
#define LOOP 'B'

tempctrl::tempctrl(QString GPIBID) : GPIBctrl(GPIBID,"LSCI,MODEL331S,333585,061404", 10000)
{
    write(QString("CSET 1, %1, 1, 1, 2").arg(LOOP));
    write(QString("*SRE %1").arg(128));
}

tempctrl::~tempctrl()
{

}

void tempctrl::ctrlmode(int mode)
{
    if ((mode>=1)&&(mode<=6))
        write(QString("CMODE 1, %1").arg(mode));
    else
        throw GPIBUnxParamValException("tempctrl::ctrlmode(mode)", QString("mode=%1").arg(mode), "1 <= mode <= 6");
}

int tempctrl::getctrlmode()
{
    return queryInt("CMODE? 1");
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
    if(!( (zonenum>=1)&&(zonenum<=10)))
        throw GPIBUnxParamValException("tempctrl::setzone(...)", QString("zonenum=%1").arg(zonenum), "1 <= zonenum <= 10");
    else if(!( (P>=0.1)&&(P<=1000) ))
        throw GPIBUnxParamValException("tempctrl::setzone(...)", QString("P=%1").arg(P), "0.1 <= P <= 1000");
    else if(!( (I>=0.1)&&(I<=1000) ))
        throw GPIBUnxParamValException("tempctrl::setzone(...)", QString("I=%1").arg(I), "0.1 <= I <= 1000");
    else if(!( (D>=0)&&(D<=200) ))
        throw GPIBUnxParamValException("tempctrl::setzone(...)", QString("D=%1").arg(D), "0 <= P <= 200");
    else if(!( (mout>=0)&&(mout<=100) ))
        throw GPIBUnxParamValException("tempctrl::setzone(...)", QString("mout=%1").arg(mout), "0 <= P <= 100");
    else if(!( (range>=0)&&(range<=3) ))
        throw GPIBUnxParamValException("tempctrl::setzone(...)", QString("range=%1").arg(range), "range=[0,1,2,3]");
    else
        write(QString("ZONE 1, %1, %2, %3, %4, %5, %6, %7").arg(zonenum).arg(toptemp).arg(P).arg(I).arg(D).arg(mout).arg(range));
}

void tempctrl::setpoint(float value)
{
    write(QString("SETP 1,%1").arg(value));
}

void tempctrl::setPID(float P, float I, float D)
{
    if(!( (P>=0.1)&&(P<=1000) ))
        throw GPIBUnxParamValException("tempctrl::setPID(...)", QString("P=%1").arg(P), "0.1 <= P <= 1000");
    else if(!( (I>=0.1)&&(I<=1000) ))
        throw GPIBUnxParamValException("tempctrl::setPID(...)", QString("I=%1").arg(I), "0.1 <= I <= 1000");
    else if(!( (D>=0)&&(D<=200) ))
        throw GPIBUnxParamValException("tempctrl::setPID(...)", QString("D=%1").arg(D), "0 <= P <= 200");
    else
        write(QString("PID 1, %1, %2, %3").arg(P).arg(I).arg(D));
}

float tempctrl::temp()/*Kelvin*/
{
    return queryFloat(QString("KRDG? %1").arg(LOOP));
    // in case this dirty hack is still needed....
    //    float temp;
    //    int status;
    //    bool tempok=true;
    //    bool statok=true;
    //    do {
    //        temp=query("KRDG? A").toFloat(&tempok);
    //        status=query("RDGST? A").toInt(&statok);
    //    } while(status!=0 || !tempok || !statok);
    //    return temp;
}

void tempctrl::readzone(int zonen, float* toptemp, float* P, float* I, float* D, float* mout, int* range)
{
    QVariantList data=queryVariantList(QString("ZONE? 1, %1").arg(zonen),',',"float,float,float,float,float,int");

    *toptemp=data[0].toDouble();
    *P=data[1].toDouble();
    *I=data[2].toDouble();
    *D=data[3].toDouble();
    *mout=data[4].toDouble();
    *range=data[5].toInt();
}

float tempctrl::getramp()
{
    QVariantList data=queryVariantList("RAMP? 1",',',"int,float");//enabled,value

    if (data.at(0)==1)
        return data.at(1).toDouble();
    else
        return 0;
}

void tempctrl::readPID(float* P, float* I, float* D)
{
    QList<float> data = queryFloatList("PID? 1",',',3);//P,I,D

    *P = data.at(0);
    *I = data.at(1);
    *D = data.at(2);
}

void tempctrl::setrange(int range)
{
    if(!( (range>=0)&&(range<=3) ))
        throw GPIBUnxParamValException("tempctrl::setrange(range)", QString("range=%1").arg(range), "range=[0,1,2,3]");
    else
        write(QString("RANGE %1").arg(range));
}

void tempctrl::setmout(float mout)
{
    if(!( (mout>=0)&&(mout<=100) ))
        throw GPIBUnxParamValException("tempctrl::setmout(...)", QString("mout=%1").arg(mout), "0 <= P <= 100");
    else
        write(QString("MOUT 1,%1").arg(mout));
}

int tempctrl::getrange()
{
    return queryInt("RANGE?");//Range
}

float tempctrl::getmout()
{
    return queryFloat("MOUT? 1");//Manual Output
}

float tempctrl::getsetp()
{
    return queryFloat("SETP? 1");//Setpoint
}

bool tempctrl::rampdone()
{
    return (queryInt("*STB?") & 128)==128;
}

void tempctrl::rst()
{
    write("*RST");
}
