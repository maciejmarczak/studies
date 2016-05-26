set pm3d
set hidden3d
set palette rgbformulae 33,13,10
set xlabel "x"
set ylabel "T"
splot 'wyniki.txt' u 1:2:3 title 'Metoda Różnic Skończonych 3D' w lines

pause -1
