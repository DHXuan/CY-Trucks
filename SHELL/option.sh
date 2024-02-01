#!/bin/bash

file_path=$(find .. -iname "data.csv")
Images_path=$(find . -type d -name "Images" -print -quit)
Temps_path=$(find . -type d -name "Temps" -print -quit)
traitementD1_path=$(find . -iname "traitementD1.sh" -print -quit)
traitementD2_path=$(find . -iname "traitementD2.sh" -print -quit)
traitementL_path=$(find . -iname "traitementL.sh" -print -quit)
traitementS_path=$(find . -iname "traitementS.sh" -print -quit)
traitementT_path=$(find . -iname "traitementT.sh" -print -quit)

chmod +x option.sh

#verifie que je passe un argument
if [ $# -eq 0 ]; then
    echo "Aucun argument fourni"
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