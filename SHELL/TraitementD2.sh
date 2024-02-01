#!/bin/bash
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
LC_NUMERIC=C awk -F';' '{sum[$6] += $5} END {for (name in sum) printf "%s %f\n", name, sum[name]}' data.csv > somme.txt
sort -t' ' -k3 -rn somme.txt > somme2.txt
head -10 somme2.txt | awk '{print $1 " " $2 ";" $3}' > somme3.txt

gnuplot<<EOF
set terminal png font "helvetica, 10"
set output "graphD2.png"
set style data histogram
set view map
set style histogram cluster gap 2 #je laisse une unité d'esp entre les barres 
set style fill solid border -1 #bordure des barres
set boxwidth 0.5 # determine la largeur des barres

#inverser l'axe des y pour placer le plus de trajets en haut 
set yrange [10:-1]
set xrange [0:160000] 
set ytics nomirror #on configure de telle sorte que on supp les marqueurs de graduation du cote droit de laxe des y 
set lmargin at screen 0.25 #je regle la marge gauche 

set datafile separator ';'

#titre des etiquettes
set xlabel "Distance (km)"
set ylabel "Nom des conducteurs" offset char 1, char 0 #je le decale un peu à droite 
set title "Option -d2 : Distance = f(driver)"

#charger les donnees depuis le fichier 
plot 'somme3.txt' using (\$2*0.5):0:(\$2*0.5):(0.4):yticlabels(1) with boxxyerrorbars lc rgb "#4863A0" notitle
EOF
end_time=$(date +%s.%s)
elapsed_time=$(echo "$end_time - $start_time" | bc)
echo "Temps écoulé : $elapsed_time secondes"
