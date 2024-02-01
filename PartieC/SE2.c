#include <stdio.h>

#include <stdlib.h>

typedef struct chainon {
  int RID; //route ID
  float min; //minimum
  float max; // maximum
  float moy; //moyenne
  float diff; //difference max-min
  struct chainon * next; // pointe vers le prochain chainon
}
Trajet;

Trajet* creationChainon(int a, float b, float c, float d, float e) {
  Trajet* n = malloc(sizeof(Trajet));
  if (n == NULL) {
    exit(5);
  }
  n->RID = a;
  n->min = b;
  n->max = c;
  n->moy = d;
  n->diff = e;
  n->next = NULL;
  return n;
}

Trajet* insert(Trajet* t, int a, float b, float c, float d, float e) {
  Trajet* n;
  Trajet* p1 = t;
  if (t == NULL) {
    return creationChainon(a, b, c, d, e);
  } else {
    
    n = creationChainon(a, b, c, d, e);
    n->next = t;
    return n;
  }
}

float min2(float a, float b) {
  if (a <= b) {
    return a;
  }
  return b;
}

float min3(float a, float b, float c) {
    return min2(min2(a, b), c);
}

float max2(float a, float b) {
  if (a >= b) {
    return a;
  }
  return b;
}

float max3(float a, float b, float c) {
    return max2(max2(a, b), c);
}

typedef struct AVL {
  struct AVL* fg;
  struct AVL* fd;
  Trajet* tr;
  int equilibre;
} AVL;

typedef AVL* pAVL;

pAVL createAVL(Trajet* t) {
  pAVL n = malloc(sizeof(AVL));
  if (n == NULL) {
    exit(1);
  }
  n->fg = NULL;
  n->fd = NULL;
  n->tr = t;
  n->equilibre = 0;
  return n;
}

pAVL rotaGauche(pAVL a) {
  if (a == NULL) {
    exit(3);
  }
  pAVL pivot;
  int eqa, eqp;
  pivot = a->fd;
  a->fd = pivot->fg;
  pivot->fg = a;
  eqa = a->equilibre;
  eqp = pivot->equilibre;
  a->equilibre = eqa - max2(eqp, 0) - 1;
  pivot->equilibre = min3(eqa - 2, eqa + eqp - 2, eqp - 1);
  a = pivot;
   //printf ("rota gauche faite\n");
  return a;

}
pAVL rotaDroite(pAVL a) {
    if (a == NULL) {
        exit(3);
    }

    pAVL pivot;
    int eqa, eqp;
    pivot = a->fg;  // Utilisez le sous-arbre gauche comme pivot pour la rotation droite
    a->fg = pivot->fd;
    pivot->fd = a;
    eqa = a->equilibre;
    eqp = pivot->equilibre;
    a->equilibre = eqa + max2(-eqp, 0) + 1;  // Inverser les opérations pour la rotation droite
    pivot->equilibre = max3(eqa + 2, eqa + eqp + 2, eqp + 1);
    a = pivot;
    //printf ("rota droite faite\n");
    return a;
}

pAVL doubleRotaDroite(pAVL a) {
  a->fg = rotaGauche(a->fg);
  //printf("test5\n");
  return rotaDroite(a);
}

pAVL doubleRotaGauche(pAVL a) {
  a->fd = rotaDroite(a->fd);
  return rotaGauche(a);
}

pAVL equilibrerAVL(pAVL a){
  if (a->equilibre >= 2){
    if (a->fd->equilibre >= 0){
      return rotaGauche(a);
    }
    else {
      return doubleRotaGauche(a);
    }
  }
  else if (a->equilibre <= -2){
    if (a->fg->equilibre <= 0){
      return rotaDroite(a);
    }
    else{
      return doubleRotaDroite(a);
    }
  }
return a;
}

/*pAVL insertionAVL(pAVL a, Trajet* t, int* h) {
  if (a == NULL) {
    *h = 1;
    return createAVL(t);
  } else if (a->tr->diff > t->diff) {
    a->fg = insertionAVL(a->fg, t, h);
    *h = - *h;
  } else if (a->tr->diff < t->diff) {
    a->fd = insertionAVL(a->fd, t, h);
  } else {
    *h = 0;
    return a;
  }
  if (*h != 0) {
    a->equilibre = a->equilibre + *h;
    if (a->equilibre == 0) {
      *h = 0;
    } else {
      *h = 1;
    }
  }
  return a;
}*/

pAVL ajouterAVL(pAVL a, Trajet* t, int* h){

  if (a==NULL){
    *h = 1;
    return createAVL(t);
  }
  else if (a->tr->diff>=t->diff){
    a->fg = ajouterAVL(a->fg, t, h);
    *h = -*h;
  }
  else if (a->tr->diff<t->diff){
    a->fd = ajouterAVL(a->fd, t, h);
  }

  //equilibrage  
  if (*h!=0){
    a->equilibre = a->equilibre + *h;
    a = equilibrerAVL(a);
    if (a->equilibre == 0){
      *h = 0;
    }
    else {
      *h = 1;
    }

  }
  return a;
}

void parcoursDRG(pAVL a, FILE * f) {
  if (a != NULL) {
    parcoursDRG(a->fd, f);
    fprintf(f, "%d;%f;%f;%f;%f\n", a->tr->RID, a->tr->min,  a->tr->moy, a->tr->max, a->tr->diff);
    parcoursDRG(a->fg, f);
  }
}

int main() {
  FILE* fichier1;
  FILE* fichier2;
  fichier1 = fopen("/temp/StatsTrajets.txt", "r"); //ouverture mode lecture du fichier StatsTrajets.txt contenant les stats suivantes sur l'ensemble des trajets : Route ID, Min, Max, Moy, Diff
  fichier2 = fopen("/resultats/ResultFinalStats.txt", "a"); //ouverture en mode ajout "add" d'un nouveau fichier.
  int a; //ID de la route
  float b; // minimum
  float c; // maximum
  float d; // moyenne
  float e; // distance max-min
  Trajet* pliste = NULL;
  Trajet* p1 = NULL;
  int indice = 1;

  // récup des données de StatsTrajets.txt
while (fscanf(fichier1, "%d", &a) == 1) {
      //printf("a = %d\n", a);
      fscanf(fichier1, "%f", &b);
      fscanf(fichier1, "%f", &c);
      fscanf(fichier1, "%f", &d);
      fscanf(fichier1, "%f", &e);
      pliste = insert(pliste, a, b, c, d, e);
  }
  p1 = pliste;

  //Creation d'une liste chainée de structure contenant les stats de l'ensemble des trajets. PS : dernier chainon avec que des 0: RID = 0, min = 0, max = 0, moy = 0, diff = 0.
  fclose(fichier1); //fermeture du fichier StatsTrajets.txt

  // Creation AVL
  pAVL racine = NULL;
  p1 = pliste;
  int x = 0;
  int* h = &x;

  // Creation + Isertion d'un chainon AVL pour chaque trajets contenu dans la liste chainée.
  while (p1->next != NULL) {
    racine = ajouterAVL(racine, p1, h);

    p1 = p1->next;
  }

  // Ecriture des stats des trajets triées dans l'ordre décroissant des différences de distance max-min dans ResultFinalStats.txt via parcours DRG
  parcoursDRG(racine, fichier2);
  fclose(fichier2); //fermeture du fichier ResultFinalStats.txt

  return 0;
}

//ResultFinalStats.txt à cut les 50 premiers avant de gnuplot.

