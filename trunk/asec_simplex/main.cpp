//#include "csimplexnd.h"
#include <math.h>
#include <QVector>
#include <QFile>
#include <QStringList>
#include <iostream>
#include "graph.h"
#include <QApplication>
#include <point2d.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_multimin.h>

#define PI 3.1415926535
#define R2 33000
#define PARAM_COUNT 6

struct param_struct {
    QVector<Point2D> *data;
    double f;
    double f_min;
    double f_max;
    double RmU,LmU,CmU,C0U,UU,R0U;
    double Rm,Lm,Cm,C0,U,R0;
};

double If(const gsl_vector *v, void *params) //I(f)
{
    param_struct *p = (param_struct*)params;

    double Rm,Lm,Cm,C0,U,R0;
    if(p->Rm<0) Rm=fabs(gsl_vector_get(v,0))*p->RmU;
    else Rm=p->Rm;
    if(p->Lm<0) Lm=fabs(gsl_vector_get(v,1))*p->LmU;
    else Lm=p->Lm;
    if(p->Cm<0) Cm=fabs(gsl_vector_get(v,2))*p->CmU;
    else Cm=p->Cm;
    if(p->C0<0) C0=fabs(gsl_vector_get(v,3))*p->C0U;
    else C0=p->C0;
    if(p->U<0) U=fabs(gsl_vector_get(v,4))*p->UU*R2;
    else U=p->U*R2;//we actually need not U, but U*R2
    if(p->R0<0) R0=fabs(gsl_vector_get(v,5))*p->R0U;
    else R0=p->R0;

    if(Rm==0 || Lm==0 || Cm==0 || C0==0)
        return GSL_NAN;

    double f0 = 1/(2*PI*sqrt(Lm*Cm));

    if(f0<(p->f_min) || f0>(p->f_max))
        return GSL_NAN;

    double w=2*PI*p->f;

    double A = (((pow(Cm,2)*pow(w,4)*pow(Rm,2)+pow(Cm,2)*pow(w,6)*pow(Lm,2)-2*Cm*pow(w,4)*Lm+pow(w,2))*pow(C0,2)+(2*Cm*pow(w,2)-2*pow(Cm,2)*pow(w,4)*Lm)*C0+pow(Cm,2)*pow(w,2))*pow(R0,2)+2*pow(Cm,2)*pow(w,2)*Rm*R0+pow(Cm,2)*pow(w,2)*pow(Rm,2)+pow(Cm,2)*pow(w,4)*pow(Lm,2)-2*Cm*pow(w,2)*Lm+1)/((pow(Cm,2)*pow(w,4)*pow(Rm,2)+pow(Cm,2)*pow(w,6)*pow(Lm,2)-2*Cm*pow(w,4)*Lm+pow(w,2))*pow(C0,2)+(2*Cm*pow(w,2)-2*pow(Cm,2)*pow(w,4)*Lm)*C0+pow(Cm,2)*pow(w,2));
    if(A>0)
        return U/sqrt(A);
    else
        return GSL_NAN;
}

double StDev(const gsl_vector *v, void *params) //sum from 0 to N-1 (I_exp(f)-I(f))**2
{
    param_struct *p = (param_struct*)params;
    QVector<Point2D> *data = p->data;
    double S=0;
    for(int i=0; i<data->count(); ++i)
    {
        p->f = data->at(i).x;
        double I = If(v,params);
        if(I==GSL_NAN)
            return I;
        double r = (data->at(i).y-I);
        S+=r*r;
    }
    return S;
}

Point2D find_extremum(QVector<Point2D> dat, bool max, int* index=0)
{
    double extY=dat.first().y;
    int exti=0;
    for(int i=0;i<dat.count();++i)
    {
        if((max && extY<=dat[i].y)||(!max && extY>dat[i].y))
        {
            extY=dat[i].y;
            exti=i;
        }
    }
    if(index!=0)
        *index=exti;
    return dat[exti];
}

