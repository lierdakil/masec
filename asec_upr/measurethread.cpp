/*
 * measurethread.cpp
 *
 *  Created on: 18.11.2008
 *      Author: kazei
 */

#include "measurethread.h"
#include "sleep.h"
#include <math.h>

#define draw_x(x,pen) emit line(x,0,x,-1,pen)
#define draw_y(y,pen) emit line(0,y,2500,y,pen)

//#define GOLDEN

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

QByteArray MeasureThread::sweep()
{
    osc->setch1(volts1);

    int mini=0;
    int maxi=0;
    int starti, stopi;

    QByteArray data;

    while(!(mini>maxi))
    {
        gen->setsweep(fsf,fff);
        osc->wait("READY");
        gen->startsweep();
        osc->wait("READY");
        data = osc->readcurve();

        /*		    max
 *		     _
 *		    / \
 *		___/   |   _______
 *		        \_/
 *		        min
 *
 *         /\     _
 *		---  \   / -------
 *            \_/
 *		    diff_min/
 *
 */

        //emit path(data,QPen(Qt::green));

        QList<qreal> diff = sm_diff(data,sm1);

        //emit path(diff,QPen(Qt::darkYellow));

        double min_diff_val=0;
        int min_diff_index;

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

        //		starti=2*maxi-min_diff_index;//double interval
        //		stopi=2*mini-min_diff_index;
        starti=2*maxi-mini;
        //draw_x(starti,QPen(Qt::magenta));
        stopi=2*mini-maxi;
        //draw_x(stopi,QPen(Qt::magenta));
    }
    //calculate coefficents to convert index to frequency
    double kt = (fff-fsf)/data.count();
    sff = kt*(stopi)+fsf;
    ssf = kt*(starti)+fsf;
    k = (sff-ssf)/data.count();

    int max_data=0;
    for(int i=0;i<data.count();i++)
        if(data.at(i)>max_data)
            max_data=data.at(i);

    //	double curv=osc->getch1();
    //	double onediv=curv/25.4;
    //	double maxv=max_data*onediv;
    //	double newv=maxv/4;
    double ch1=osc->getch1();
    ch1=ch1*max_data/101.6;
    osc->setch1(ch1);
    k2=ch1/25.4;
    k3=ch1*5;

    gen->setsweep(ssf,sff);
    osc->wait("READY");
    gen->startsweep();
    osc->wait("READY");

    QByteArray data2 = osc->readcurve();
    return data2;
}

float MeasureThread::getamplonf(float freq)
{
    gen->setfreq(freq);
    return vol->acquire();
}

float MeasureThread::golden(float a, float b, float epsilon, bool max)
{
    const double phi = 1.61803398874989484;

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
    while(start<stop && sm >= 1)
    {
        QList<qreal> diff=sm_diff(dat,sm);
        //left to right
        for(int i=start;i<stop;i++)
        {
            if((max && diff[i]<=0) || (!max && diff[i]>=0))
            {
                start=i-sm;
                break;
            }
        }
        //right to left
        for(int i=stop;i>start;i--)
        {
            if((max && diff[i]>=0) || (!max && diff[i]<=0))
            {
                stop=i+sm;
                break;
            }
        }
        sm--;
    }

    return k*(start+stop)/2.0f+ssf;
#endif
}

double Func(Point3D P, double w)
{
    double R_m=P.x;
    double L_m=P.y;
    double C_m=P.z;
    double R_0=0;//?
    double C_0=0;//?

    double w2=pow(w,2);
    double C_02=pow(C_0,2);
    double R_m2=pow(R_m,2);
    double R_02=pow(R_0,2);
    double A = w*L_m - 1/(C_m*w);
    double A2 = pow(A,2);
    double Z = R_m2 + A2;
    double Z2=pow(Z,2);

    return (w2*C_02*Z2
            -2*w*A*C_0*Z+
            Z)/
            ((w2*C_02*R_02+1)*Z2
             +(2*R_m*R_0-2*w*A*C_0*R_02)*Z
             +Z*R_02);
}

double FuncDiff(Point3D P, double f)
{
    double R_m=P.x;
    double L_m=P.y;
    double C_m=P.z;
    double R_0=0;//?
    double C_0=0;//?

    double w=2*3.1415926535*f;
    double w2=pow(w,2);
    double C_02=pow(C_0,2);
    double R_m2=pow(R_m,2);
    double R_02=pow(R_0,2);
    double A = w*L_m - 1/(C_m*w);
    double A2 = pow(A,2);
    double Z = R_m2 + A2;
    double Z2=pow(Z,2);
    double B = (w2*C_02*R_02+1)*Z2+(2*R_m*R_0-2*w*A*C_0*R_02)*Z+R_02*Z;
    double B2 = pow(B,2);

    return (2*w*C_02*Z2-2*A*C_0*Z)/B
            -((w2*C_02*Z2-2*w*A*C_0*Z+Z)*(2*w*C_02*R_02*Z2-2*A*C_0*R_02*Z))/B2;
}

