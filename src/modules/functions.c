/*
 * This program contains some functions.
 */


#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <gsl/gsl_rng.h>

#include "global.h"
#include "geometry.h"


/* different configurations,
all spin up for sign=0
all spin down for any other integer */

int* all_spin_parall(int sign)
{
	int *s=malloc(sizeof(int)*VOL);
	if(sign ==0){
		for(int i=0; i < VOL; i++)
			s[i]=1;
	}else{
		for(int i=0; i < VOL; i++)
			s[i]=-1;
	}
	return s;
}

/* start with spin up */

int* altn_spin_chain(void)
{
	int *s=malloc(sizeof(int)*N);
	int k;
	k= N % 2;
	if(k ==0){
		// VOLis even
		for(int i=0; i < N/2; i++){
			s[2*i]=1;
			s[2*i+1]=-1;
		}
	}else{
		for(int i=0; i < (N-1)/2; i++){
			s[2*i]=1;
			s[2*i+1]=-1;
		}
		s[N-1]=1;
	}
	/*
	for(int i=0; i < N; i++){
		if(i %N < N-1){
			printf("%d	", s[i]);
		}else{
			printf("%d \n", s[i]);
		}
	}
	*/
	return s;
}

int *altn_spin_lattice (void)
{
	int *s= malloc(sizeof(int)*VOL);
	int *se=malloc(sizeof(int)*N);
	se=altn_spin_chain();
	int n = 0;
	while(n < VOL){
		// concerns every odd line: 1 -1 1 -1 ...
		for(int i = 0; i < N; i++) {
			s[n] = se[i];
			n+=1;
		}

		//next column starts with -1 1 -1 1 ...
		for(int m = 0; m < N; m++) {
			s[n] = -1*se[m];
			n+=1;
		}
	}
	free(se);
	return s;
}


void print_2d_lattice(int *s)
{
	if(D > 2) {
		printf("ATTENTION: This is D > 2\n\n");
	}
	for(int i = 0; i < VOL; i++) {
		if(i % N < N - 1)
			printf("%d	", s[i]);
		else
			printf("%d\n", s[i]);
	}
}

int magnetization(int *s)
{
	int M = 0.0;
	for(int k = 0; k < VOL; k++)
		M += s[k];
	return M;
}

/*
 * calculate energy. input is a configuration, which must be a pointer
 */
double energy(int *s)
{
	double H = 0;
	double H_B = 0;
	double H_aB= 0;

	/* loop over whole lattice and get the neighbour */
	for(int x = 0; x < VOL; x++){
		int d = 0;
		for(int u = 0; u < D; u++) /* sum of all neighbouring spins */
			/* TODO: test where we have given nn array and calculate for
			 * ourselves wether they give the right sum */
			d += s[nn[u][2 * x]];
		/* printf("d = %d \t", d); */	
		H -= d  * s[x];
		H_B -=  B* s[x] ;
		H_aB -= s[x];
	}
	H_aB= H_aB*B;
#ifdef DEBUG
	printf("#H_aB = %f\n", H_aB);
	printf("#H_B = %f\n", H_B);
	printf("#H = %f\n", H);
#endif
	return H + H_aB;
}



/* TODO: change comment */
/*
 * Function: MonCar
 * ----------------
 *  calculates a new spin configuration, based on a probability rule.
 *  Contributes to find good samples for averaging.
 *  Creates one Monte Carlo step in Markov chain.
 *
 *  *s: pointer to a spin configuration of a d-dimensional lattice mapped
 *  to 1D by index mapping
 *
 *
 */
double MonCar(int *s, gsl_rng *r)
{
	double rnd;

	double e_diff = 0.0; /* real applied energy difference after the mc */

	for(int z = 0; z < VOL ; z++) {
		/* updated configuration */
		/* energy difference determines probability rule */
		double diff = 2 * B * s[z] ;
		for(int u = 0; u < D; u++)
			/* need back and forth neighbours */
			diff += 2 * (s[nn[u][2 * z]] + s[nn[u][2 * z + 1]] ) * s[z];
		/* set prob for flip */
		double p = exp(-beta * diff);

		if(p >= 1) {
			s[z] = -s[z];
			e_diff += diff;
		}
		else {
			/* draw a random number between 0,1 uniformly */
			rnd = gsl_rng_uniform_pos(r); /* exclude 1 */

			if (p > rnd) {
				s[z] = -s[z];
				e_diff += diff;
			}
		}
	}
	return e_diff;
}


/*
 * Function: exponential fit
 * -------------------------------
 * generates the two best parameters a and b in the formula y = a * exp(b * k).
 *
 * IN:
 *  *C
 *	  Data for the y-axis of the data
 *  *k
 *    Data for the x-axis of the data
 *  n
 *    Amount of points
 * OUT:
 *  *a, *b
 *	  best fits parameters at the end of the program
 *
 */
void ExpFit(double *C, double *k, int n, double *a, double *b)
{
	double sum_1 = 0;
	double sum_2 = 0;
	double sum_3 = 0;
	double sum_4 = 0;
	double x;
	double y;

	/* Least squares fitting */
	/* Data transformed to linear data */
	for(int i = 0; i < n; i++){
		x = k[i];
		y = log(C[i]);
		sum_1 += x * y;
		sum_2 += x;
		sum_3 += y;
		sum_4 += x * x;
	}
	*b = (sum_1 - 1. / (double) n * sum_2 * sum_3)
		/ (sum_4 - 1. / (double) n * sum_2 * sum_2);
	*a = exp((1. / (double) n) * sum_3 - (1. / (double) n) * (*b) * sum_2);
}
