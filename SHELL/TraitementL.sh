#!/bin/bash
start_time=$(date +%s.%s)
LC_NUMERIC=C awk -F';' '{sum[$1] += $5} END {for (name in sum) printf "%s %f\n", name, sum[name]}' data.csv >somme_trajet.txt
sort -k2,2rn somme_trajet.txt | head -10  > resultat_trajet.txt
sort -rn resultat_trajet.txt > resultat_final.txt

gnuplot<<EOF
set terminal png font "helvetica, 10" size 1000,800
set output "graphDL.png"
set title "Option -l Distance =f(Route)"
set style data histogram
set style histogram cluster gap 2 #je laisse une unité d'esp entre les barres 
set style fill solid border -1 #bordure des barres
set boxwidth 0.5 # determine la largeur des barres

set xrange [*:*] 
set xtics 
set yrange [0:3000]
set ytics nomirror #on configure de telle sorte que on supp les marqueurs du cote droit de laxe des y 

#titre des etiquettes
set xlabel "Route ID"
set ylabel "Distance" 

#charger les donnees depuis le fichier 
plot 'resultat_final.txt' using 2:xticlabels(1) with boxes lc rgb "skyblue" notitle
EOF
end_time=$(date +%s.%s)
elapsed_time=$(echo "$end_time - $start_time" | bc)
echo "Temps écoulé : $elapsed_time secondes"
