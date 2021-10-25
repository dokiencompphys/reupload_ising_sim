#!/bin/sh
#generates the data using ising-sim

bin=../executables/ising-sim
v_D=2
v_N=100
v_beta="0.1 0.3 0.5 0.7"
v_B="0 0.01 0.03"
v_seed="20200212"

for B in $v_B
do
for beta in $v_beta
do
	echo_cmnd="${bin} -N ${v_N} -D ${v_D} -s ${v_seed} -b ${beta} -B ${B} \
> ising_B${B}_b${beta}.dat"
	echo ${echo_cmnd}
	${bin} -N ${v_N} -D ${v_D} -s ${v_seed} -b ${beta} -B ${B} > ising_B${B}_b${beta}.dat
done
done
