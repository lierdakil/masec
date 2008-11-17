#include "measure.h"
#include "sleep.h"
//TODO: Fix comments...

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

cmeasure::cmeasure(QString oscstr, QString genstr, QString volstr, double sf, double ff, double epsilon, QWidget *window)
{
    connect(this,SIGNAL(path(QList<qreal>,QPen)),window,SLOT(path(QList<qreal>,QPen)),Qt::QueuedConnection);
    connect(this,SIGNAL(path(QByteArray,QPen)),window,SLOT(path(QByteArray,QPen)),Qt::QueuedConnection);
    connect(this,SIGNAL(line(qreal,qreal,qreal,qreal,QPen)),window,SLOT(line(qreal,qreal,qreal,qreal,QPen)),Qt::QueuedConnection);
	this->oscstr=oscstr;
	gen = new genctrl(genstr);
	vol = new volctrl(volstr);
	fsf=sf;
	fff=ff;
	this->epsilon=epsilon;
	findresonance();
}

cmeasure::~cmeasure()
{
	delete gen;
	delete vol;
}

QByteArray cmeasure::sweep()
{
	osc = new oscctrl((char*)oscstr.toAscii().data());

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

		QList<qreal> diff = sm_diff(data,2);

		double min_diff_val=0;
		int min_diff_index;

		//find diff_min
		for (int i=0;i<diff.count();i++)
			if (diff[i]<min_diff_val)
			{
				min_diff_val = diff[i];
				min_diff_index = i;
			}

		//find diff=0 right from diff_min <=> min
		for (int i=min_diff_index;i<diff.count()-1;i++)
			if (diff[i]<=0 && diff[i+1]>0)
			{
				mini = i;
				break;
			}

		//find diff=0 left from diff_min <=> max
		for (int i=min_diff_index;i>0;i--)
			if (diff[i]>=0 && diff[i-1]<0)
			{
				maxi = i;
				break;
			}

//		starti=2*maxi-min_diff_index;//double interval
//		stopi=2*mini-min_diff_index;
		starti=2*maxi-mini;
		stopi=2*mini-maxi;
	}
	//calculate coefficents to convert index to frequency
	double kt = (fff-fsf)/data.count();
	sff = kt*(stopi)+fsf;
	ssf = kt*(starti)+fsf;
	k = (sff-ssf)/data.count();

	k2=osc->setch1(data[maxi]);
	//qDebug()<<1;
	sleep(1);//TODO: no idea why it dies without this
	gen->setsweep(ssf,sff);
	osc->wait("READY");
	gen->startsweep();
	//qDebug()<<4;
	osc->wait("READY");
	//qDebug()<<5;

	QByteArray data2 = osc->readcurve();

	delete osc;
	return data2;
}

float cmeasure::getamplonf(float freq)
{
	gen->setfreq(freq);
	return vol->acquire();
}

float cmeasure::golden(float a, float b, float epsilon, bool max)
{
	//Метод золотого сечения
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

void cmeasure::findresonance()
{
	int xmin=0,xmax1=0,xfmax=0,xmax2=0;

	QByteArray dat;
	QList<qreal> diff;

	while ((xfmax <= xmax1)||(xfmax >= xmin))
	{
		dat = sweep();

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
		emit line(0,0,dat.count(),0,Qt::SolidLine);

		diff=sm_diff(dat,12);

		emit path(diff, QPen(Qt::red));

		float min=255;
		float max1=0;
		float max2=0;

		for (int i=0;i<dat.count();i++)
			if (min>diff[i])
			{
				min=diff[i];
				xmin=i;
			}

		for (int i=0;i<xmin;i++)
			if (max1<diff[i])
			{
				max1=diff[i];
				xmax1=i;
			}

		for (int i=xmin;i<dat.count();i++)
			if (max2<diff[i])
			{
				max2=diff[i];
				xmax2=i;
			}

		emit line(xmin,0,xmin,-127,Qt::SolidLine);
		emit line(xmax1,0,xmax1,-127,Qt::SolidLine);
		emit line(xmax2,0,xmax2,-127,Qt::SolidLine);
	}

	for(int i=0; i<dat.count(); i++)
	{
		curve<<QPair<double,double>(k*i+ssf, dat[i]*k2);
	}

	gen->sweepoff();

#ifdef GOLDEN
	rf = golden(k*xmax1+newsf,k*xmin+newsf,epsilon,true);
#else
	rf=0;
	for(int i=xmax1;i<xmin;i++)
	{
		if(diff[i]<0)
		{
			rf=i;
			break;
		}
	}
	rf=k*rf+ssf;
#endif
	ra=getamplonf(rf);

	emit line((rf-ssf)/k,0,(rf-ssf)/k,-127,Qt::SolidLine);

#ifdef GOLDEN
	af = golden(k*xmin+newsf,k*xmax2+newsf,epsilon,false);
#else
	af=0;
	for(int i=xmin;i<xmax2;i++)
	{
		if(diff[i]>0)
		{
			af=i;
			break;
		}
	}
	af=k*af+ssf;
#endif
	aa=getamplonf(af);

	emit line((af-ssf)/k,0,(af-ssf)/k,-127,Qt::SolidLine);
}

