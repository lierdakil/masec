#include "functions.h"

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

    if(f0<(p->f_min) || f0>(p->f_max))//if it tries to walk away from solution, return something big
        return U/R0;//it's actually the theoretical maximum

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

