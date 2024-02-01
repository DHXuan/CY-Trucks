#include <stdio.h>

#include <stdlib.h>

typedef struct chainon {
  int routeID;
  float distance;
  struct chainon * next;
}
Chainon;

Chainon * creationChainon(int a, float b) {
  Chainon * c = malloc(sizeof(Chainon));
  if (c == NULL) {
    exit(5);
  }
  c->routeID = a;
  c->distance = b;
  c->next = NULL;
  return c;
}

Chainon * insertFin(Chainon * pliste, int a, float b){
  Chainon * n = creationChainon(a, b);
  Chainon * p1;
  if (pliste == NULL) {
    return n;
  }
  p1 = pliste;
  while (p1->next != NULL) {
    p1 = p1->next;
  }
  p1->next = n;
  return pliste;
}

Chainon* insertDebut(Chainon* pliste, int a, float b){
  Chainon * n = creationChainon(a, b);
  
  if (pliste == NULL) {
    return n;
  }
  n->next = pliste;
  pliste = n;
  
  return pliste;
}
  
//fonction permettant de savoir le maximum
float MaxiStep(float a, float b){
  if (a <= b) {
    return b;
  }
  return a;
}
//fonction permettant de savoir le minimum
float MiniStep(float a, float b){
  if (a >= b) {
    return b;
  }
  return a;
}

//permet de faire la somme des distances

/*float somme (float ){
  float result=0;
  for (int i=0; i<25; i++){
    result = result + score[2*i+1];
}
  return result;
}*/


int main() {

  float max = 0;
  float min = 100000000; //on initialise le min à une valeur très grande pour pouvoir comparer avec les valeurs des distances
  float som = 0;
  float moy;
  float diff;
  int a;
  float b;

  FILE * fichier = NULL;
  Chainon * pliste = NULL;

  printf("start\n");

  //ouverture du fichier
  fichier = fopen("home/temp/donnees.txt", "r");

  //on récurèpe les données du fichiers et on les insère dans une liste chainée
  while (fscanf(fichier, "%d", &a) == 1) {
    //printf("a=%d\n", a);

    fscanf(fichier, "%f", &b);

    //printf("b=%f\n", b);
    pliste = insertDebut(pliste, a, b);

  }

  if (pliste == NULL) {
    printf("Liste chainée vide \n");
  } else {
    printf("ok\n");
  }

  FILE * fichier2;
  // Ouvre le fichier.txt en mode écriture = fichier dans lequel on écrit
  fichier2 = fopen("home/temp/StatsTrajets.txt", "a");
  // ordre des données : Route ID, distance minimum, distance maximum, distance moyenne,

  // Vérifie si le fichier a été ouvert avec succès
  if (fichier2 == NULL) {
    printf("Impossible d'ouvrir le fichier.\n");
    return 1;
  }

  //phase traitemanet : on calcule le maximum, le minimum, la somme des distances et la moyennede chaque route ID dans la liste chainée
  
  Chainon * p1;
  
  
  //on compare initialise le min, le max, la som aux valeurs de la première étape du premier trajet de la liste
  p1 = pliste;
  int count = 0;
  max = MaxiStep(max, p1->distance);
  min = MiniStep(min, p1->distance);
  som = som + p1->distance;
  count += 1;
      
  while (p1 && p1->next != NULL){
    while (p1->next == NULL || p1->routeID == p1->next->routeID) {
       if (p1->next == NULL){
        break;
        }
      p1 = p1->next; // on passe au suivant seulement si le Route ID est le même
      max = MaxiStep(max, p1->distance);//on met à jour le max en fonction de la valeur ajoutée
      min = MiniStep(min, p1->distance);//on met à jour le min en fonction de la valeur ajoutée
      som = som + p1->distance; // on met à jour la somme totale
      count += 1; // on augmente d'1 le nombre de step de la route ID
      }
      
    moy = som / count; // Calcul moyenne
    
    diff = max - min; // Calcul de la diff

    // Écriture dans le fichier

    fprintf(fichier2, "%d ", p1->routeID);
    fprintf(fichier2, "%f ", min);
    fprintf(fichier2, "%f ", max);
    fprintf(fichier2, "%f ", moy);
    fprintf(fichier2, "%f\n", diff);
    
    p1 = p1->next; // on passse au suivant 
    
    //on initialise le max le min aux valeurs de la première étape du trajet suivant si seulement il existe un suivant
    if (p1!=NULL){
    
    max = p1->distance;
    min = p1->distance;
    som = p1->distance;
    count = 1;
    }
  } 

  // Ferme les fichiers
  fclose(fichier);
  fclose(fichier2);

  return 0; // Fin du programme
}
