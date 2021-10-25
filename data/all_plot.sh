#!/bin/sh
# generates all plots

pltfiles=$(ls -1 *.plt)

for f in ${pltfiles}
do
	gnuplot ${f}
done
