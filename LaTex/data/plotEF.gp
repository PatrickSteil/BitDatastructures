set terminal epslatex
set output "efplot.tex"
set datafile separator ','
set style data histograms
set style fill solid border -1
set tics scale 0
set xtics rotate by 45 right
#set xzeroaxis
set ytics nomirror
set autoscale y
set y2tics
set ylabel "Space [bits]"
set y2label "Time [ms]"
set title "Space and Time"
set key autotitle columnhead
plot 'resultsEF.csv' using 2:xtic(1) title "Space [bits]" axes x1y1, '' using 3:xtic(1) title "Time [ms]" axes x1y2
