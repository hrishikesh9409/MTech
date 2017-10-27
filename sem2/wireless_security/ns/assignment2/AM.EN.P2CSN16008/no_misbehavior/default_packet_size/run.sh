#!/bin/bash
gnuplot -persist <<-EOFMarker
    set title "NS 2 - No Misbehaviour - Throughput Graph with default packet size" font ",14" textcolor rgbcolor "royalblue"
    set timefmt "%y/%m/%d"
    set xlabel "Simulation Time (sec)"
    set ylabel "Throughput (bits received)"
    set pointsize 1
    plot "plot2.dat" using 1:2 with lines title "default packet size"

EOFMarker
