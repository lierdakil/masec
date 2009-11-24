/*
 * measurethread.cpp
 *
 *  Created on: 18.11.2008
 *      Author: kazei
 */

#include "measurethread.h"
#include "sleep.h"

#define draw_x(x) emit line(x,0,x,-127,Qt::SolidLine)
#define draw_y(y) emit line(0,y,2500,y,Qt::SolidLine)

//#define GOLDEN
#define PHI 1.61803398874989484

class UserStoppedError
{
//does not do a thing! wohoo!
};

QList<qreal> sm_diff(QByteArray data, int nr)//nr - Depth of smoothing in one direction
{
    QList<qreal> diff;

    for (int i=0;i<data.count();i++)
    {
        if(i<nr || i>=data.count()-nr)
        {
            diff<<0;
            continue;
        }
        double a=0;
        double b=0;
        for (int k=-nr;k<=nr;k++)
        {
            a+=k*data[i+k]-k*data[i+k]/nr;
            b+=k*k;
        }

        diff<<a/b*10;
    }
    return diff;
}

inline QByteArray MeasureThread::dosweep(double startf, double stopf)
{
    gen->setsweep(startf,stopf);
    osc->trig_mode(oscctrl::Normal);
    osc->wait("READY");
    gen->startsweep();
    osc->wait("READY");
    QByteArray result = osc->readcurve();
    osc->trig_mode(oscctrl::Auto);
    if (stop_scheldued) throw UserStoppedError();
    return result;
}

QByteArray MeasureThread::sweep()
{
    osc->setch1(volts1);

    int mini=0;
    int maxi=0;
    int starti=0, stopi=0;

    QByteArray data;

    curve_forward_x.clear();
    curve_reverse_x.clear();
    curve_wide_x.clear();
    curve_forward_y.clear();
    curve_reverse_y.clear();
    curve_wide_y.clear();

    while(!(mini>maxi))
    {
        data=dosweep(fsf,fff);

        /*    max
        *     _
        *    / \
        *___/   |   _______
        *        \_/
        *        min
        *
        *   /\     _
        *---  \   /  -------
        *      \_/
        *    diff_min/
        *
        */

        //emit path(data,QPen(Qt::green));

        QList<qreal> diff = sm_diff(data,sm1);

        //emit path(diff,QPen(Qt::darkYellow));

        double min_diff_val=0;
        int min_diff_index=0;

        //find diff_min
        for (int i=0;i<diff.count();i++)
        {
            if (diff[i]<min_diff_val)
            {
                min_diff_val = diff[i];
                min_diff_index = i;
            }
        }

        //find diff=0 right from diff_min <=> min
        for (int i=min_diff_index;i<diff.count()-1;i++)
        {
            if (diff[i]<=0 && diff[i+1]>0)
            {
                mini = i;
                break;
            }
        }

        //find diff=0 left from diff_min <=> max
        for (int i=min_diff_index;i>0;i--)
        {
            if (diff[i-1]>=0 && diff[i]<0)
            {
                maxi = i;
                break;
            }
        }

        //starti=2*maxi-min_diff_index;//double interval
        //stopi=2*mini-min_diff_index;
        starti=2*maxi-mini;
        //draw_x(starti);
        stopi=2*mini-maxi;
        //draw_x(stopi);
    }
    //calculate coefficents to convert index to frequency
    kt = (fff-fsf)/data.count();
    sff = kt*(stopi)+fsf;
    ssf = kt*(starti)+fsf;
    k = (sff-ssf)/data.count();
    emit marker(ssf, 0, QPen(Qt::darkGreen), QwtPlotMarker::VLine);
    emit marker(sff, 0, QPen(Qt::darkGreen), QwtPlotMarker::VLine);

    int max_data=0;
    for(int i=0;i<data.count();i++)
        if(data.at(i)>max_data)
            max_data=data.at(i);

    //double curv=osc->getch1();
    //double onediv=curv*10.0/254;
    //double maxv=max_data*onediv;
    //double newv=maxv/4;
    //double ch1=osc->getch1();
    k2=osc->ymul();
    double ch1=max_data*k2/4;

    for(int i=0; i<data.count(); i++)
    {
        curve_wide_x<<kt*i+fsf;
        curve_wide_y<<data.at(i)*k2;
    }

    emit path(curve_wide_x,curve_wide_y,QPen(Qt::green));

    QByteArray data2_forward;
    QByteArray data2_reverse;
    int max_data2=0;
    do {
        osc->setch1(ch1);

        data2_forward=dosweep(ssf,sff);

        k2=osc->ymul();

        data2_reverse=dosweep(sff,ssf);

        max_data2=0;
        for(int i=0; i<data2_forward.count(); ++i)
            if(data2_forward.at(i)>max_data2)
                max_data2=data2_forward.at(i);
        for(int i=0; i<data2_reverse.count(); ++i)
            if(data2_reverse.at(i)>max_data2)
                max_data2=data2_reverse.at(i);
        if(max_data2>=127)
            ch1=ch1*PHI;
    } while(max_data2>=127);
    QByteArray data2_reverse2;
    for(int i=1; i<=data2_reverse.count();++i)
        data2_reverse2.append(data2_reverse.at(data2_reverse.count()-i));

    /*emit path(data2_forward,QPen(Qt::magenta));
    emit path(data2_reverse2,QPen(Qt::blue));*/

    for(int i=0; i<data2_forward.count(); i++)
    {
        curve_forward_x<<k*i+ssf;
        curve_forward_y<<data2_forward.at(i)*k2;
    }
    emit path(curve_forward_x, curve_forward_y,QPen(Qt::blue));

    for(int i=0; i<data2_reverse2.count(); i++)
    {
        curve_reverse_x<<k*i+ssf;
        curve_reverse_y<<data2_reverse2.at(i)*k2;
    }
    emit path(curve_reverse_x, curve_reverse_y,QPen(Qt::magenta));

    return data2_reverse2;
}

