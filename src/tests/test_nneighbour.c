#define MAINPROGRAM

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "global.h"
#include "geometry.h"


/* reminder: 0 is false, every other number is true */
int main()
	/* TEMPORARY NOT FINISHED nneighbour function test */
	/* TODO: more information in the printf */
{
	/* CARE GLOBAL VARIABLES */
	N = 100;
	D = 3;
	B = 0;
	beta = 0.01;

	VOL = ipow(N, D);
	nn = nn_create();
	/* ********************* */

	print_global_v();

	/* test of nneighbour by checking the difference in each dimension
	 * between neighbours
	 */
	int *coord = init_coord();
	int *coord_n = init_coord();

#ifdef DEBUG
					/* DEBUG PRINT */
					printf("ind axis direction difference\n");
#endif

	for (int ind = 0; ind < VOL; ind++) {	/* iteration over all indices */
		ind2coord(coord, ind);

#ifdef DEBUG
					/* DEBUG PRINT */
					printf("%d ", ind);
#endif

		for (int d = 0; d < D; d++) {		/* axis of neighboar */
			for (int dir = 0; dir < 2; dir++) { /* bw=0/fw=1 direction */
				int ind_n = nneighbour(ind, d, dir);
				ind2coord(coord_n, ind_n);

				for (int i = 0; i < D; i++) {
					int diff = coord[i] - coord_n[i];
					diff = (N + diff) % N;

#ifdef DEBUG
					/* DEBUG PRINT */
					printf("%d %s %d\n", d, dir ? "fw" : "bw", diff);
#endif

					if (i == d && (diff == (dir ? N - 1 : 1))) {
					} else if (i != d && diff == 0) {
					} else {
						printf("[ geometry.c : nneighbour ]"
								" test: comparing differences"
								" in coordinates of neighbours: failed\n");
						exit(-1);
					}
				}
			}
		}
	}

	/* free doesn't remove declared variables/pointers */
	/* actually there is no real way to remove them at all in c */
	free(coord);
	free(coord_n);

	printf("[ geometry.c : nneighbour ]"
			" test: comparing differences in coordinates"
			" of neighbours: successful\n");


	/* test of nn which is the product of **nn_create() by checking the
	 * difference of coordinates in each dimension between neighbours
	 */

	coord = init_coord();
	coord_n = init_coord();

	for (int ind = 0; ind < VOL; ind++) {	/* iteration over all indices */
		ind2coord(coord, ind);

		for (int d = 0; d < D; d++) {		/* axis of neighboar */
			for (int dir = 0; dir < 2; dir++) { /* bw=0/fw=1 direction */
				int ind_n = nn[d][2 * ind + dir];
				ind2coord(coord_n, ind_n);

				for (int i = 0; i < D; i++) {
					int diff = coord[i] - coord_n[i];
					diff = (N + diff) % N;

#ifdef DEBUG
					/* DEBUG PRINT */
					printf("%d ", diff);
#endif

					if (i == d && (diff == (dir ? N - 1 : 1))) {
					} else if (i != d && diff == 0) {
					} else {
						printf("[ geometry.c : nn_create ]"
								" test: comparing differences"
								" in coordinates of neighbours: failed\n");
						exit(-1);
					}
				}
			}
		}
	}

	free(coord);
	free(coord_n);

	printf("[ geometry.c : nn_create ]"
			" test: comparing differences in coordinates"
			" of neighbours: successful\n");

	return 0;
}

