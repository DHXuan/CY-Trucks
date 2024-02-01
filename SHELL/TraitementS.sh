#!/bin/bash

start_time=$(date +%s.%s)
awk -F";" '
#pour chaque ligne à partir de la deuxième puisque jignore len tête 
    NR > 1 {
#je vérifie avec un tableau associatif data si le numéro route id est déjà présent dans le tableau. Si on rencontre le même numéro, cela crée une nvelle ligne avec la nouvelle distance. Sinon, si cest un nouveau numéro, 
#cela crée une nvelle ligne pour ce numero

        if ($1 in data) {
            data[$1] = data[$1] "\n" $1 " " $5
        } else {
            data[$1] = $1 " " $5
            order[++count] = $1
        }
    }

#une fois que le fichier dentrée a été lue, jexécute ces actions avec le modèle FIN 

    END {
        for (i = 1; i <= count; i++) {
            print data[order[i]]
        }
    } ' data.csv > ./temps/donnees.txt
 
./yy

./zz

head -50 ./temps/ResultFinalStats.txt > ./resultats/StatsFinales.txt 

gnuplot<<EOF
set terminal png font "helvetica, 10" size 1000,800
set output "GraphS.png"
set style data histogram
set style histogram cluster gap 2 #je laisse une unité d'esp entre les barres 
set style fill solid border -1 #bordure des barres
set boxwidth 2 # determine la largeur des barres
set key fixed right top vertical Right noreverse noenhanced autotitle nobox #configure la légende du graphique, en haut à droite du graphique un à un, placé sous la précédente
set colorbox vertical origin screen 0.9, 0.2 size screen 0.05, 0.6 front noinvert bdefault #je configure les couleurs de la legende, ainsi que leur emplacement

#inverser l'axe des y pour placer le plus de trajets en haut 
set yrange [*:*] noreverse writeback  #définit automatiquement la plage de donnees, noreverse pour ne pas inverser les données 
set y2range [*:*] noreverse writeback 
set xrange [*:*] noreverse writeback
set xtics rotate by 45 offset -3,-2 font "helvetica, 8"
set xtics left 
set lmargin at screen 0.1 #je regle la marge gauche 
set bmargin at screen 0.10

set datafile separator ';'

#titre des etiquettes
set xlabel "Routes ID" offset char 0, char -3
set ylabel "Distance (km)" offset char 1, char 0 #je le decale un peu à droite 
set title "Option -s : Distance = f(Route)"

#charger les donnees depuis le fichier 
plot './resultats/StatsFinales.txt' using 2:xticlabels(1) with lines title 'Distances Max/Min (Km)' lc rgb "skyblue", '' u 4:xticlabels(1) with lines title "" lc rgb "skyblue", '' u 3 smooth mcs with lines title 'Distance moyenne (km)' lc rgb "#4863A0"
EOF


end_time=$(date +%s.%s)
elapsed_time=$(echo "$end_time - $start_time" | bc)
echo "Temps écoulé : $elapsed_time secondes"

rm ./temps/donnees.txt
rm ./temps/StatsTrajets.txt
rm ./resultats/ResultFinalStats.txt     
