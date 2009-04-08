#ifndef POINT2D_H
#define POINT2D_H

#include "asec_simplex_global.h"

struct ASEC_SIMPLEXSHARED_EXPORT Point2D
{
    double x,y;
};

Point2D ASEC_SIMPLEXSHARED_EXPORT fPoint2D(double x, double y);
Point2D ASEC_SIMPLEXSHARED_EXPORT operator+(Point2D a, Point2D b);
Point2D ASEC_SIMPLEXSHARED_EXPORT operator-(Point2D a, Point2D b);
Point2D ASEC_SIMPLEXSHARED_EXPORT operator-(Point2D a);
Point2D ASEC_SIMPLEXSHARED_EXPORT operator/(Point2D a, double b);
Point2D ASEC_SIMPLEXSHARED_EXPORT operator*(Point2D a, double b);
Point2D ASEC_SIMPLEXSHARED_EXPORT operator*(double b, Point2D a);
double ASEC_SIMPLEXSHARED_EXPORT len(Point2D a, Point2D b);

#endif // POINT2D_H
