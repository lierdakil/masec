//#include "csimplexnd.h"
#include <math.h>
#include <QVector>
#include <QFile>
#include <QStringList>
#include <iostream>
#include "graph.h"
#include <QApplication>

#define iRm 0
#define iLm 1
#define iCm 2
#define iC0 3
#define iU 4
#define iR0 5


int main(int argc, char* argv[])
{
    QString parameters;
    QStringList arguments;
    bool consoleonly=false;
    bool has_table=true;

    for(int i=1; i<argc; ++i)
    {
        if(argv[i][0]=='-')
            parameters+=QString::fromLocal8Bit(argv[i]+1);
        else
            arguments<<QString::fromLocal8Bit(argv[i]);
    }

    if (!parameters.isEmpty())
    {
        for(int i=0; i<parameters.length(); ++i)
        {
            switch(parameters.at(i).toAscii()){
            case 'c':
                consoleonly=true;
                break;
            default:
                goto usage;
                break;
            }
        }
    }

    if(argc<2)
    {
        usage:
        std::cerr<<"Usage:\n"
                <<"\t"<<argv[0]<<" [-c] datafile(s)\n"
                <<"\tAnd ASCII table to append to on stdin\n"
                <<"\tc - console only, no gui\n";
        return 1;
    }

    QFile ft;
    QStringList table;
    if(ft.open(0,QFile::ReadOnly))
    {
        while(true)
        {
            QByteArray buf=ft.readLine();
            if(buf.isEmpty())
                break;
            QString line=QString::fromLocal8Bit(buf);
            if(line.startsWith('#'))
                std::cout<<line.toLocal8Bit().data();
            else if(!line.trimmed().isEmpty())
                table<<line.trimmed();
        }
        ft.close();
    }

    if(table.count()<=1)
    {
        has_table=false;
    } else if(table.count()!=arguments.count()+1)
    {
        std::cerr<<"Invalid ascii table specified";
        return 1;
    }

    if(has_table)
        table.first().append("\tFile\t"
                             "Simplex Resonance freq, Hz\t"
                             "Simplex Resonance ampl, V\t"
                             "Simplex Antiresonance freq, Hz\t"
                             "Simplex Antiresonance ampl, V\t"
                             "St. dev., V\t"
                             "Mean Noise, V");

    gsl_vector *units;
    units=gsl_vector_alloc(PARAM_COUNT);

    gsl_vector *a;
    a=gsl_vector_alloc(PARAM_COUNT);

    bool useC0=false ,useLm=false, useU=true, useR0=true;
    double inLm=0, inC0=0, inU=5, inR0=1000;

    for(int ifile=0;ifile<arguments.count();++ifile)
    {
        QFile f(arguments[ifile]);
        if (!f.open(QIODevice::ReadOnly))
        {
            std::cerr<<"could not open "<<arguments.at(ifile).toLocal8Bit().data();
            return 2;
        }

        if(has_table)
        {
            int i = table.first().split("\t").indexOf("Generator amplitude, VPP");
            if(i>=0)
            {
                bool ok;
                double tU = table.at(ifile+1).split("\t").at(i).toDouble(&ok);
                if(ok) inU=tU/2;
                std::cerr<<"read U value: "<<inU<<"\n";
            }
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
        int step=50;
        for(int i=step/2;i<func_data.count()-step/2;++i)
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
        noise/=func_sm_data.count();//Standard deviation from mean

        Point2D Res, Antires;
        int resi,aresi;
        Res=find_extremum(func_sm_data,true,&resi);
        Antires=find_extremum(func_sm_data,false,&aresi);

        //experimental
        if(ifile==0) //we only need initial parameters if it's first run, for other files in series,
            //parameters do not change much
        {
            double fr=Res.x, fa=Antires.x, Ir=Res.y, Ia=Antires.y;

            double C0,Lm,Rm,Cm,R0,U;
            R0=inR0;
            U=inU;
            double Zmax=U*R2/Ia,
            Zmin=U*R2/Ir;
            Rm=Zmin;
            C0=1/sqrt(4*pow(PI,2)*pow(fa,2)*Rm*Zmax);
            Lm=1/(4*pow(PI,2)*C0*(pow(fa,2)-pow(fr,2)));
            Cm=1/(4*PI*PI*fr*fr*Lm);

            gsl_vector_set(a,iRm,Rm);
            gsl_vector_set(a,iLm,Lm);
            gsl_vector_set(a,iCm,Cm);
            gsl_vector_set(a,iC0,C0);
            gsl_vector_set(a,iU,U);
            gsl_vector_set(a,iR0,R0);

            gsl_vector_set_all(units,1e-10);
            gsl_vector_mul(units,a);//units=a*1e-10;

            gsl_vector_set_all(a,1e10);
        }

        int gstatus=0;
        do{
            {//update Cm if f0 changed a bit too dramatically
                double Lm = useLm ? inLm : gsl_vector_get(a,iLm)*gsl_vector_get(units,iLm);
                double Cm = gsl_vector_get(a,iCm)*gsl_vector_get(units,iCm);
                double f0=1/(2*PI*sqrt(Lm*Cm));
                if(f0<func_sm_data.first().x || f0>func_sm_data.last().x)
                    gsl_vector_set(a,iCm,1/(gsl_vector_get(units,iCm)*4*PI*PI*Res.x*Res.x*Lm));
            }

            gsl_vector *ss;//step size
            ss=gsl_vector_alloc(PARAM_COUNT);
            gsl_vector_set_all(ss, 1e-3);
            gsl_vector_mul(ss,a);

            gsl_multimin_function func;
            func.n=PARAM_COUNT;
            func.f=&StDev;

            param_struct func_params;

            func_params.data=&func_data;
            func_params.resi=resi;
            func_params.aresi=aresi;

            func_params.f_min=func_data.first().x;
            func_params.f_max=func_data.last().x;
            func_params.RmU=gsl_vector_get(units,iRm);
            func_params.LmU=gsl_vector_get(units,iLm);
            func_params.CmU=gsl_vector_get(units,iCm);
            func_params.C0U=gsl_vector_get(units,iC0);
            func_params.UU=gsl_vector_get(units,iU);
            func_params.R0U=gsl_vector_get(units,iR0);

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
            double oldsize=0;

            do
            {
                iter++;
                if(iter % 10==0)
                    oldsize=gsl_multimin_fminimizer_size(min);
                status = gsl_multimin_fminimizer_iterate(min);

                if (status)
                    break;

                double size = gsl_multimin_fminimizer_size(min);
                //status = gsl_multimin_test_size (size, 1e-10);
                if(size!=oldsize || iter%10!=9 || size>10)
                    status=GSL_CONTINUE;
                else
                    status=GSL_SUCCESS;

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
                            <<"U=";
                    if(!useC0) std::cerr<<gsl_vector_get (min->x, 4)*gsl_vector_get (units, 4);
                    else std::cerr<<inU;
                    std::cerr<<"\t"
                            <<"R0=";
                    if(!useC0) std::cerr<<gsl_vector_get (min->x, 5)*gsl_vector_get (units, 5);
                    else std::cerr<<inR0;
                    std::cerr<<"\n"
                            <<"StDev="<<min->fval<<"\n"
                            <<"Noise="<<noise<<"\n";
                } else {
                    std::cerr<<iter<<"\t"
                            <<min->fval<<"\t"
                            <<size<<"\n";
                }
            }
            while (status == GSL_CONTINUE && iter < 10000);

            QVector<qreal> X_exp,Y_exp,X_f,Y_f;

            foreach(Point2D P, func_data)
            {
                X_exp.push_back(P.x);
                Y_exp.push_back(P.y);
            }

            double maxf=func_data[0].x;
            double maxI=If(min->x,&func_params,maxf);
            double minf=maxf;
            double minI=If(min->x,&func_params,maxf);

            for(double freq=func_data[0].x; freq<func_data.last().x; ++freq)
            {
                double I=If(min->x,&func_params, freq);
                X_f.push_back(freq);
                Y_f.push_back(I);
                if(I>maxI)
                {
                    maxI=I;
                    maxf=freq;
                }

                if(I<minI)
                {
                    minI=I;
                    minf=freq;
                }
            }

            std::cerr << "Resonance freqency = " << maxf << "\n"
                    << "Resonance amplitude = " << maxI << "\n"
                    << "Antiresonance frequency = " << minf << "\n"
                    << "Antiresonance amplitude = " << minI << "\n";

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

            QApplication app(argc,argv);
            Graph g(X_exp, Y_exp, X_f, Y_f, gsl_vector_get(par,0), gsl_vector_get(par,1),
                    gsl_vector_get(par,2), gsl_vector_get(par,4), gsl_vector_get(par,3),
                    gsl_vector_get(par,5), minf, minI, maxf, maxI);
            g.setWindowTitle(f.fileName());
            if(!consoleonly)
                gstatus=g.exec();
            else
                gstatus=QDialog::Accepted; //if it was not shown, we have to agree :)

            inU=g.U();
            inR0=g.R0();
            inLm=g.Lm();
            inC0=g.C0();

            if(gstatus==QDialog::Rejected)
            {
                gsl_vector_set(par,iRm,g.Rm());
                gsl_vector_set(par,iLm,g.Lm());
                gsl_vector_set(par,iCm,g.Cm());
                gsl_vector_set(par,iC0,g.C0());
                gsl_vector_set(par,iU,g.U());
                gsl_vector_set(par,iR0,g.R0());
                gsl_vector_memcpy(a,par);
                gsl_vector_div(a,units);
            } else {
                useLm=true;
                useC0=true;
                useU=true;
                useR0=true;
                if (has_table) {
#define table_append_num(v) table[ifile+1].append(QString::number(v,'f',10)+"\t")
                    table[ifile+1].append("\t"+f.fileName()+"\t");
                    table_append_num(maxf);
                    table_append_num(maxI);
                    table_append_num(minf);
                    table_append_num(minI);
                    table_append_num(min->fval);
                    table_append_num(noise);
                }
            }

            gsl_vector_free(par);
        }while(gstatus==QDialog::Rejected);
    }
    gsl_vector_free(a);
    gsl_vector_free(units);
    if (has_table)
        foreach(QString s, table)
            std::cout<<s.toLocal8Bit().data()<<"\n";
}


