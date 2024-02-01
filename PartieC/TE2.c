#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// au préalable 
// $head -10 T_Results.txt > 10T_Results.txt

// ordre des données :  Nombre Total, Nombre Départs, Nom Ville

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

typedef struct AVL {
	char ville[50];
	int total;
	int depart;
	int equilibre;
	struct AVL* fg;
	struct AVL* fd;
} AVL;

typedef AVL* pAVL;

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

pAVL creerAVL(char* ville, int tot, int dep){
	pAVL n = malloc(sizeof(AVL));
	if (n==NULL){
		exit(1);
	}
	strcpy(n->ville,ville);
	n->total = tot;
	n->depart = dep;
	n->fg = NULL;
	n->fd = NULL;
	n->equilibre = 0;
	return n;
}

pAVL ajouterAVL(pAVL a,char *ville, int tot, int dep, int* h){
  if (a == NULL){
    *h = 1;
    return creerAVL(ville, tot, dep);
  }
  else if (strcmp(ville, a->ville) < 0){
    a->fg = ajouterAVL(a->fg, ville, tot, dep, h);
    *h = -*h;
  }
  else if (strcmp(ville, a->ville) > 0){
    a->fd = ajouterAVL(a->fd, ville, tot, dep, h);
  }
  else{ // La ville est dejà dans l'AVL
    *h = 0;
    
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

void parcoursInfixev2(pAVL a, FILE* fichier){ // GRD == dans l'orde alphabetique
  if (a != NULL){
    parcoursInfixev2(a->fg,fichier);
    fprintf(fichier,"%s; %d; %d\n", a->ville, a->total, a->depart);
    parcoursInfixev2(a->fd,fichier);
  }
}	

int main(){

	char ville[50];
	int tot;
	int dep;
	int h = 0;
	pAVL racine = NULL;
	FILE* fichier = fopen("home/temp/10T_Results.txt","r");
	FILE* fichier2 = fopen("home/resultats/T.txt","w");
	
	while (fscanf(fichier,"%d %d %[^\n]", &tot, &dep, ville)==3){
		//printf("%s ", ville);
		//printf("%d ",tot);
		//printf("%d \n",dep);
		racine = ajouterAVL(racine, ville, tot, dep, &h);
	}
	
	parcoursInfixev2(racine,fichier2);
	
	fclose(fichier);
	fclose(fichier2);
	
	return 0;
	
}


	
	 

