#!/bin/bash

client=2
seed=100
k=1

echo -n > data.temp
echo -n > plot.dat

for (( i = 1; i <= 10; i++ ))      ### Outer for loop ###
do

    for (( j = 1 ; j <= 20; j++ )) ### Inner for loop ###
    do
          ns 1.tcl $client -seed $seed
          awk -f parse.awk trace.tr >> data.temp
          seed=$((seed + 100))
    done
    echo -n "$k " >> plot.dat
    python AVG.py >> plot.dat
    echo -n > data.temp
    ((client++))
    ((k++))
    seed=100
  echo "" #### print the new line ###
done

echo "ALL DONE!!"

gnuplot -persist <<-EOFMarker
    set title "NS 2 - Question 1" font ",14" textcolor rgbcolor "royalblue"
    set timefmt "%y/%m/%d"
    set xlabel "Number of clients"
    set ylabel "End to end delay (Throughput)"
    set pointsize 1
    plot "plot.dat" using 1:2 with lines
EOFMarker
