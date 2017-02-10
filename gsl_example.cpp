/* clang++ -w -o ./gsl_example -O4 ./gsl_example.cpp -I /usr/local/include/ -L /usr/local/lib/ -lgsl -lgslcblas
 */

#include <stdlib.h>
#include <stdio.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_multifit_nlin.h>

#include "mathOps.h"
using namespace std;

struct datav {
    size_t n;
    vector <vector <double> > yIn;
    vector <vector <double> > yOut;
    double * sigma;
};

int expb_fv (const gsl_vector * x,
             void *params,
             gsl_vector * f)
{
    size_t n = ((struct datav *)params)->n;
    vector <vector <double> > yin = ((struct datav *)params)->yIn;
    vector <vector <double> > yout = ((struct datav *)params)->yOut;

    double *sigma = ((struct datav *) params)->sigma;
    
//    double B = gsl_vector_get (x, 0);
//    double B[6];
//    FORI(6)
//        B[i] = gsl_vector_get (x, i);
//    
//    size_t i;
//    
//    double b[9] = { B[0], B[1], 1.0 - B[0] - B[1],
//                    B[2], B[3], 1.0 - B[2] - B[3],
//                    B[4], B[5], 1.0 - B[4] - B[5] };
    
    double b[9];
    FORI(9) {
        b[i] = gsl_vector_get (x, i);
        printf("%f, ", b[i]);
    }

    FORI(yout.size()) {
        FORJ(yout[0].size())
        {
            double ytmp = b[3 * j + 0] * yin[i][0] + b[3*j + 1] * yin[i][1] + b[3*j + 2] * yin[i][2];
            gsl_vector_set (f, i*(yout[0].size())+j, (yout[i][j] - ytmp)/sigma[i*(yout[0].size())+j]);
        }
    }

    return GSL_SUCCESS;
};

int expb_dfv (const gsl_vector * x,
              void *params,
              gsl_matrix * J)
{
    size_t n = ((struct datav *)params)->n;
    double *sigma = ((struct datav *) params)->sigma;
    
//    double B[6];
//    FORI(6)
//        B[i] = gsl_vector_get (x, i);
    
    double B[9];
    FORI(9)
        B[9] = gsl_vector_get (x, i);
    
    size_t i;

    FORI(n){
        double s = sigma[i];
//        FORJ(6) {
        FORJ(9) {
            gsl_matrix_set (J, i, j, B[j]/s);
        }
    }

    return GSL_SUCCESS;
};


int expb_fdfv (const gsl_vector * x,
               void *params,
               gsl_vector * f, gsl_matrix * J)
{
    expb_fv (x, params, f);
    expb_dfv (x, params, J);
    
    return GSL_SUCCESS;
};

int print_state (size_t iter,
                 gsl_multifit_fdfsolver * s)
{
    printf ("iter: %3u x = % 15.8f % 15.8f % 15.8f % 15.8f % 15.8f % 15.8f "
            "|f(x)| = %g\n",
            iter,
            gsl_vector_get (s->x, 0),
            gsl_vector_get (s->x, 1),
            gsl_vector_get (s->x, 2),
            gsl_vector_get (s->x, 3),
            gsl_vector_get (s->x, 4),
            gsl_vector_get (s->x, 5),
            gsl_blas_dnrm2 (s->f));
};

int main (void)
{
    const gsl_multifit_fdfsolver_type *Tv;
    gsl_multifit_fdfsolver *sv;
    
    std::vector < std::vector <double> > GT(3, std::vector<double>(3));
    FORI(3)
        FORJ(3)
            GT[i][j] = ground_truth[i][j];
    
    std::vector < std::vector <double> > IN_Test(10, std::vector<double>(3));
    FORI(10)
        FORJ(3)
            IN_Test[i][j] = in_test[i][j];
    
    std::vector < std::vector <double> > OUT_Test = transposeVec(mulVector(GT, IN_Test));
    
    int status;
    size_t i, iter = 0;
    
    const size_t n = 40;

    const size_t nv = OUT_Test.size() * (OUT_Test[0].size());
    const size_t pv = 9;

    gsl_matrix *covarv = gsl_matrix_alloc (pv, pv);
    
    double sigmav[nv];
    
    FORI(nv)
        sigmav[i] = 1.0;
    
    struct datav dv = {nv, IN_Test, OUT_Test, sigmav};
    
    gsl_multifit_function_fdf fv;
    
//    double x_init[6] = { 1.0, 0.0, 0.0 };
    double xv_init[9];
    
    xv_init[0] = 1.0;
    xv_init[1] = 0.0;
    xv_init[2] = 1.0 - xv_init[0] - xv_init[1];
    xv_init[3] = 1.0;
    xv_init[4] = 0.0;
    xv_init[5] = 1.0 - xv_init[3] - xv_init[4];
    xv_init[6] = 0.0;
    xv_init[7] = 0.0;
    xv_init[8] = 1.0 - xv_init[6] - xv_init[7];
    
    gsl_vector_view xv = gsl_vector_view_array (xv_init, pv);
    
    const gsl_rng_type * type;
    gsl_rng * r;
    
    gsl_rng_env_setup();
    
    type = gsl_rng_default;
    r = gsl_rng_alloc (type);
    
    fv.f = &expb_fv;
    fv.df = &expb_dfv;
    fv.fdf = &expb_fdfv;
    fv.n = nv;
    fv.p = pv;
    fv.params = &dv;
    
    Tv = gsl_multifit_fdfsolver_lmsder;
    sv = gsl_multifit_fdfsolver_alloc (Tv, nv, pv);
    gsl_multifit_fdfsolver_set (sv, &fv, &xv.vector);
    
    print_state (iter, sv);
    
    do
    {
        iter++;
        status = gsl_multifit_fdfsolver_iterate (sv);
        
        printf ("status = %s\n", gsl_strerror (status));
        
        print_state (iter, sv);
        
        if (status)
            break;
        
        status = gsl_multifit_test_delta (sv->dx,
                                          sv->x,
                                          1e-17,
                                          1e-17);
    }
    while (status == GSL_CONTINUE && iter < 500);
    
    gsl_multifit_covar (sv->J, 0.0, covarv);
    gsl_matrix_fprintf (stdout, covarv, "%g");
    
#define FIT(i) gsl_vector_get(sv->x, i)
#define ERR(i) sqrt(gsl_matrix_get(covarv,i,i))
    
    printf("B[0] = %.5f +/- %.5f\n", FIT(0), ERR(0));
    printf("B[1] = %.5f +/- %.5f\n", FIT(1), ERR(1));
    printf("B[2] = %.5f +/- %.5f\n", FIT(2), ERR(2));
    printf("B[3] = %.5f +/- %.5f\n", FIT(3), ERR(3));
    printf("B[4] = %.5f +/- %.5f\n", FIT(4), ERR(4));
    printf("B[5] = %.5f +/- %.5f\n", FIT(5), ERR(5));
    printf("B[6] = %.5f +/- %.5f\n", FIT(6), ERR(6));
    printf("B[7] = %.5f +/- %.5f\n", FIT(7), ERR(7));
    printf("B[8] = %.5f +/- %.5f\n", FIT(8), ERR(8));
    
    printf ("status = %s\n", gsl_strerror (status));
    
    gsl_multifit_fdfsolver_free (sv);

    return 0;
}