float MeasureThread::getamplonf(float freq)
{
    gen->setfreq(freq);
    return vol->acquire();
}

float MeasureThread::golden(float a, float b, float epsilon, bool max)
{
    const double phi = PHI;

    float x1=b-(b-a)/phi;
    float x2=a+(b-a)/phi;

    float y1=getamplonf(x1);
    float y2=getamplonf(x2);


    while ((b-a)>=epsilon*2)
    {
        if ( (y1>y2 && max) || (y1<y2 && !max))
        {
            b=x2;
            x2=x1;
            y2=y1;
            x1=a+b-x2;
            y1=getamplonf(x1);
        }
        else
        {
            a=x1;
            x1=x2;
            y1=y2;
            x2=b-(x1-a);
            y2=getamplonf(x2);
        }
    }

    return (a+b)/2;
}

double MeasureThread::find_extremum(QByteArray dat, int start, int stop, int sm, bool max)
{
#ifdef GOLDEN
    return golden(k*start+ssf,k*stop+ssf,epsilon,true);
#else
    QList<qreal> diff=sm_diff(dat,sm);
    int a=0,b=0;
    //left to right
    for(int i=start;i<stop;i++)
    {
        if((max && (diff[i]<=0 && diff[i-1]>=0)) || (!max && (diff[i]>=0 && diff[i-1]<=0)))
        {
            a=i;
            break;
        }
    }
    //right to left
    for(int i=stop;i>start;i--)
    {
        if((max && (diff[i]>=0 && diff[i+1]<=0)) || (!max && (diff[i]<=0 && diff[i+1]>=0)))
        {
            b=i;
            break;
        }
    }

    return k*(a+b)/2.0f+ssf;
#endif
}

void MeasureThread::findresonance()
{
    int xmin=0,xmax1=0,xfmax=0,xmax2=0;

    QByteArray dat;
    QList<qreal> diff;

    int cycles=0;
    while ((xfmax <= xmax1)||(xfmax >= xmin))
    {
        //draw X axis
        if (stop_scheldued) throw UserStoppedError();

        dat = sweep();

        //emit line(0,0,dat.count(),0,Qt::SolidLine);

        int fmax=0;
        for (int i=0; i<dat.count();i++)
        {
            if (fmax<dat[i])
            {
                fmax=dat[i];
                xfmax=i;
            }
        }
        if (stop_scheldued) throw UserStoppedError();

        diff=sm_diff(dat,sm2);
        if (stop_scheldued) throw UserStoppedError();

        //emit path(diff, QPen(Qt::red));
        if (stop_scheldued) throw UserStoppedError();

        float min=255;
        float max1=0;
        float max2=0;

        for (int i=0;i<dat.count();i++)
        {
            if (min>diff[i])
            {
                min=diff[i];
                xmin=i;
            }
        }
        if (stop_scheldued) throw UserStoppedError();

        for (int i=0;i<xmin;i++)
        {
            if (max1<diff[i])
            {
                max1=diff[i];
                xmax1=i;
            }
        }
        if (stop_scheldued) throw UserStoppedError();

        for (int i=xmin;i<dat.count();i++)
        {
            if (max2<diff[i])
            {
                max2=diff[i];
                xmax2=i;
            }
        }
        if (stop_scheldued) throw UserStoppedError();

        //draw_x(xmin);
        //draw_x(xmax1);
        //draw_x(xmax2);
        emit marker(xmin*k+ssf, 0, QPen(Qt::red), QwtPlotMarker::VLine);
        emit marker(xmax1*k+ssf, 0, QPen(Qt::red), QwtPlotMarker::VLine);
        emit marker(xmax2*k+ssf, 0, QPen(Qt::red), QwtPlotMarker::VLine);
        cycles++;
        if(cycles>4)
            throw GPIBGenericException(trUtf8("Could not acquire resonance qurve"));
    }

    gen->sweepoff();

    rf=find_extremum(dat,xmax1,xmin,sm2,true);
    //draw_x((rf-ssf)/k);
    //ra=curve_forward_y.at((rf-ssf)/k);
    ra=getamplonf(rf)*sqrt(2);
    //draw_y(-ra/k2*sqrt(2));
    emit marker(rf, ra, QPen(Qt::black), QwtPlotMarker::Cross);

    af=find_extremum(dat,xmin,xmax2,sm2,false);
    //draw_x((af-ssf)/k);
    //aa=ra=curve_forward_y.at((af-ssf)/k);
    aa=getamplonf(af)*sqrt(2);
    //draw_y(-aa/k2*sqrt(2));
    emit marker(af, aa, QPen(Qt::black), QwtPlotMarker::Cross);

    gen->sweepon();
}



