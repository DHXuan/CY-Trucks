#!/bin/bash
start_time=$(date +%s.%s)
cut -d';' -f1,6 data.csv | sort -n -t';' -k1 | uniq | awk -F';' '{print $2 " " $3}' | sort -d -t';' -k2 > trajets.txt
cut -d';' -f2 trajets.txt | sort | uniq -c | sort -nr -t';' | head -10 > results.txt 

#creation de l'histogramme mais à la verticale avec gnuplot

gnuplot -persist <<EOF
set terminal pngcairo enhanced font 'arial,10' size 700, 600 
set output
set title
set xlabel
set ylabel
set style fill solid
set xtics nomirror
set ytics nomirror
set yrange [-1:10]

#inverser l'axe des y pour placer le plus de trajets en haut 


#charger les donnees depuis le fichier 
plot 'results.txt' using (\$1*0.5):0:(\$1*0.5):(0.4):yticlabels with boxes lc rgb "skyblue" notitle
EOF

end_time=$(date +%s.%s)
elapsed_time=$(echo "$end_time - $start_time" | bc)
echo "Temps écoulé : $elapsed_time secondes"
