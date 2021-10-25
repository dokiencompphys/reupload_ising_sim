#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "global.h" /* global external variables */


int ipow(int base, int exp)
{
	if (exp < 0) {
		fprintf(stderr, "[ geometry.c : ipow ] Error!"
				" exponent has to be >0 (exp = %d)\n", exp);
		exit(1);
	}

	int ret = 1;

	while (exp-- > 0)
		ret *= base;

	return ret;
}


void print_global_v(void)
{
	printf("#D=%d, N=%d, VOL=%d, B=%f, beta=%f\n", D, N, VOL, B, beta);
}


int *init_coord(void)
{
	int *coord = malloc(sizeof(int) * D);
	for (int i = 0; i < D; i++)
		coord[i] = 0;
	return coord;
}


/*
 * Function: coord2ind
 * -------------------
 * returns indices for given coordinates as we map a D-dimensional
 * object to a 1-D.
 *
 * *coord: pointer to array of integers (coordinates of point)
 *
 */
int coord2ind(int *coord)
{
	int ind = 0;
	for(int k = 0; k < D; k++)
		ind += coord[k] * ipow(N, k);
	return ind;
}


/*
 * Function:ind2coord
 * ---------------------
 * function which gives out coordinates to corresponding indices.
 * It is the reverse function of coord2ind.
 *
 * *coord: pointer to array of integers (coordinates of point)
 *
 * ind: integer, indicates which coordinates will come out
 */
void ind2coord(int *coord, int ind)
{
	int count = D - 1;

	while(count >= 0) {
		coord[count] = ind / ipow(N, count);
		ind = ind % ipow(N, count);

		count = count - 1;
	}
}


/*
 * Function: nneighbour
 * --------------------
 * returns index of nearest neighbour. Boundary conditions are respected.
 *
 * ind_in: integer, initial index point
 *
 * axis: integer, indicates in which axis we look for the neighbour
 *	 axis[k] corresponds to look for coord[k+-1]
 *
 * direction: integer, indicates wether coord[k+1], or coord[k-1]
 *	      is being searched.
 *
 */

/* non-zero e.g. 1(true) for positive direction
 * 0(false) for negative direction */
int nneighbour(int ind_in, int axis, int direction)
{
	int *coord_out, ind_out;
	coord_out = malloc(sizeof(int) * D);

	ind2coord(coord_out, ind_in);

	/* transform in lattice first */
	if (direction)
		coord_out[axis] = (coord_out[axis] + 1) % N;
		/* boundary condition */
	else
		coord_out[axis] = (N + coord_out[axis] - 1) % N;

	/* transform back*/
	ind_out = coord2ind(coord_out);
	free(coord_out);

	return ind_out;
}


/*
 * Function: nn_create
 * -------------------
 * creates an pointer, which points to pointers. These pointers point
 * to arrays of different dimensions. Effectively having in each row alternating
 * between back (-) and forth (+) nearest neighbour of one lattice point.
 * In each column we specify another dimension.
 *
 */
int **nn_create(void)
{
	int **nn;
	nn = malloc(sizeof(int*) * D);

	for (int i = 0; i < D; i++)
		*(nn + i) = malloc(sizeof(int) * 2 * ipow(N, D));
		/* jump to the next block*/

	for (int i = 0; i < D; i++)
		for (int j = 0; j < ipow(N, D); j++) {
			nn[i][2 * j] = nneighbour(j, i, 0);
			nn[i][2 * j + 1] = nneighbour(j, i, 1);
		}

	return nn;
}
