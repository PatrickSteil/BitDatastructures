set terminal epslatex
set output "rmqplot.tex"
set datafile separator ','
set style data histograms
set style fill solid border -1
set tics scale 0
set xzeroaxis
set ytics nomirror
set y2tics
set ylabel "Space [bits]"
set y2label "Time [ms]"
set title "Space and Time"
set key autotitle columnhead
plot 'resultsRMQN.csv' using 2:xtic(1) title "N Space [bits]" axes x1y1, '' using 3:xtic(1) title "N Time [ms]" axes x1y2, 'resultsRMQNLogN.csv' using 2:xtic(1) title "N Log N Space [bits]" axes x1y1, '' using 3:xtic(1) title "N Log N Time [ms]" axes x1y2