#!/bin/bash

AP=5
client=1
seed=100
k=1
r=5

echo -n > data.temp
echo -n > plot.dat

for (( i = 1; i <= 10; i++ ))      ### Outer for loop ###
do

    for (( j = 1 ; j <= 20; j++ )) ### Inner for loop ###
    do
          ns 3.tcl $AP $client $r -seed $seed
          awk -f latency.awk trace.tr >> data.temp
          seed=$((seed + 100))
    done
    echo -n "$k " >> plot.dat
    python AVG.py >> plot.dat
    echo -n > data.temp
    ((client++))
    ((k++))
    r=$((r + 10))
    seed=100
  echo "" #### print the new line ###
done

echo "ALL DONE!!"

gnuplot -persist <<-EOFMarker
    set title "NS 2 - Question 3" font ",14" textcolor rgbcolor "royalblue"
    set timefmt "%y/%m/%d"
    set xlabel "Varying Number of Nodes Per Access Point(Varying Traffic Source)"
    set ylabel "Latency"
    set pointsize 1
    plot "plot.dat" using 1:2 with lines
EOFMarker