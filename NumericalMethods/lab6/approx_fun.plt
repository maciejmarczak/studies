set key nobox above vertical maxrows 1

#set xrange [-6.0:6.0]
#set yrange [0.0:3.0]

set xlabel 'x'
set ylabel 'y'

set format y "%2.1f"
set format x "%2.1f"

plot 'results.txt' u 1:3 title 'f(x)' w lines lt rgb "#03A8FF", \
	 'results.txt' u 1:2 title 'f(x) approximation' w lines lt rgb "#800000", \
	 'nodes.txt' u 1:2 title 'f(x) points' w points ps 1 pt 7 lt rgb "#000000"

pause -1