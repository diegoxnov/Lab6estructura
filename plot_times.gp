set datafile separator ","
set terminal png
set output "execution_times.png"
set title "Tiempos de Ejecución de Operaciones en el Árbol B"
set xlabel "Operación"
set ylabel "Tiempo (segundos)"
set style data histograms
set style fill solid 0.5 border
plot "execution_times.csv" using 2:xtic(1) title "Tiempo de ejecución"
