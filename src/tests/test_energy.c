/*
test program, which checks for different configurations wether it matches
analytical result.
*/

#define MAINPROGRAM

#include <stdlib.h>
#include <stdio.h>
#include <time.h>


#include "global.h"
#include "functions.h"
#include "geometry.h"


int main()
	/* for big N an error in the energy calculation accumulates, see e.g.
	 * N = 2000000, D = 1, B = 0.1
	 */
	/* TODO: check inconsistent energy calculation for alternating spin conf */
{
	/* CARE GLOBAL VARIABLES */
	N = 4000000;
	D = 1; 
	B = 0.1;
	beta =0.01;

	VOL = ipow(N, D);
	nn = nn_create();
	/* ********************* */

	time_t time_seconds = time(NULL);
	srand(time_seconds);

	int sign = 0;	/* 0 means spin up */

	/* TODO: test wether I can do that */

	VOL = 1;
	for(int i = 0; i < D; i++)
		VOL *= N;


	double E = 0;

	/* set up initial configuartion */
	int *s = malloc(sizeof(int) * VOL); /* Is there a more proper data type? */
	for(int i = 0; i < VOL; i++) {
		s[i] = rand() - RAND_MAX / 2;
		if (s[i] > 0 || s[i] == 0)
			s[i]=1;
		else if (s[i] < 0)
			s[i]=-1;

	}

#ifdef DEBUG
	printf("initial configuration is:\n");
	print_2d_lattice(s);
#endif

	/* print energy */
	E = energy(s);
	printf("energy for random config: %f\n", E);

	/* ********************************************************************* */

	s = all_spin_parall(sign);

#ifdef DEBUG
	printf("ALL SPIN UP config:\n");
	print_2d_lattice(s);
#endif

	/* print energy */
	E = energy(s);
	printf("energy for all spin-up config: %f\n", E);

	double Emin = -(D + B) * VOL;
	printf("analytical energy for all spin-up config:"
			" H=-(D+B)*VOL = %f\n", Emin);

	/* ********************************************************************* */

//	s = altn_spin_lattice();
#ifdef DEBUG
//	printf("ALTERNATING SPIN config:\n");
//	print_2d_lattice(s);
#endif

//	E = energy(s);
//	printf("energy for alternating spin config: %f\n", E);


	int k;
	k = VOL % 2;
	if (k == 0) {
		/* VOL is even */
		double Ealtn_even = VOL * D;
		printf("energy for alternating spin config (even N)"
				" H=D*VOL = %f\n", Ealtn_even);
	} else {
		double Ealtn_odd = VOL * D - B;
		printf("energy for alternating spin config (odd N)"
				" H=D*VOL-B = %f\n", Ealtn_odd);

	}
	printf("**NOTE: that only for 'even' we get the right energy for " 
			"  alternating  spin configuration, because the "
			" neighbours on the boundary will be antiparallel to "
			" the respective lattice point.\n"
			" In thermodynamic limit these boundary effects will have"
			" no role.**\n");
	printf("**NOTE: We also observe that H_aB != H_B. (coupling with "
		 	" magnetic field). It matters wether we calculate "
	       		" a sum (over many numbers) over the product "
		       	" of B and lattice point or calculate the sum first  "
			" and then multiply with B.\n");

	return 0;
}
