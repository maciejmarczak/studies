set pm3d
set hidden3d
set palette rgbformulae 33,13,10
set palette defined (1.5 "green", 2 "blue", 2.5 "red")
set xlabel "Liczba punktów podziału X"
set ylabel "Liczba punktów podziału T"
set zlabel "Dokładność"
splot 'wyniki.txt' u 1:2:3 title 'MRS - Metoda Jawna' w lines

pause -1
