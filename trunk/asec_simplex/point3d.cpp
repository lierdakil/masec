#include "point3d.h"
#include <math.h>

Point3D fPoint3D(double x, double y, double z)
{
    Point3D ret;
    ret.x=x;
    ret.y=y;
    ret.z=z;
    return ret;
}

Point3D operator+(Point3D a, Point3D b)
{
    a.x+=b.x;
    a.y+=b.y;
    a.z+=b.z;
    return a;
}

Point3D operator-(Point3D a, Point3D b)
{
    a.x-=b.x;
    a.y-=b.y;
    a.z-=b.z;
    return a;
}

Point3D operator-(Point3D a)
{
    a.x=-a.x;
    a.y=-a.y;
    a.z=-a.z;
    return a;
}

Point3D operator/(Point3D a, double b)
{
    a.x/=b;
    a.y/=b;
    a.z/=b;
    return a;
}

Point3D operator*(Point3D a, double b)
{
    a.x*=b;
    a.y*=b;
    a.z*=b;
    return a;
}

Point3D operator*(double b, Point3D a)
{
    a.x*=b;
    a.y*=b;
    a.z*=b;
    return a;
}

double len(Point3D a, Point3D b)
{
    Point3D t = b-a;
    return sqrt(t.x*t.x+t.y*t.y+t.z*t.z);
}