void MeasureThread::findresonance()
{
    int xmin=0,xmax1=0,xfmax=0,xmax2=0;

    dat.clear();
    QList<qreal> diff;

    int cycles=0;
    while ((xfmax <= xmax1)||(xfmax >= xmin))
    {
        //draw X axis
        dat = sweep();

        emit line(0,0,dat.count(),0,Qt::SolidLine);

        int fmax=0;
        for (int i=0; i<dat.count();i++)
        {
            if (fmax<dat[i])
            {
                fmax=dat[i];
                xfmax=i;
            }
        }

        emit path(dat,QPen(Qt::blue));

        diff=sm_diff(dat,sm2);

        emit path(diff, QPen(Qt::red));

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

        for (int i=0;i<xmin;i++)
        {
            if (max1<diff[i])
            {
                max1=diff[i];
                xmax1=i;
            }
        }

        for (int i=xmin;i<dat.count();i++)
        {
            if (max2<diff[i])
            {
                max2=diff[i];
                xmax2=i;
            }
        }

        draw_x(xmin,QPen(Qt::black));
        draw_x(xmax1,QPen(Qt::black));
        draw_x(xmax2,QPen(Qt::black));
        cycles++;
        if(cycles>4)
            throw GPIBGenericException(trUtf8("Could not acquire resonance qurve"));
    }

    for(int i=0; i<dat.count(); i++)
    {
        curve<<fPoint2D(k*i+ssf, dat[i]*k2);
    }

    gen->sweepoff();

    CSimplex3D simp(Func, 50, fPoint3D(25,25,25));
    simp.setData(curve);
    Point3D params = simp.optimize(0.1);

    QList<qreal> opt;
    for(double freq=ssf; freq<sff; freq +=k)
    {
        opt<<Func(params,freq);
    }
    emit path(opt,QPen(Qt::red));

    //rf=find_extremum(dat,xmax1,xmin,sm2,true);
    draw_x((rf-ssf)/k,QPen(Qt::black));
    ra=getamplonf(rf);
    //draw_y(-ra/k3,QPen(Qt::blue));

    //af=find_extremum(dat,xmin,xmax2,sm2,false);
    draw_x((af-ssf)/k,QPen(Qt::black));
    aa=getamplonf(af);
    //draw_y(-aa/k3,QPen(Qt::blue));

    gen->sweepon();
}



void MeasureThread::run()
{
    epsilon=0.1;
    fsf=startf;
    fff=stopf;

    gen=0;
    vol=0;
    osc=0;

    curve.clear();

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

        findresonance();

        if(!filename.isEmpty())
        {
            QFile f(filename);
            f.open(QFile::WriteOnly);
            QString buf;
            for(int i=0;i<curve.count();i++)
            {
                buf.setNum(curve.at(i).x,'f',10);
                f.write(buf.toAscii());
                f.write("\t");
                buf.setNum(curve.at(i).y,'f',10);
                f.write(buf.toAscii());
                f.write("\r\n");
            }
            f.close();
        }

        QStringList data;
        data<<QString("First run start freq, Hz:%1").arg(fsf);
        data<<QString("First run stop freq, Hz:%1").arg(fff);
        data<<QString("Second run start freq, Hz:%1").arg(ssf);
        data<<QString("Second run stop freq, Hz:%1").arg(sff);
        data<<QString("Resonance freq, Hz:%1").arg(rf);
        data<<QString("Resonance ampl, V:%1").arg(ra);
        data<<QString("Antiresonance freq, Hz:%1").arg(af);
        data<<QString("Antiresonance ampl, V:%1").arg(aa);
        emit finished(data);
        //------END main block-------------------
    } catch (GPIBGenericException e) {
        QStringList data;
        data<<QString("::ERROR::");
        data<<e.report();
        emit finished(data);
    } catch(std::exception e) {
        QStringList data;
        data<<QString("::ERROR::");
        data<<trUtf8("Caught unexpected exception %1").arg(e.what());
        emit finished(data);
    }

    delete gen;
    delete vol;
    delete osc;
}
