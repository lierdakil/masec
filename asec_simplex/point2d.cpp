#include "point2d.h"
#include <math.h>

Point2D fPoint2D(double x, double y)
{
    Point2D ret;
    ret.x=x;
    ret.y=y;
    return ret;
}

Point2D operator+(Point2D a, Point2D b)
{
    a.x+=b.x;
    a.y+=b.y;
    return a;
}

Point2D operator-(Point2D a, Point2D b)
{
    a.x-=b.x;
    a.y-=b.y;
    return a;
}

Point2D operator-(Point2D a)
{
    a.x=-a.x;
    a.y=-a.y;
    return a;
}

Point2D operator/(Point2D a, double b)
{
    a.x/=b;
    a.y/=b;
    return a;
}

Point2D operator*(Point2D a, double b)
{
    a.x*=b;
    a.y*=b;
    return a;
}

Point2D operator*(double b, Point2D a)
{
    a.x*=b;
    a.y*=b;
    return a;
}

double len(Point2D a, Point2D b)
{
    Point2D t = b-a;
    return sqrt(t.x*t.x+t.y*t.y);
}
