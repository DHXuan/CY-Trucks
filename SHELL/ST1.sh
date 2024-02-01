#!/bi<n/bash

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
    } ' data.csv > donnees.txt
       
