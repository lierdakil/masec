#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <point2d.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_multimin.h>
#include <QVector>

#define PI 3.1415926535
#define R2 33000
#define PARAM_COUNT 6

struct param_struct {
    QVector<Point2D> *data;
    int resi;
    int aresi;
    double f_min;
    double f_max;
    double RmU,LmU,CmU,C0U,UU,R0U;
    double Rm,Lm,Cm,C0,U,R0;
};

double If(const gsl_vector *v, void *params, double f); //I(f)

double StDev(const gsl_vector *v, void *params); //sum from 0 to N-1 (I_exp(f)-I(f))**2

Point2D find_extremum(QVector<Point2D> dat, bool max, int* index=0);

#endif // FUNCTIONS_H
