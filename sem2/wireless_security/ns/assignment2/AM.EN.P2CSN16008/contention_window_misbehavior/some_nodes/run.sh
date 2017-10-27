#!/bin/bash
gnuplot -persist <<-EOFMarker
    set title "NS 2 - Contention Window Misbehaviour - Throughput Graph with varying packet size and 50% nodes misbehaving" font ",14" textcolor rgbcolor "royalblue"
    set timefmt "%y/%m/%d"
    set xlabel "Simulation Time (sec)"
    set ylabel "Throughput (bits received)"
    set pointsize 1
    plot "p1.dat" using 1:2 with lines title "packet size 300", \
         "p2.dat" using 1:2 with lines title "packet size 500", \
         "p3.dat" using 1:2 with lines title "packet size 900", \
         "p4.dat" using 1:2 with lines title "packet size 1200", \
         "p5.dat" using 1:2 with lines title "packet size 1500"

EOFMarker
