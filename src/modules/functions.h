#include <gsl/gsl_rng.h>

int magnetization(int *s);
double energy(int *s);
double MonCar(int *s, gsl_rng *r);

void print_2d_lattice(int *s);
int *altn_spin_lattice (void);
int *all_spin_parall(int sign);

void ExpFit(double *C, double *k, int n, double *a, double *b);
