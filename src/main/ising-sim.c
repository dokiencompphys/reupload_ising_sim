#define MAINPROGRAM

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <gsl/gsl_rng.h>

#include "global.h"
#include "functions.h"
#include "geometry.h"

int main(int argc, char *argv[])
{
	/* time_t time_seconds = time(NULL); */

	/* CARE GLOBAL VARIABLES */
	N = 100;
	D = 2;
	B = 0.01;
	beta = 0.1;
	config = 50000;
	/* ********************* */

	/* default seed*/
	int seed = 1234;

	/* COMMAND LINE HANDLING */
	int opt;
	char *ptr; /* pointer for checking conversion of strings to numerics */
	opterr = 1;
	while ((opt = getopt(argc, argv, "+N:D:B:b:s:")) != -1) {
		switch (opt) {
			case 'N':
				/* N lattice points in each dimension */
				N = strtol(optarg, &ptr, 0);
#ifdef DEBUG
				printf("opt N: left over string after conv.: |%s|\n", ptr);
#endif
				if ((*ptr != '\0') | (N <= 0)) {
					printf("N = |%s| is not a valid positive integer\n",
							optarg);
					exit(-1);
				}
				break;
			case 'D':
				/* D dimensions */
				D = strtol(optarg, &ptr, 0);
#ifdef DEBUG
				printf("opt D: left over string after conv.: |%s|\n", ptr);
#endif
				if ((*ptr != '\0') | (D <= 0)) {
					printf("D = |%s| is not a valid positive integer\n",
							optarg);
					exit(-1);
				}
				break;
			case 'B':
				/* B magnetic flux */
				B = strtod(optarg, &ptr);
#ifdef DEBUG
				printf("opt B: left over string after conv.: |%s|\n", ptr);
#endif
				if (*ptr != '\0') {
					printf("B = |%s| is not a valid double\n",
							optarg);
					exit(-1);
				}
				break;
			case 'b':
				/* beta inverse temperature (1/(k_b*T)) */
				beta = strtod(optarg, &ptr);
#ifdef DEBUG
				printf("opt b: left over string after conv.: |%s|\n", ptr);
#endif
				if (*ptr != '\0') {
					printf("beta = |%s| is not a valid double\n",
							optarg);
					exit(-1);
				}
				break;
			case 's':
				/* set seed for pseudo rng */
				seed = strtol(optarg, &ptr, 0);
#ifdef DEBUG
				printf("opt s: left over string after conv.: |%s|\n", ptr);
#endif
				if ((*ptr != '\0') | (seed <= 0)) {
					printf("s = |%s| is not a valid positive integer\n",
							optarg);
					exit(-1);
				}
				break;
			case '?':
				/* error case */
				exit(-1);
		}
	}
	/* ********************* */

	/* CARE GLOBAL CALC VARIABLES */
	VOL = ipow(N, D);
	nn = nn_create();
	/* ************************** */

	/* rng initialization */
	srand(seed);
	gsl_rng *r = gsl_rng_alloc(gsl_rng_taus);
	gsl_rng_set(r, seed);

	/* print system variables */
	print_global_v();
	printf("#seed = %d\n", seed);

	/* random initial configuration */
	int *s = malloc(sizeof(int) * VOL);
	for(int i = 0; i < VOL; i++) {
		s[i] = rand() - RAND_MAX / 2;
		if (s[i] > 0 || s[i] == 0)
			s[i] = 1;
		else if(s[i] < 0)
			s[i] = -1;
	}

	/* initial energy of initial config s */
	double E_init = energy(s);
	/* energy for current MC Step */
	double E = E_init;
	int Mag = magnetization(s);

	printf("#mc_time\tenergy\t magnetization\n");
	printf("%d\t%f\t%d\n", 0, E_init, Mag);
	for (int step = 1; step < config; step++) {
		E += MonCar(s, r);
		Mag = magnetization(s);
		printf("%d\t%f\t%d\n", step, E, Mag);
	}
	gsl_rng_free(r);
	free(s);
	return 0;
}
