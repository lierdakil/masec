#ifndef CSIMPLEX3D_H
#define CSIMPLEX3D_H

#include "asec_simplex_global.h"
#include <stdlib.h>
#include "point2d.h"
#include "point3d.h"
#include <QList>

class ASEC_SIMPLEXSHARED_EXPORT CSimplex3D
{
private:
    double (*F)(Point3D, double);
    double dreflect,dcontract,dexpand;
    Point3D x[4];
    double f[4];
    QList<double> X;
    QList<double> Y;

    void qs();
    void quicksort(int left, int right);
    void swap(int a, int b);
    double Func(Point3D x);

public:
    CSimplex3D(double (*FuncPtr)(Point3D, double), double simplex_size, Point3D simplex_base,  double reflection=1, double contraction=0.5, double expansion=2);
    void setData(QList<Point2D> data);
    Point3D optimize(double minV);
};

#endif // CSIMPLEX3D_H
