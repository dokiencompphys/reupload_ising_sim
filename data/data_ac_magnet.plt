set terminal pngcairo size 1920, 1080 enhanced font 'Verdana,12'
set output 'all_ac_magnet.png'
set key top right
set grid
set yrange [-0.1:1]
set label 1 "lag" at screen 0.5,0.005 center
set label 2 "autocorrelation" at screen 0.005,0.5 rotate by 90 center
set multiplot layout 3,4 title "AC Magnetization MC-Time Graphs (N=100, D=2, seed=20200212)"
#list_data=system("ls ac_ising_*.dat")
# for explicit ordering of the plots
list_data="ac_ising_B0_b0.1.dat \
ac_ising_B0_b0.3.dat \
ac_ising_B0_b0.5.dat \
ac_ising_B0_b0.7.dat \
ac_ising_B0.01_b0.1.dat \
ac_ising_B0.01_b0.3.dat \
ac_ising_B0.01_b0.5.dat \
ac_ising_B0.01_b0.7.dat \
ac_ising_B0.03_b0.1.dat \
ac_ising_B0.03_b0.3.dat \
ac_ising_B0.03_b0.5.dat \
ac_ising_B0.03_b0.7.dat"
f(x) = a*exp(b*x)
do for [fn in list_data] {
	sys_cmd='head -n 1 '.fn." | awk -F \', \' \'{print $4 \" \" $5}\'"
	stats fn using 1:3 nooutput
	test_string=sprintf("%s mean: %f", fn, STATS_mean_y)
	print test_string
	TITLE=system(sys_cmd)
	a = 1
	b = -1
	fit [0:1000] f(x) fn using 1:3 via a,b
	TITLE_FIT=sprintf("f(x) = %f * exp(%f * x)", a, b)
	plot [0:100] fn u 1:3 w l lw 2 title TITLE, f(x) lw 2 title TITLE_FIT
	}
unset multiplot
unset output
