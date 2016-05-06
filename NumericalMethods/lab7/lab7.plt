set title 'Finite Difference   max_err: 0.0010873
set xrange [-1:3.0708]
plot 'points.txt' u 1:2 title 'f(x)' w lines lt rgb "#03A8FF", \
'org_points.txt' u 1:2 title 'f(x) - dokladne' w lines lt rgb "#800000"
pause -1
