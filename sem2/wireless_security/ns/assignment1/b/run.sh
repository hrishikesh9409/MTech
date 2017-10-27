#!/bin/bash

AP=1
client=1
seed=100
k=1

echo -n > data.temp
echo -n > plot.dat

for (( p  = 1; p <= 10; p++))
do
	for (( i = 1; i <= 10; i++ ))
	do
    	for (( j = 1 ; j <= 20; j++ )) 
    	do
        	ns 2.tcl $AP $client -seed $seed
          	awk -f parse.awk trace.tr >> data.temp
          	seed=$((seed + 100))
    	done
	done
	echo -n "$k " >> plot.dat
    python AVG.py >> plot.dat
    echo -n > data.temp
    ((client++))
    ((AP++))
    ((k++))
    seed=100
  	echo "" 
done

echo "ALL DONE!!"

gnuplot -persist <<-EOFMarker
    set title "NS 2 - Question 1" font ",14" textcolor rgbcolor "royalblue"
    set timefmt "%y/%m/%d"
    set xlabel "Number of Access Points"
    set ylabel "End to end delay (Throughput)"
    set pointsize 1
    plot "plot.dat" using 1:2 with lines
EOFMarker