#!/usr/bin/gnuplot

set terminal pdfcairo
set output "cvPlot.pdf"

set xlabel "Nb rectangles"
set format x "%.0e"

set ylabel "Erreur"
set format y "%.0e"

set logscale xy
plot "cvPlot.dat" using 1:3 title "Evaluation native"

unset output
! evince cvPlot.pdf &
