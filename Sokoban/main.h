#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct{

  int colonne;
  int ligne;
  
}point_t;

typedef struct{

  char* terrain;
  point_t* perso; 
  int nb_colonnes;
  int nb_lignes;
  int nb_de_pas;
  
}niveau_t;

typedef struct 
{
	char* nom;
	int score;
}score_s;

typedef struct 
{
	//int lvl;
	int taille;
	score_s* tab;
}tableau_score;

typedef struct 
{
  int taille;
  int dernier;
  niveau_t** tableau; 
}historique_t;




#include "fonctions.h"
