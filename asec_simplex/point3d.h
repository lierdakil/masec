#ifndef POINT3D_H
#define POINT3D_H

struct Point3D
{
    double x,y,z;
};

Point3D fPoint3D(double x, double y, double z);
Point3D operator+(Point3D a, Point3D b);
Point3D operator-(Point3D a, Point3D b);
Point3D operator-(Point3D a);
Point3D operator/(Point3D a, double b);
Point3D operator*(Point3D a, double b);
Point3D operator*(double b, Point3D a);
double len(Point3D a, Point3D b);

#endif // POINT3D_H
