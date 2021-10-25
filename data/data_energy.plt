set terminal pngcairo size 1920, 1080 enhanced font 'Verdana,12'
set output 'all_energy.png'
set key bottom right
set grid
set label 1 "MC-Time" at screen 0.5,0.005 center
set label 2 "Energy" at screen 0.005,0.5 rotate by 90 center
set multiplot layout 3,4 title "Energy MC-Time Graphs (N=100, D=2, seed=20200212)"
#list_data=system("ls ising_*.dat")
# for explicit ordering of the plots
list_data="ising_B0_b0.1.dat \
ising_B0_b0.3.dat \
ising_B0_b0.5.dat \
ising_B0_b0.7.dat \
ising_B0.01_b0.1.dat \
ising_B0.01_b0.3.dat \
ising_B0.01_b0.5.dat \
ising_B0.01_b0.7.dat \
ising_B0.03_b0.1.dat \
ising_B0.03_b0.3.dat \
ising_B0.03_b0.5.dat \
ising_B0.03_b0.7.dat"
do for [fn in list_data] {
	sys_cmd='head -n 1 '.fn." | awk -F \', \' \'{print $4 \" \" $5}\'"
	stats fn using 1:2 nooutput
	test_string=sprintf("%s mean: %f", fn, STATS_mean_y)
	print test_string
	TITLE=system(sys_cmd)
	plot [0:50000] fn u 1:2 w l lw 2 title TITLE
	}
unset multiplot
unset output
