set title "PCA - Estudiantes"
set xlabel "PC1"
set ylabel "PC2"

set grid

plot "estudiantes.dat" using 1:2:3 with labels point pt 7 offset char 1,1 title ""