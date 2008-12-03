/*
 * measurethread.cpp
 *
 *  Created on: 18.11.2008
 *      Author: kazei
 */

#include "measurethread.h"

void MeasureThread::run()
{
    if (oscid.isEmpty() || genid.isEmpty() || mulid.isEmpty())
    {
        if(oscid.isEmpty())
            qWarning()<<"Oscilloscope GPIB ID is empty";
        if(genid.isEmpty())
            qWarning()<<"Generator GPIB ID is empty";
        if(mulid.isEmpty())
            qWarning()<<"Multimeter GPIB ID is empty";
        return;
    }

    cmeasure mes(oscid,genid,mulid,startf,stopf,0.1,volts1,sm1,sm2,parent());

    if(!filename.isEmpty())
    {
        QFile f(filename);
        f.open(QFile::WriteOnly);
        QString buf;
        for(int i=0;i<mes.curve.count();i++)
        {
            buf.setNum(mes.curve.at(i).first,'f',10);
            f.write(buf.toAscii());
            f.write("\t");
            buf.setNum(mes.curve.at(i).second,'f',10);
            f.write(buf.toAscii());
            f.write("\n");
        }
        f.close();
    }

    QStringList data;
    data<<QString("First run start freq, Hz:%1").arg(mes.fsf);
    data<<QString("First run stop freq, Hz:%1").arg(mes.fff);
    data<<QString("Second run start freq, Hz:%1").arg(mes.ssf);
    data<<QString("Second run stop freq, Hz:%1").arg(mes.sff);
    data<<QString("Resonance freq, Hz:%1").arg(mes.rf);
    data<<QString("Resonance ampl, V:%1").arg(mes.ra);
    data<<QString("Antiresonance freq, Hz:%1").arg(mes.af);
    data<<QString("Antiresonance ampl, V:%1").arg(mes.aa);
    emit finished(data);
}
