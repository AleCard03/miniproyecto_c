set title "PCA Biplot"
set xlabel "PC1"
set ylabel "PC2"
set grid
set size square

# Círculo unitario
set parametric
set trange [0:2*pi]
plot cos(t), sin(t) with lines lc rgb "black" notitle, \
"variables_circle.dat" using 1:2:3:4 with vectors head filled lc rgb "blue" title "Variables", \
"variables_circle_labels.dat" using 1:2:3 with labels offset char 1,1 notitle