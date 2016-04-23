set key nobox above vertical maxrows 1

#set xrange [-6.0:6.0]
#set yrange [0.0:3.0]

set xlabel 'x'
set ylabel 'y'

set format y "%2.1f"
set format x "%2.1f"

plot 'results.txt' u 1:2 title 'f(x)' w lines lt rgb "#03A8FF", \
	 'results.txt' u 1:3 title 'cubic-spline' w lines lt rgb "#FF0000"

pause -1