#!/bin/bash
chmod 777 T.sh
gcc -o xx TE1.c
gcc -o nn TE2.c 

start_time=$(date +%s.%s)

cut -f1,3 -d";" data.csv | tail -n +2 > /temps/departs.txt
cut -f1,4 -d";" data.csv | tail -n +2 > /temps/arrivees.txt

./xx
head -10 /temps/T_Results.txt > /Resultats/10T_Results.txt
./nn

gnuplot<<EOF
set terminal png font "helvetica, 10" size 800,800
set output "graphT.png"
set style data histogram
set style histogram cluster gap 2 #je laisse une unité d'esp entre les barres 
set style fill solid border -1 #bordure des barres
set boxwidth 2 # determine la largeur des barres
set key fixed right top vertical Right noreverse noenhanced autotitle nobox #configure la légende du graphique, en haut à droite du graphique un à un, placé sous la précédente
set colorbox vertical origin screen 0.9, 0.2 size screen 0.05, 0.6 front noinvert bdefault #je configure les couleurs de la legende, ainsi que leur emplacement

#inverser l'axe des y pour placer le plus de trajets en haut 
set yrange [*:*] noreverse writeback  #définit automatiquement la plage de donnees, noreverse pour ne pas inverser les données 
set y2range [*:*] noreverse writeback # j'ajoute l'autre plage des données en fonction des villes
set xrange [*:*] noreverse writeback
set xtics rotate by 45 offset -3,-4.5 font "helvetica, 8"
set xtics left 
set lmargin at screen 0.1 #je regle la marge gauche 
set bmargin at screen 0.12

set datafile separator ';'

#titre des etiquettes
set xlabel "Villes" offset char 0, char -4
set ylabel "Nb routes" offset char 1, char 0 #je le decale un peu à droite 
set title "Option -t : Nb routes = f(Villes)"

#charger les donnees depuis le fichier 
plot 'T.txt' using 2:xticlabels(1) title 'Total routes' lc rgb "skyblue", '' u 3 title 'ville départ' lc rgb "#4863A0"
EOF

end_time=$(date +%s.%s)
elapsed_time=$(echo "$end_time - $start_time" | bc)
echo "Temps écoulé : $elapsed_time secondes"

rm /temps/departs.txt
rm /temps/arrivees.txt
rm /temps/T_Results.txt
rm 10T_Results.txt
