#!/bin/bash
#generates autocorrelation data (run gen_data.sh before)

bin=../executables/auto

index="0 1 2 3 4 5 6 7 8 9 10 11"

declare -a v_thermalization=('6' '9' '240' '127' '8' '10' '200' '250' '5' '7' '70' '60' );

#declare -a list_data= (ls ising_*.dat) 
declare -a list_data=('ising_B0_b0.1.dat' \
      	'ising_B0_b0.3.dat' \
       	'ising_B0_b0.5.dat'\
      	'ising_B0_b0.7.dat'\
      	'ising_B0.01_b0.1.dat' \
       	'ising_B0.01_b0.3.dat' \
	'ising_B0.01_b0.5.dat'\
       	'ising_B0.01_b0.7.dat'\
       	'ising_B0.03_b0.1.dat'\
       	'ising_B0.03_b0.3.dat'\
       	'ising_B0.03_b0.5.dat'\
       	'ising_B0.03_b0.7.dat');

for i in $index
do
	echo_cmnd="${bin} -t ${v_thermalization[i]}  -f ${list_data[i]} > ac_${list_data[i]}"
	echo ${echo_cmnd}
	${bin} -t ${v_thermalization[i]} -f ${list_data[i]} > ac_${list_data[i]}
done
