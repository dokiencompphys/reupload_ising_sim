/*
 * Test wether different initial configurations end up at the
 * same average energy
*/
#define MAINPROGRAM

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <gsl/gsl_statistics.h>

#include "global.h"
#include "functions.h"
#include "geometry.h"

int main()
{
	time_t time_seconds = time(NULL);
	srand(time_seconds);

	/* CARE GLOBAL VARIABLES */
	N = 100; /* ((int) rand() % 15) + 1; */
	D = 2;  /* ((int) rand() % 2) + 1; */
	VOL = ipow(N, D);
	nn = nn_create();
	B = 0.01;
	beta = 0.5;
	config = 50000;
	/* ********************* */

	int *s = malloc(sizeof(int) * VOL);
	int *s2 = malloc(sizeof(int) * VOL);
	int *s3 = malloc(sizeof(int) * VOL);

	/* for 1nd random initial configuration */
	printf("for 1st random initial configuration\n");
	for(int i = 0; i < VOL; i++) {
		s[i] = rand() - RAND_MAX / 2;

		if(s[i] > 0 || s[i] == 0)
			s[i] = 1;
		else if(s[i] < 0)
			s[i] = -1;
	}

	print_2d_lattice(s);

	/* for 2nd random initial configuration */
	printf("for 2nd random initial configuration\n");
	for(int i = 0; i < VOL; i++) {
		s2[i] = rand() - RAND_MAX / 2;

		if (s2[i] > 0 || s2[i] == 0)
			s2[i] = 1;
		else if (s2[i] < 0)
			s2[i] = -1;
	}

	//print_2d_lattice(s2);

	/* for all spin up initial configuration */
	printf("for all spin up initial configuration\n");
	s3 = all_spin_parall(0);
	print_2d_lattice(s3);

	/* sum of energies */
	double *avg_E_s = malloc(sizeof(double) * config);
	double *avg_E_s2 = malloc(sizeof(double) * config);
	double *avg_E_s3 = malloc(sizeof(double) * config);
	double E = energy(s3);	/* for comparisons */
	double  E_s = 0;
	double E_s2 = 0;
	double E_s3 = 0;

	int seed = 1234;
	gsl_rng *r = gsl_rng_alloc(gsl_rng_taus);
	gsl_rng_set(r, seed);
	for (int step = 0; step < config; step++) {
		/* MonCar return type is double, check if theres errors without
		 * assigning to another variable */
		MonCar(s, r);
		MonCar(s2, r);
		MonCar(s3, r);

		E_s += energy(s);
		E_s2 += energy(s2);
		E_s3 += energy(s3);

		avg_E_s[step] = E_s;
		avg_E_s2[step] += E_s2;
		avg_E_s3[step] += E_s3;

		if (step > 0)
			printf( "%f\t%f\t%f\n", avg_E_s[step] / VOL / step,
					avg_E_s2[step]/VOL / step,
					 avg_E_s3[step]/ VOL / step);
	}

	printf("#random config1\trandom config2\tall spins up config\n");
	printf("#initial energy of all spins up: %f\n", E);
	double var_E_s = gsl_stats_sd_m(avg_E_s, 1, config, avg_E_s[config]);
	double var_E_s2 = gsl_stats_sd_m(avg_E_s2, 1, config, avg_E_s2[config]);
	double var_E_s3 = gsl_stats_sd_m(avg_E_s3, 1, config, avg_E_s3[config]);

	printf("#Variances are %f,\t%f, \t%f\n", var_E_s/VOL/config, var_E_s2/VOL/config, var_E_s3/VOL/config);

	gsl_rng_free(r);
	free(s);
	free(s2);
	free(s3);
	free(avg_E_s);
	free(avg_E_s2);
	free(avg_E_s3);

	return 0;
}
