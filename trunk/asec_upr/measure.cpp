#include "measure.h"
//TODO: Fix comments...

cmeasure::cmeasure(QString oscstr, QString genstr, QString volstr, float sf, float ff, float epsilon,QGraphicsScene *scene)
{
	this->oscstr=oscstr;
	gen = new genctrl(genstr);
	vol = new volctrl(volstr);
	fsf=sf;
	fff=ff;
	this->epsilon=epsilon;
	this->scene=scene;
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

	char maxd=0;
	char mind=127;

	QByteArray data;

	while(!(mini>maxi))
	{
		gen->setsweep(fsf,fff);
		osc->wait("READY");
		gen->startsweep();
		osc->wait("TRIGGER");
		osc->wait("READY");
		data = osc->readcurve();

		for (int i=0;i<data.length();i++)
		{
			if (maxd<data[i])
			{
				maxd = data[i];
				maxi = i;
			}

			if (mind>data[i])
			{
				mind = data[i];
				mini = i;
			}
		}

		starti=maxi-(mini-maxi)-1;
		stopi=mini+(mini-maxi)+1;
	}
	//�������������� ����������� �������� �� ������ ����� � �������,
	//������ � �����
	double kt = (fff-fsf)/data.count();
	sff = kt*(stopi)+fsf;
	ssf = kt*(starti)+fsf;
	k = (sff-ssf)/data.count();

        //���������� ��������� ������� ������ ������������ - �������� � ����
        //qDebug()<<"1!";
        k2=osc->setch1(maxd/2);
        //���������� ����� �������� �������
        //qDebug()<<"2!";
        gen->setsweep(ssf,sff);
        //qDebug()<<"3!";

	//��������� ���������� ������������
        osc->wait("READY");
        //qDebug()<<"4!";

	//������ ������ ������ �� �������
	gen->startsweep();
	osc->wait("TRIGGER");//�� �����?
	osc->wait("READY");

        //��������� ������ � �����������
        //qDebug()<<"5!";
	data = osc->readcurve();

	delete osc;

        //qDebug()<<"6!";
	return data;
}

float cmeasure::getamplonf(float freq)
{
	gen->setfreq(freq);
	//_sleep(1000); //msec
	return vol->acquire();
}

float cmeasure::golden(float a, float b, float epsilon, bool max)
{
	//��������� "�������� �������"
	const double phi = 1.61803398874989484;

	float x1=b-(b-a)/phi; //��������� ����������
	float x2=a+(b-a)/phi;

	float y1=getamplonf(x1); //�������� ��������� ����������
	float y2=getamplonf(x2); //�������� ��������� ����������


	while ((b-a)>=epsilon*2) //���� �� ���������� ������� ��������
	{
		if ( (y1>y2 && max) || (y1<y2 && !max)) //������ ��������/������� ������� "�������� �������"
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
	QList<double> diff;

	while ((xfmax <= xmax1)||(xfmax >= xmin))
	{
		dat = sweep();

		int fmax=0;
                QPainterPath curve;
                curve.moveTo(0,-dat[0]);

		for (int i=0; i<dat.count();i++)
                {
			if (fmax<dat[i])
			{
				fmax=dat[i];
				xfmax=i;
                        }
                        curve.lineTo(i,-dat[i]);
		}

		if (scene!=0)
                {
                        scene->addLine(0,0,dat.count(),0,Qt::SolidLine);
			scene->addPath(curve,QPen(Qt::blue),Qt::NoBrush);
                        //((QGraphicsView*)scene->parent())->fitInView(scene->sceneRect());
		}

		const int nr=12;//Depth of smoothing in one direction
                diff.clear();

		QPainterPath diff_c;
		diff_c.moveTo(0,0);

		for (int i=0;i<dat.count();i++)
		{
			if(i<nr || i>=dat.count()-nr)
			{
                                diff<<0;
				diff_c.lineTo(i,0);
				continue;
			}
			double a=0;
			double b=0;
			for (int k=-nr;k<=nr;k++)
			{
				a+=k*dat[i+k]-k*dat[i+k]/nr;
				b+=k*k;
			}

                        diff<<a/b*10;

			diff_c.lineTo(i,-diff[i]);
		}
		if (scene!=0)
		{
			scene->addPath(diff_c,QPen(Qt::red),Qt::NoBrush);
                        //((QGraphicsView*)scene->parent())->fitInView(scene->sceneRect());
		}

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

		if (scene!=0)
		{
			scene->addLine(xmin,0,xmin,-127,Qt::SolidLine);
			scene->addLine(xmax1,0,xmax1,-127,Qt::SolidLine);
			scene->addLine(xmax2,0,xmax2,-127,Qt::SolidLine);
                        //(QGraphicsView*)scene->parent())->fitInView(scene->sceneRect());
		}
        }

        qDebug()<<"!!";

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

	if (scene!=0)
	{
		scene->addLine((rf-ssf)/k,0,(rf-ssf)/k,-127,Qt::SolidLine);
                //((QGraphicsView*)scene->parent())->fitInView(scene->sceneRect());
	}

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

	if (scene!=0)
	{
		scene->addLine((af-ssf)/k,0,(af-ssf)/k,-127,Qt::SolidLine);
                //((QGraphicsView*)scene->parent())->fitInView(scene->sceneRect());
	}
}

