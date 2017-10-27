#!/bin/bash
gnuplot -persist <<-EOFMarker
    set title "NS 2 - No Misbehaviour - Delay Graph with varying packet size" font ",14" textcolor rgbcolor "royalblue"
    set timefmt "%y/%m/%d"
    set xlabel "Simulation Time (sec)"
    set ylabel "Delay (sec)"
    set pointsize 1
    plot "plot1.dat" using 1:2 with lines title "packet size 300", \
         "plot2.dat" using 1:2 with lines title "packet size 500", \
         "plot3.dat" using 1:2 with lines title "packet size 900", \
         "plot4.dat" using 1:2 with lines title "packet size 1200", \
         "plot5.dat" using 1:2 with lines title "packet size 1500"

EOFMarker