int main(int argc, char* argv[])
{
    if(argc<=3)
    {
        std::cerr<<"Usage:\n"
                <<"\t"<<argv[0]<<" Lm C0 datafile\n";
        return 1;
    }

    std::cout<<"File\t"
            "Resonance frequency, Hz\t"
            "Resonance amplitude, V\t"
            "Antiresonance frequency, Hz\t"
            "Antiresonance amplitude, V\t"
            "St. dev., V\t"
            "Mean Noise, V\n";

    gsl_vector *units;
    units=gsl_vector_alloc(PARAM_COUNT);

    gsl_vector *a;
    a=gsl_vector_alloc(PARAM_COUNT);

    bool useC0,useLm, useU=false, useR0=false;
    double inLm, inC0, inU, inR0;

    for(int ifile=3;ifile<argc;++ifile)
    {
        QFile f(argv[ifile]);
        if (!f.open(QIODevice::ReadOnly))
        {
            std::cerr<<"could not open "<<argv[ifile];
            return 2;
        }

        QVector<Point2D> func_data;//I(f)
        while(!f.atEnd())
        {
            QStringList line = QString::fromAscii(f.readLine()).split('\t');
            func_data<<fPoint2D(line[0].toDouble(),line[1].toDouble());
        }
        f.close();

        QVector<Point2D> func_sm_data; //smoothen initial data and lessen number of points
        double noise=0;
        int step=100;
        for(int i=step/2;i<func_data.count()-step/2;i+=step)
        {
            double y=0;
            double x=0;
            for(int k=i-step/2; k<i+step/2;++k)
            {
                x+=func_data[k].x;
                y+=func_data[k].y;
            }
            x/=step;
            y/=step;
            func_sm_data<<fPoint2D(func_data[i].x,y);
            noise+=pow(func_sm_data.last().y-func_data[i].y,2);
        }
        //noise/=func_sm_data.count();//Standard deviation from mean

        //experimental
        if(ifile==3) //we only need initial parameters if it's first run, for other files in serise,
            //parameters do not change much
        {
            Point2D Res, Antires;
            Res=find_extremum(func_data,true);
            Antires=find_extremum(func_data,false);
            double fr=Res.x, fa=Antires.x, Ir=Res.y, Ia=Antires.y;

            inLm=QString(argv[1]).toDouble(&useLm);
            inC0=QString(argv[2]).toDouble(&useC0);


            double C0,Lm,Rm,Cm,R0,U;
            R0=1000;
            U=5;
            double Zmax=U*R2/Ia,
            Zmin=U*R2/Ir;
            Rm=Zmin;
            if(!useC0) C0=1/sqrt(4*pow(PI,2)*pow(fa,2)*Rm*Zmax);
            else C0=inC0;
            if(!useLm) Lm=1/(4*pow(PI,2)*C0*(pow(fa,2)-pow(fr,2)));
            else Lm=inLm;
            Cm=1/(4*PI*PI*fr*fr*Lm);

            gsl_vector_set(a,0,Rm);
            gsl_vector_set(a,1,Lm);
            gsl_vector_set(a,2,Cm);
            gsl_vector_set(a,3,C0);
            gsl_vector_set(a,4,U);
            gsl_vector_set(a,5,R0);

            gsl_vector_set_all(units,1e-10);
            gsl_vector_mul(units,a);

            gsl_vector_set_all(a,1e10);
        }

        gsl_vector *ss;//step size
        ss=gsl_vector_alloc(PARAM_COUNT);
        gsl_vector_set_all(ss, 1e-3);
        gsl_vector_mul(ss,a);

        gsl_multimin_function func;
        func.n=PARAM_COUNT;
        func.f=&StDev;

        param_struct func_params;

        func_params.data=&func_sm_data;
        func_params.f_min=func_sm_data.first().x;
        func_params.f_max=func_sm_data.last().x;
        func_params.RmU=gsl_vector_get(units,0);
        func_params.LmU=gsl_vector_get(units,1);
        func_params.CmU=gsl_vector_get(units,2);
        func_params.C0U=gsl_vector_get(units,3);
        func_params.UU=gsl_vector_get(units,4);
        func_params.R0U=gsl_vector_get(units,5);

        func_params.Rm=-1;
        func_params.Lm=-1;
        func_params.Cm=-1;
        func_params.C0=-1;
        func_params.U=-1;
        func_params.R0=-1;

        if(useLm) func_params.Lm=inLm;
        if(useC0) func_params.C0=inC0;
        if(useU) func_params.U=inU;
        if(useR0) func_params.R0=inR0;

        func.params=(void*)&func_params;

        gsl_multimin_fminimizer * min =
                gsl_multimin_fminimizer_alloc (gsl_multimin_fminimizer_nmsimplex, PARAM_COUNT);
        gsl_multimin_fminimizer_set(min, &func, a, ss);
        int status;
        size_t iter=0;
        do
        {
            iter++;
            status = gsl_multimin_fminimizer_iterate(min);

            if (status)
                break;

            double size = gsl_multimin_fminimizer_size (min);
            status = gsl_multimin_test_size (size, 1e-3);

            if (status == GSL_SUCCESS)
            {
                std::cerr
                        <<"converged to minimum in \""<<f.fileName().toLocal8Bit().data()<<"\" at\n"
                        <<"Rm="<<gsl_vector_get (min->x, 0)*gsl_vector_get (units, 0)<<"\t"
                        <<"Lm=";
                if(!useLm) std::cerr<<gsl_vector_get (min->x, 1)*gsl_vector_get (units, 1);
                else std::cerr<<inLm;
                std::cerr<<"\t"
                        <<"Cm="<<gsl_vector_get (min->x, 2)*gsl_vector_get (units, 2)<<"\t"
                        <<"C0=";
                if(!useC0) std::cerr<<gsl_vector_get (min->x, 3)*gsl_vector_get (units, 3);
                else std::cerr<<inC0;
                std::cerr<<"\n"
                        <<"StDev="<<min->fval<<"\n"
                        <<"Noise="<<noise<<"\n";
            } else {
                std::cerr<<iter<<"\t"
                        <<min->fval<<"\t"
                        <<size<<"\n";
            }
        }
        while (status == GSL_CONTINUE && iter < 2000);

        QVector<qreal> X_exp,Y_exp,X_f,Y_f;

        foreach(Point2D P, func_data)
        {
            X_exp.push_back(P.x);
            Y_exp.push_back(P.y);
        }


        double maxf=func_data[0].x;
        func_params.f = maxf;
        double maxI=If(min->x,&func_params);
        double minf=maxf;
        double minI=If(min->x,&func_params);

        for(double f=func_data[0].x; f<func_data.last().x; f++)
        {
            func_params.f = f;
            double I=If(min->x,&func_params);
            X_f.push_back(f);
            Y_f.push_back(I);
            if(I>maxI)
            {
                maxI=I;
                maxf=f;
            }

            if(I<minI)
            {
                minI=I;
                minf=f;
            }
        }
        std::cout<<f.fileName().toAscii().data()<<"\t"<<maxf<<"\t"<<maxI<<"\t"<<minf<<"\t"<<minI<<"\t"<<min->fval<<"\t"<<noise<<"\n";

        gsl_vector_memcpy(a, min->x);

        gsl_vector *par;
        par = gsl_vector_alloc(PARAM_COUNT);
        gsl_vector_memcpy(par, min->x);
        gsl_vector_mul(par,units);
        if(useLm) gsl_vector_set(par,1,inLm);
        if(useC0) gsl_vector_set(par,3,inC0);
        if(useU) gsl_vector_set(par,4,inU);
        if(useR0) gsl_vector_set(par,5,inR0);

        gsl_multimin_fminimizer_free(min);
        gsl_vector_free(ss);

        if(ifile==3)
        {
            useLm=true;
            inLm=gsl_vector_get(par,1);
            useC0=true;
            inC0=gsl_vector_get(par,3);
//            useU=true;
//            inU=gsl_vector_get(par,4);
            useR0=true;
            inR0=gsl_vector_get(par,5);
        }

        QApplication app(argc,argv);
        Graph* g= new Graph(X_exp, Y_exp, X_f, Y_f, gsl_vector_get(par,0), gsl_vector_get(par,1),
                            gsl_vector_get(par,2), gsl_vector_get(par,4), gsl_vector_get(par,3),
                            gsl_vector_get(par,5));
        gsl_vector_free(par);
        g->setWindowTitle(f.fileName());
        g->show();
        app.exec();
    }
    gsl_vector_free(a);
    gsl_vector_free(units);
}


