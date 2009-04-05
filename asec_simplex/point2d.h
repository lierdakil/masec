#ifndef POINT2D_H
#define POINT2D_H

struct Point2D
{
    double x,y;
};

Point2D fPoint2D(double x, double y);
Point2D operator+(Point2D a, Point2D b);
Point2D operator-(Point2D a, Point2D b);
Point2D operator-(Point2D a);
Point2D operator/(Point2D a, double b);
Point2D operator*(Point2D a, double b);
Point2D operator*(double b, Point2D a);
double len(Point2D a, Point2D b);

#endif // POINT2D_H
