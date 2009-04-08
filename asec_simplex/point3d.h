#ifndef POINT3D_H
#define POINT3D_H

#include "asec_simplex_global.h"

struct ASEC_SIMPLEXSHARED_EXPORT Point3D
{
    double x,y,z;
};

Point3D ASEC_SIMPLEXSHARED_EXPORT fPoint3D(double x, double y, double z);
Point3D ASEC_SIMPLEXSHARED_EXPORT operator+(Point3D a, Point3D b);
Point3D ASEC_SIMPLEXSHARED_EXPORT operator-(Point3D a, Point3D b);
Point3D ASEC_SIMPLEXSHARED_EXPORT operator-(Point3D a);
Point3D ASEC_SIMPLEXSHARED_EXPORT operator/(Point3D a, double b);
Point3D ASEC_SIMPLEXSHARED_EXPORT operator*(Point3D a, double b);
Point3D ASEC_SIMPLEXSHARED_EXPORT operator*(double b, Point3D a);
double ASEC_SIMPLEXSHARED_EXPORT len(Point3D a, Point3D b);

#endif // POINT3D_H
