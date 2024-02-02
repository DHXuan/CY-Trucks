#!/bin/bash

start_time=$(date +%s.%s)
cut -d';' -f1,6 data.csv | sort -n -t';' -k1 | uniq | sort -d -t';' -k2 > trajets.txt
cut -d';' -f2 trajets.txt | sort | uniq -c | sort -nr -t';' | head -10 > results.txt 
awk '{print $1 ";" $2 " "$3}' results.txt > combi.txt

#creation de l'histogramme à l'horizontal avec gnuplot

gnuplot<<EOF
set terminal png font "helvetica,9"
set output "~/Images/graphD1.png"
set style data histogram
set view map
set style histogram cluster gap 2 #je laisse une unité d'esp entre les barres 
set style fill solid border -1 #bordure des barres
set boxwidth 0.4 # determine la largeur des barres

#inverser l'axe des y pour placer le plus de trajets en haut 
set yrange [10:-1]
set xrange [0:300] 
set xtics nomirror
set ytics nomirror #on configure de telle sorte que on supp les marqueurs du cote droit de laxe des y 
set lmargin at screen 0.25 #je regle la marge gauche 
set rmargin at screen 0.9

set datafile separator ';'

#titre des etiquettes
set title "option -d1 Nb routes = f(Driver)"
set xlabel "Nombre de routes"
set ylabel "Nom des conducteurs" 

#charger les donnees depuis le fichier 
plot 'combi.txt' using (\$1*0.5):0:(\$1*0.5):(0.4):yticlabels(2) with boxxyerrorbars lc rgb "#4863A0" notitle
EOF
end_time=$(date +%s.%s)
elapsed_time=$(echo "$end_time - $start_time" | bc)
echo "Temps écoulé : $elapsed_time secondes"
