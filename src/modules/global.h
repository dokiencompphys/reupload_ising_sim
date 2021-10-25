#ifndef MAINPROGRAM
	#define EXTERN extern
#else
	#define EXTERN
#endif

EXTERN int D;		/* number of space dimensions */
EXTERN int N;		/* number of lattice points in one dimension */
EXTERN int VOL;		/* volume = N^D */
EXTERN int **nn;	/* pointer to nearest neighbour array, see **nn_create() */
EXTERN double B;	/* magnetic flux */
EXTERN double beta;	/* inverse temp */
EXTERN int config;	/* number of configurations */
