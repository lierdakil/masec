/***********************************************/
/* T.Kouya's GSL sample program collection     */
/*          Solving Linear System of Equations */
/*                            LU decomposition */
/*                   Written by Tomonori Kouya */
/*                                             */
/* Version 0.01: 2007-10-02                    */
/***********************************************/
#include <stdio.h>
#include <math.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>

/* Dimension of Matrix and Vectors */
#define DIM 3
//#define LAMBDA 1.788965
//#define LAMBDA 1.540562
#define PI 3.1415926535

int main()
{
    int i, j;
    gsl_matrix *a;
    gsl_vector *x, *b;
//    int signum;
//    gsl_permutation *perm;

    /* allocate a, x, b */
    a = gsl_matrix_alloc(DIM, DIM);
    x = gsl_vector_alloc(DIM);
    b = gsl_vector_alloc(DIM);

    float LAMBDA;
    scanf("%f",&LAMBDA);

    /* set matrix                      */
    for(i = 0; i < DIM; i++)
    {
        int h,k,l;
        float THETA;
        scanf("%d %d %d %f",&h,&k,&l,&THETA);
        THETA=THETA*PI/360;
        gsl_matrix_set(a, i, 0, h*h);
        gsl_matrix_set(a, i, 1, k*k);
        gsl_matrix_set(a, i, 2, l*l);
        gsl_vector_set(b,i,4*sin(THETA)*sin(THETA)/(LAMBDA*LAMBDA));
    }

    /* Print matrix */
    printf("Matrix(DIM = %d)\n", DIM);
    for(i = 0; i < DIM; i++)
    {
        printf("%3d: ", i);
        for(j = 0; j < DIM; j++)
            printf("%g ", gsl_matrix_get(a, i, j));
        printf("%f\n", gsl_vector_get(b,i));
    }
    printf("\n");

    /* LU decomposition and forward&backward substition */
    /*perm = gsl_permutation_alloc(DIM);
    gsl_linalg_LU_decomp(a, perm, &signum);
    gsl_linalg_LU_solve(a, perm, b, x);
    gsl_permutation_free(perm);*/
    //Householder transformations
    gsl_linalg_HH_solve(a,b,x);

    /* print */
    for(i = 0; i < DIM; i++)
        printf("%d: %f\n", i, 1/sqrt(gsl_vector_get(x, i)));

    /* free a, x, b */
    gsl_matrix_free(a);
    gsl_vector_free(x);
    gsl_vector_free(b);

    return 0;
}
