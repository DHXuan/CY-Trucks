#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//au prealable sur UNIX
//cut -f1,3 -d";" data.csv | tail -n +2 > departs.txt
//cut -f1,4 -d";" data.csv | tail -n +2 > arrivees.txt

int min2(int a, int b){
  if (a <= b) {
    return a;
  }
  return b;
}

int min3(int a, int b, int c){
  return min2(min2(a, b), c);
}

int max2(int a, int b){
  if (a >= b){
    return a;
  }
  return b;
}

int max3(int a, int b, int c){
  return max2(max2(a, b), c);
}

typedef struct chainon{
  int nb;
  struct chainon *next;
} RID; // liste chainee de route ID

typedef RID *pRID;

pRID creerRID(int val){
  pRID n = malloc(sizeof(RID));
  if (n == NULL){
    exit(1);
  }
  n->nb = val;
  n->next = NULL;
  return n;
}

typedef struct AVL{
  char ville[50];
  pRID pliste; // pointeur vers le premier terme de la liste des RIDs passant par cette ville
  int nbpassage;
  int nbdeparts;
  struct AVL *fg;
  struct AVL *fd;
  int equilibre;
} AVL;

typedef AVL *pAVL;

pAVL creerAVL(char *ville, int val){
  pAVL n = malloc(sizeof(AVL));
  if (n == NULL){
    exit(1);
  }
  strcpy(n->ville, ville);
  n->pliste = creerRID(val);
  n->nbpassage = 1;
  n->nbdeparts = 1;
  n->fg = NULL;
  n->fd = NULL;
  n->equilibre = 0;
  return n;
}

