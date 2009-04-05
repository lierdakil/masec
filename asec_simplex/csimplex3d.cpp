#include "csimplex3d.h"
#include <math.h>

//Quicksort the array
void CSimplex3D::quicksort(int left, int right)
{
    int i = left, j = right;
    double x = f[(i + j) /2];
    do
    {
        while(f[i]<x)
            i++;
        while(f[j]>x)
            j--;
        if(i<=j)
            swap(i++, j--);
    }while(i<=j);

    if (i < right)
        quicksort(i,right);
    if (left < j)
        quicksort(left,j);

}

double CSimplex3D::Func(Point3D x)
{
    if(X.count()==0 || Y.count()==0 || X.count()!=Y.count())
        return F(x,0);

    double S=0;
    for(int i=0; i<X.count(); ++i)
    {
        double r = (Y[i]-F(x,X[i]));
        S+=r*r;
    }

    return S;
}

//Swap integer values by array indexes
void CSimplex3D::swap(int a, int b)
{
    double td = f[a];
    f[a] = f[b];
    f[b] = td;

    Point3D tp = x[a];
    x[a] = x[b];
    x[b] = tp;
}

void CSimplex3D::setData(QList<Point2D> data)
{
    foreach(Point2D d, data)
    {
        X.append(d.x);
        Y.append(d.y);
    }
}

CSimplex3D::CSimplex3D(double (*FuncPtr)(Point3D, double), double simplex_size, Point3D simplex_base, double reflection, double contraction, double expansion)
{
    dreflect=reflection;
    dcontract=contraction;
    dexpand=expansion;
    F=FuncPtr;

    double l=simplex_size;
    Point3D xc=simplex_base;
    double a = 109.5*3.1415926535/180;
    double b = 120*3.1415926535/180;

    x[0]=xc+l*fPoint3D(0,0,1);
    x[1]=xc+l*fPoint3D(sin(-a),0,cos(-a));
    x[2]=xc+l*fPoint3D(sin(a)*cos(b/2),sin(a)*sin(b/2),cos(a));
    x[3]=xc+l*fPoint3D(sin(a)*cos(-b/2),sin(a)*sin(-b/2),cos(a));

    //Now x[0..3] form thetraedre
}

void CSimplex3D::qs()
{
    //quicksort(0,3);
    int i, j;

    for(i = 3 ; i > 0 ; i--)
     {
        for(j = 0 ; j < i ; j++)
         {
            if( f[j] > f[j+1] ) swap(j,j+1);
         }
     }
}

Point3D CSimplex3D::optimize(double minV)
{
    for(int i=0; i<4; ++i)
        f[i] = Func(x[i]);

    start:
    //check
    /*double V = fabs(x[1].x*x[2].y*x[3].z+x[2].x*x[3].y*x[1].z+x[1].y*x[2].z*x[3].x-x[3].x*x[2].y*x[1].z-
               x[2].x*x[1].y*x[3].z-x[1].x*x[3].y*x[2].z-
               x[0].x*(x[2].y*x[3].z+x[1].y*x[2].z+x[3].y*x[1].z-x[2].y*x[1].z-x[1].y*x[3].z-
                       x[3].y*x[2].z)+
               x[0].y*(x[2].x*x[3].z+x[1].x*x[2].z+x[3].x*x[1].z-x[2].x*x[1].z-x[1].x*x[3].z-
                        x[3].x*x[2].z)+
               x[0].z*(x[2].x*x[3].y+x[3].x*x[1].y+x[1].x*x[2].y-x[2].x*x[1].y-x[1].x*x[3].y-
                       x[3].x*x[2].y))/6;*/
    double l[6];
    l[0]=len(x[0],x[1]);
    l[1]=len(x[1],x[2]);
    l[2]=len(x[2],x[3]);
    l[3]=len(x[3],x[0]);
    l[4]=len(x[0],x[2]);
    l[5]=len(x[1],x[3]);
    double maxl=l[0];
    for(int i=1;i<6;++i)
        if(l[i]>maxl)
            maxl=l[i];
    if(maxl<minV)
        return (x[0]+x[1]+x[2]+x[3])/4;
    //sort
    qs();//sorts in ascending order

    //center of gravity
    Point3D xc = (x[0]+x[1]+x[2])/3; //except x[3]

    //reflection
    Point3D xr = xc + dreflect*(xc - x[3]);//reflected point
    double fr = Func(xr);
    if(f[0]<=fr && fr<f[3])
    {
        x[3] = xr;
        f[3] = fr;
        goto start;
    }
    //expansion
    if(fr<f[0])
    {
        Point3D xe = xc + dexpand*(xc-x[3]);//expanded point
        double fe=Func(xe);
        if(fe<fr) {
            x[3] = xe;
            f[3] = fe;
            goto start;
        } else {
            x[3] = xr;
            f[3] = fr;
            goto start;
        }
    }
    //contraction
    //it's certain that fr>f[2]
    Point3D xcc = x[3]+dcontract*(xc-x[3]);
    double fcc = Func(xcc);
    if(fcc<=f[3])
    {
        x[3] = xcc;
        f[3] = fcc;
        goto start;
    }
    //shrink
    for(int i=1; i<4; ++i)
    {
        x[i]=x[0]+dcontract*(x[i]-x[0]);
        f[i]=Func(x[i]);
    }

    goto start;
}
