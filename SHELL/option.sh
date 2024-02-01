#!/bin/bash

file_path=$(find .. -iname "data.csv") 2>/dev/null
Images_path=$(find . -type d -name "Images" -print -quit) 2>/dev/null
Temps_path=$(find . -type d -name "Temps" -print -quit) 2>/dev/null
traitementD1_path=$(find . -iname "traitementD1.sh" -print -quit) 2>/dev/null
traitementD2_path=$(find . -iname "traitementD2.sh" -print -quit) 2>/dev/null
traitementL_path=$(find . -iname "traitementL.sh" -print -quit) 2>/dev/null
traitementS_path=$(find . -iname "traitementS.sh" -print -quit) 2>/dev/null
traitementT_path=$(find . -iname "traitementT.sh" -print -quit) 2>/dev/null

chmod +x option.sh

if [ -e "$file_path" ]; then
 echo "le fichier data.csv existe"
 echo "$file_path"
fi

if [ -n "$Images_path" ]; then
  echo "Le dossier Images existe et il va etre vidé"
  rm -r "$Images_path"/*
  echo " Le dossier a été vidé"
else
  echo "Le dossier Images n'existe pas et il va être créer"
  mkdir -p "Images"
fi

if [ -n "$Temps_path" ]; then
  echo "Le dossier Temps existe et il va etre vidé"
  rm -r "$Temps_path"/*
  echo " Le dossier a été vidé"
else
  echo "Le dossier Temps n'existe pas et il va être créer"
  mkdir -p "Temps"
fi


#verifie que je passe un argument
if [ $# -eq 0 ]; then
    echo "Aucun argument fourni"
    exit 1
fi*

doccsv="$1"
if[! -f"doccsv"]];then
  echo "Le fichier CSV '$doccsv' est introuvable"
  exit 1
fi
 
#une boucle qui parcourt tous les arguments 
for arg in "$@"
do
  #execute le script correspondant à l'argument
  case "$arg" in
    -d1)
      if [ -n "$traitementD1_path" ]; then
        chmod +x "$traitementD1_path"
        echo "Exécution du script traitementD1.sh"
        "$traitementD1_path"
      else
        echo "Le script traitementD1.sh n'a pas été trouvé."
      fi
      ;;
    -d2)
      if [ -n "$traitementD2_path" ]; then
        chmod +x "$traitementD2_path"
        echo "Exécution du script traitementD2.sh"
        "$traitementD2_path"
      else
        echo "Le script traitementD2.sh n'a pas été trouvé."
      fi
      ;;
    -l)
      if [ -n "$traitementL_path" ]; then
        chmod +x "$traitementL_path"
        echo "Exécution du script traitementL.sh"
        "$traitementL_path"
      else
        echo "Le script traitementL.sh n'a pas été trouvé."
      fi
      ;;
    -h)
      if [ -n "$aide_path" ]; then
        cat explication.txt
        echo "Voici le message d'aide"
      else
        echo "Le script aide.sh n'a pas été trouvé."
      fi
      ;;
    *)
      echo "Option invalide : $arg. Les options valides sont -d1, -d2, -l, -h"
      ;;
  esac
done