pAVL rotaGauche(pAVL a){
  if (a == NULL){
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
  ///printf("rota gauche faite\n");
  return a;
}
pAVL rotaDroite(pAVL a){
  if (a == NULL)
  {
    exit(3);
  }

  pAVL pivot;
  int eqa, eqp;
  pivot = a->fg; // Utilisez le sous-arbre gauche comme pivot pour la rotation droite
  a->fg = pivot->fd;
  pivot->fd = a;
  eqa = a->equilibre;
  eqp = pivot->equilibre;
  a->equilibre = eqa + max2(-eqp, 0) + 1; // Inverser les operations pour la rotation droite
  pivot->equilibre = max3(eqa + 2, eqa + eqp + 2, eqp + 1);
  a = pivot;
  //printf("rota droite faite\n");
  return a;
}

pAVL doubleRotaDroite(pAVL a){
  a->fg = rotaGauche(a->fg);
  // printf("test5\n");
  return rotaDroite(a);
}

pAVL doubleRotaGauche(pAVL a){
  a->fd = rotaDroite(a->fd);
  return rotaGauche(a);
}

pAVL equilibrerAVL(pAVL a){
  if (a->equilibre >= 2){
    if (a->fd->equilibre >= 0){
      return rotaGauche(a);
    }
    else{
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

// Tri AVL par ordre alphabetique : tout à Gauche = A /Tout à Droite = Z
pAVL ajouterAVLAlphaDepart(pAVL a, char *ville, int RID, int *h){
  pRID p1;
  pAVL n;
  if (a == NULL){
    *h = 1;
    n = creerAVL(ville, RID);
    return n;
  }
  else if (strcmp(ville, a->ville) < 0){
    a->fg = ajouterAVLAlphaDepart(a->fg, ville, RID, h);
    *h = -*h;
  }
  else if (strcmp(ville, a->ville) > 0){
    a->fd = ajouterAVLAlphaDepart(a->fd, ville, RID, h);
  }
  else{ // La ville est dejà dans l'AVL
    *h = 0;
    p1 = a->pliste;
    a->nbdeparts++;
    while (p1->next != NULL){ // parcours de la liste des RIDs
      if (p1->nb == RID){ // si le RID est dejà dans la liste
        return a; // on ne fait rien
      }
      p1 = p1->next;
    }
    p1->next = creerRID(RID); // ajoute le RID à la liste des RID traversant la ville
    a->nbpassage++;           // on ajoute le nb de RID passant par la ville de 1 ssi le RID n'etait pas dejà dans la liste
    
    return a;
  }
  // equilibrage
  if (*h != 0){
    a->equilibre = a->equilibre + *h;
    a = equilibrerAVL(a);
    if (a->equilibre == 0){
      *h = 0;
    }
    else{
      *h = 1;
    }
  }
  return a;
}

// Tri AVL par ordre alphabetique : tout à Gauche = A /Tout à Droite = Z
pAVL ajouterAVLAlphaArrivee(pAVL a, char *ville, int RID, int *h){
  pRID p1;
  pAVL n;
  if (a == NULL){
    *h = 1;
    n = creerAVL(ville, RID);
    
    return n;
  }
  else if (strcmp(ville, a->ville) < 0){
    a->fg = ajouterAVLAlphaArrivee(a->fg, ville, RID, h);
    *h = -*h;
  }
  else if (strcmp(ville, a->ville) > 0){
    a->fd = ajouterAVLAlphaArrivee(a->fd, ville, RID, h);
  }
  else{ // La ville est dejà dans l'AVL
    *h = 0;
    p1 = a->pliste;
    while (p1->next != NULL){ // parcours de la liste des RIDs
      if (p1->nb == RID){ 
      	a->nbdeparts = a->nbdeparts - 1;
        return a; // on ne fait rien
      }
      p1 = p1->next;
    }
    p1->next = creerRID(RID); // ajoute le RID à la liste des RID traversant la ville
    a->nbpassage++;           // on ajoute le nb de RID passant par la ville de 1 ssi le RID n'etait pas dejà dans la liste
    return a;
  }
  // equilibrage
  if (*h != 0){
    a->equilibre = a->equilibre + *h;
    a = equilibrerAVL(a);
    if (a->equilibre == 0){
      *h = 0;
    }
    else{
      *h = 1;
    }
  }
  return a;
}

void parcoursInfixe(pAVL a){ // GRD == dans l'orde alphabetique
  if (a != NULL){
    parcoursInfixe(a->fg);
    printf("%s %d %d\n", a->ville, a->nbpassage, a->nbdeparts);
    parcoursInfixe(a->fd);
  }
}

void parcoursInfixev2(pAVL a, FILE* fichier){ // DRG ordre décroissant 
  if (a != NULL){
    parcoursInfixev2(a->fd,fichier);
    fprintf(fichier,"%d %d %s \n", a->nbpassage, a->nbdeparts, a->ville);
    parcoursInfixev2(a->fg,fichier);
  }
}

pAVL ajouterAVLTrie(pAVL racine, pAVL new, int *h){ // AVL Triage numérique classique
  pAVL n = malloc(sizeof(AVL));
  strcpy(n->ville,new->ville);
  n->nbpassage = new->nbpassage;
  n->nbdeparts = new->nbdeparts;
  n->fd = NULL;
  n->fg = NULL;
  n->equilibre = 0;
  n->pliste = new->pliste;

  if (racine == NULL){
    *h = 1;
    return n;
  }
  else if (racine->nbpassage >= new->nbpassage){
    racine->fg = ajouterAVLTrie(racine->fg, new, h);
    if (racine->fg == racine->fd){
      printf("stop\n");
    }
    *h = -*h;
  }
  else if (racine->nbpassage < new->nbpassage)
  {
    racine->fd = ajouterAVLTrie(racine->fd, new, h);
    if (racine->fg == racine->fd){
      printf("stop\n");
    }
  }
  if (*h != 0){
    racine->equilibre = racine->equilibre + *h;
    racine = equilibrerAVL(racine);
    if (racine->equilibre == 0){
      *h = 0;
    }
    else{
      *h = 1;
    }
  }
  return racine;
}



pAVL creerAVLTrie(pAVL racine, pAVL alpha, int *h){ // alpha = racine de l'AVL trie alphabetiquement
  
  if (alpha != NULL){
    racine = creerAVLTrie(racine, alpha->fg, h);
    racine = ajouterAVLTrie(racine, alpha, h);
    racine = creerAVLTrie(racine, alpha->fd, h);
  }
  return racine;
}

int main(){
  char str[50];
  int RID;
  pAVL racine = NULL;
  int x = 0;
  int i = 0;
  int *h = &x;

  // top10* pliste = NULL;
  pAVL AVLTrie = NULL;

  FILE* fichier = fopen("/temp/departs.txt", "r");
  FILE* fichier2 = fopen("/temp/arrivees.txt", "r");
  FILE* fichier3 = fopen("/temp/T_Results.txt","a");

  if (fichier == NULL){
    printf("Erreur lors de l'ouverture du fichier");
    exit(1);
  }

  while (fscanf(fichier, "%d; %[^\n] ", &RID, str) == 2){
    //printf("RID = %d\n", RID);
    //printf("str = %s\n", str);
    racine = ajouterAVLAlphaDepart(racine, str, RID, h);
  }
  //parcoursInfixe(racine);
  
  while (fscanf(fichier2, "%d; %[^\n] ", &RID, str) == 2){
    //printf("RID = %d\n", RID);
    //printf("str = %s\n", str);
    racine = ajouterAVLAlphaArrivee(racine, str, RID, h);
  }
  
  //parcoursInfixe(racine);
  
  AVLTrie = creerAVLTrie(AVLTrie, racine, &i);
  parcoursInfixev2(AVLTrie,fichier3);

  fclose(fichier);
  fclose(fichier2);
  fclose(fichier3);

  return 0;
}