void MeasureThread::run()
{
    stop_scheldued=false;
    epsilon=0.1;
    fsf=startf;
    fff=stopf;

    gen=0;
    vol=0;
    osc=0;

    try{
        //---------Main block-------------
        if (oscid.isEmpty() || genid.isEmpty() || mulid.isEmpty())
        {
            if(oscid.isEmpty())
                throw GPIBGenericException(trUtf8("Oscilloscope GPIB ID is empty"));
            if(genid.isEmpty())
                throw GPIBGenericException(trUtf8("Generator GPIB ID is empty"));
            if(mulid.isEmpty())
                throw GPIBGenericException(trUtf8("Multimeter GPIB ID is empty"));
            return;
        }

        gen = new genctrl(genid);
        vol = new volctrl(mulid);
        osc = new oscctrl(oscid);
        gen->setvol(genvolpp);

        findresonance();

        QStringList data;
        data<<QString("Generator amplitude, VPP:%1").arg(genvolpp);
        data<<QString("First run start freq, Hz:%1").arg(fsf);
        data<<QString("First run stop freq, Hz:%1").arg(fff);
        data<<QString("Second run start freq, Hz:%1").arg(ssf);
        data<<QString("Second run stop freq, Hz:%1").arg(sff);
        data<<QString("Resonance freq, Hz:%1").arg(rf);
        data<<QString("Resonance ampl, V:%1").arg(ra);
        data<<QString("Antiresonance freq, Hz:%1").arg(af);
        data<<QString("Antiresonance ampl, V:%1").arg(aa);

        if(!filename.isEmpty())
        {
            QFile f(filename+"_forward.txt");
            f.open(QFile::WriteOnly | QIODevice::Text);
            foreach(QString s, data)
            {
                f.write("#");
                f.write(s.toAscii());
                f.write("\n");
            }
            f.write(QString("Frequency, Hz").toAscii());
            f.write("\t");
            f.write(QString("Amplitude, V").toAscii());
            f.write("\n");
            for(int i=0;i<curve_forward_x.count();i++)
            {
                f.write(QString::number(curve_forward_x.at(i),'f',10).toAscii());
                f.write("\t");
                f.write(QString::number(curve_forward_y.at(i),'f',10).toAscii());
                f.write("\n");
            }
            f.close();
        }

        if(!filename.isEmpty())
        {
            QFile f(filename+"_reverse.txt");
            f.open(QFile::WriteOnly | QIODevice::Text);
            foreach(QString s, data)
            {
                f.write("#");
                f.write(s.toAscii());
                f.write("\n");
            }
            f.write(QString("Frequency, Hz").toAscii());
            f.write("\t");
            f.write(QString("Amplitude, V").toAscii());
            f.write("\n");
            for(int i=0;i<curve_reverse_x.count();i++)
            {
                f.write(QString::number(curve_reverse_x.at(i),'f',10).toAscii());
                f.write("\t");
                f.write(QString::number(curve_reverse_y.at(i),'f',10).toAscii());
                f.write("\n");
            }
            f.close();
        }

        if(!filename.isEmpty())
        {
            QFile f(filename+"_wide.txt");
            f.open(QFile::WriteOnly | QIODevice::Text);
            foreach(QString s, data)
            {
                f.write("#");
                f.write(s.toAscii());
                f.write("\n");
            }
            f.write(QString("Frequency, Hz").toAscii());
            f.write("\t");
            f.write(QString("Amplitude, V").toAscii());
            f.write("\n");
            for(int i=0;i<curve_wide_x.count();i++)
            {
                f.write(QString::number(curve_wide_x.at(i),'f',10).toAscii());
                f.write("\t");
                f.write(QString::number(curve_wide_y.at(i),'f',10).toAscii());
                f.write("\n");
            }
            f.close();
        }

        emit finished(data);
        //------END main block-------------------
    } catch (GPIBGenericException e) {
        QStringList data;
        data<<QString("::ERROR::");
        data<<e.report();
        emit finished(data);
    } catch (UserStoppedError e) {
        //do nothing
    } catch(std::exception e) {
        QStringList data;
        data<<QString("::ERROR::");
        data<<trUtf8("Caught unexpected exception %1").arg(e.what());
        emit finished(data);
    }

    delete gen;
    gen=0;
    delete vol;
    vol=0;
    delete osc;
    osc=0;
}

void MeasureThread::stop()
{
    stop_scheldued=true;
}
