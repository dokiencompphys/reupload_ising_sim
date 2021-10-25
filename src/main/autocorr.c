/* calculate auto correlation function  */


#define MAINPROGRAM

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <gsl/gsl_statistics.h>

#include "geometry.h"
#include "global.h"

int main(int argc, char *argv[])
{
	/* for our purposes it is ok, to hardcode this part. One coould 
	 * improve here, by reading the whole file here, already. 
	 * ignore all the lines which are indicated as comments and
	 *  then do analysis. */
	
	/* CARE GLOBAL VARIABLE */
	N = 100;
	D = 2;
	VOL = ipow(N,D);
	config = 50000;
	/* ******************* */

	/* COMMAND LINE HANDLING */
	int t = 1; /* thermalization */
	int opt;
	char *fil; 
	char *ptr; 
	int len;
	opterr=1;

	while((opt = getopt(argc, argv, "+t:c:f:")) != -1) {
		switch(opt) {
			case 't':
				/* thermalization: cut the first 
				 * unphysical points out */
				t = strtol(optarg, &ptr, 0);
				
				if ((*ptr != '\0') | (t <= 0)) {
					printf("t = |%s| is not a valid \
							positive integer\n",
							optarg);
					exit(-1);
				}
				break;
			case 'f':
				fil = optarg;
				len = strlen(fil);		
				/* read name of the file with data in */
				if (((*ptr) != '\0') | ((fil[len - 4]) != '.') \
					       	| ((fil[len - 3]) != 'd')  \
					        | ((fil[len - 2]) != 'a') \
					       	| ((fil[len - 1]) != 't')  ) {
					/* one can improve by reading every 
					 * string and compare it with available
					 * names in the directory */
					printf(" opt f: is not of *.dat");
					exit(-1);
				}
				break;
			case '?':
				exit(-1);
			}
	}

	
	FILE *file = fopen(fil, "r");

	double *E = malloc(sizeof(double) * (config-t));
	double *M = malloc(sizeof(double) * (config-t));

	double num = 0;
	double  num1 = 0;
	int num2 = 0;
	
	double mean_E = 0;
	double mean_M = 0;
	double std_E = 0;
        double std_M = 0;

	char check[100];
	int comm = 3; /* numbers of comment lines in *.dat file  */	
	char parameter_line[100]; /* reprint the parameter line  */

	fgets(parameter_line, 100, file);

	/* read off comment lines, so the only data left in file 
	 * are the relevant numbers */
	for(int n = 0; n < comm-1; n++)
		fgets(check, 100, file);

	/* thermalization time */
	for(int n = 0; n < t; n++)
		fgets(check, 100, file);

	int k = 0;
	while(!feof(file)) {
		fscanf(file,"%d", &num2 );
		fscanf(file,"%lf", &num  );
		fscanf(file,"%lf", &num1 );

		E[k] = num;
		M[k]= num1;
		k = k + 1;
	}
//	for(int k = 0; k < config; k++)
//		printf("%lf\t%lf\n", E[k], M[k]);
	mean_E = gsl_stats_mean(E, 1, config - t);
	std_E = gsl_stats_sd_m(E, 1, config - t, mean_E);
	
	mean_M = gsl_stats_mean(M, 1, config - t);
	std_M = gsl_stats_sd_m(M, 1, config - t, mean_M);

	double t_ac_E = 0;
	double t_ac_M = 0;

	double cov_E;
	double cov_M;

	double auto_corr_E;
	double auto_corr_M;

	int cut = 1600; /* we only use the first 'cut' amount of points
			   to prevent influence of not having
		   	   independent random variables */

	printf("%s", parameter_line);
	printf("#Autocorrelation time with Thermalization time =%d\n", t);
	printf("#lag\tenergy\tMagnetization\n");	
	

	for(int k = 0; k < cut ; k++) {
		auto_corr_E = 0;
		auto_corr_M = 0;

		/* calculate autocorrelation here */
		for(int n = k ; n < config - t; n++) {
			auto_corr_E += (E[n] - mean_E) * (E[n - k] - mean_E);
			auto_corr_M += (M[n] - mean_M) * (M[n - k] - mean_M);

		}
		auto_corr_E = auto_corr_E / (config - t - k);
		auto_corr_M = auto_corr_M / (config - t - k);
		
		if( k == 0) {
			cov_E = auto_corr_E;
			cov_M = auto_corr_M;
		}

		/* Normalization */
		auto_corr_E = auto_corr_E / cov_E;
		auto_corr_M = auto_corr_M / cov_M;
		
		/* integrated autocorrelation time */
		t_ac_E += auto_corr_E;
		t_ac_M += auto_corr_M;

		/* print the integrated auto correlation time vs lag */
		//printf("%d\t%f\t%f\n", k, t_ac_E, t_ac_M);

		/* auto correlation function */
	 	printf("%d\t%f\t%f\n", k, auto_corr_E, auto_corr_M);

	}

	int N_eff_E = (config - t) / (2* t_ac_E);
	int N_eff_M = (config - t) / (2* t_ac_M);

	/* print statistics */ 
	printf("#config-t \t <H> \t dH \t t_ac_E \t 1/1_ac_E \t error <H> \t <M> \t dM \t \
			t_ac_M \t 1/t_ac_M \t error <M> \n");
	printf("#%d\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n", \
		       	config-t, mean_E/VOL, std_E/VOL , t_ac_E,1/t_ac_E,\
		       	std_E/VOL/sqrt(N_eff_E), mean_M/VOL,\
		       	std_M/VOL, t_ac_M, 1/t_ac_M, std_M/VOL/sqrt(N_eff_M));

	free(E);
	free(M);
	fclose(file);

	return 0;
}

