set title "PCA - Variables"
set xlabel "PC1"
set ylabel "PC2"

set grid
set zeroaxis

# Dibujar flechas y etiquetas
plot \
"variables.dat" using 1:2:3:4 with vectors head filled lc rgb "red" title "Variables", \
"variables_labels.dat" using 1:2:3 with labels offset char 1,1 notitle