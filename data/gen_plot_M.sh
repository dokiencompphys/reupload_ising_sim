#!/bin/bash
#extracts and gathers core data (run after gen_data2.sh)


v_index_beta="0 1 2 3"
v_index_B="0 1 2 3 4 5 6 7 8  9 10 11"

#hard coded
declare -a v_beta=('0.1' '0.3' '0.5' '0.7' '0.1' '0.3' '0.5' '0.7' '0.1' '0.3' '0.5' '0.7')
declare -a v_B=('0.00'  '0.00'  '0.00' '0.00'  '0.01'  '0.01' '0.01'  '0.01'  '0.03' '0.03'  '0.03'  '0.03')
declare -a v_list_data=(ls ac_ising_*.dat)

#unfortunately amk or sed cant read 'ls'
declare -a list_data=(
'ac_ising_B0_b0.1.dat' 
'ac_ising_B0_b0.3.dat' \
'ac_ising_B0_b0.5.dat' \
'ac_ising_B0_b0.7.dat' \
'ac_ising_B0.01_b0.1.dat' \
'ac_ising_B0.01_b0.3.dat' \
'ac_ising_B0.01_b0.5.dat' \
'ac_ising_B0.01_b0.7.dat' \
'ac_ising_B0.03_b0.1.dat' \
'ac_ising_B0.03_b0.3.dat' \
'ac_ising_B0.03_b0.5.dat' \
'ac_ising_B0.03_b0.7.dat')

echo "##Plot Magnetization vs temperature" > plot_M0.dat
echo "##B    beta     config - t    <H>/V    stddev/V    t_ac_E    1/t_ac_E     err<H>/V    <M>/V     stddev/V    t_ac_M    1/t_ac_M    err<M>/V " >>plot_M0.dat
for i in $v_index_B
do
	echo -n  "${v_B[i]}   " >> plot_M0.dat	
	echo -n  "${v_beta[i]}   "  >> plot_M0.dat
	
	sed -n '$p' ${list_data[i]}>> plot_M0.dat 
	sed 's/#//' plot_M0.dat > plot_M.dat
       	
done





