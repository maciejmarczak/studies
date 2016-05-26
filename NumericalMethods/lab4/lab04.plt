set key nobox above vertical maxrows 1

#set xrange [-7.0:7.0]
#set yrange [0.0:3.0]

set xlabel 'x'
set ylabel 'y'

set format y "%2.1f"
set format x "%2.1f"

plot 'wyniki.txt' u 1:2 title 'f(x)' w lines lt rgb "#03A8FF", \
	 'wyniki.txt' u 1:3 title 'Wielomian Newtona' w lines lt rgb "#FF0000", \
	 'wyniki.txt' u 1:4 title 'Wielomian Lagrangea' w lines lt rgb "#009900", \
	 'nodes.txt' u 1:2 title 'Węzły interpolacji' w points ps 1 pt 7 lt rgb "#000000"

pause -1
