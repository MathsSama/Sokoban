#include "main.h"

niveau_t* nouveau_niveau(int nb_colonnes, int nb_lignes){//initialse la structure niveau_t
  niveau_t* niveau= malloc (sizeof(niveau_t));
  niveau->terrain = malloc(sizeof(char)*(nb_colonnes*nb_lignes));
  niveau->perso = malloc(sizeof(point_t));
  niveau->nb_de_pas=1;
  niveau->nb_colonnes=nb_colonnes;
  niveau->nb_lignes=nb_lignes;
  return niveau;
}
void place_sur_terrain (niveau_t* niveau, int colonne, int ligne, char car){//place sur le plateau le char car
  niveau->terrain[colonne*niveau->nb_lignes+ligne]=car;  
}
char lecture_du_terrain (niveau_t* niveau, int colonne, int ligne){//affiche le char du plateau ce trouvant sur les coordonnée donné
  return  niveau->terrain[colonne*niveau->nb_lignes+ligne];
}
void initialise_le_terrain (niveau_t* niveau){//initialise le terrain avec le char #
  for(int i=0;i<niveau->nb_lignes;i++){
    for(int j=0;j<niveau->nb_colonnes;j++){
      place_sur_terrain (niveau,j, i, '#');      
    }    
  }
}

void initialise_perso(niveau_t* niveau){ //initialise le perso en donnant les coordonée du char @
  for(int i=0;i<niveau->nb_lignes;i++){

    for(int j=0;j<niveau->nb_colonnes;j++){
      if(niveau->terrain[j*niveau->nb_lignes+i] == '@'){
       niveau->perso->ligne = i;
       niveau->perso->colonne = j;
       return ;
     }

   }
 }

}


void affichage_niveau (niveau_t* niveau){ // affiche le niveau

  for(int i=0;i<niveau->nb_lignes;i++){
    for(int j=0;j<niveau->nb_colonnes;j++){
      printf("%c",niveau->terrain[(j*niveau->nb_lignes)+i]);     
    }
    printf("\n");   
  }  
}

//----------------------------Fonction sur Historique de Partie-----------------------------------

  niveau_t* copie_du_niveau (niveau_t* niveau){// fonction qui fait une copie du niveau lors du derniers coup
    niveau_t* copie = nouveau_niveau(niveau->nb_colonnes, niveau->nb_lignes);
    strcpy(copie->terrain,niveau->terrain);
    copie->perso->colonne=niveau->perso->colonne;
    copie->perso->ligne=niveau->perso->ligne;
    copie->nb_de_pas = niveau->nb_de_pas;
    return copie;  
  }

  historique_t* initialise_historique_t(int nb_colonnes, int nb_lignes){//initilaise la stucure historique_t
    historique_t* historique = malloc (sizeof(historique));
    historique-> taille = 1;
    historique->dernier=-1;
    historique->tableau = malloc (sizeof(niveau_t)*5);
    for(int i = 0;i<1;i++){
      historique->tableau[i]=nouveau_niveau(nb_colonnes,nb_lignes);
    }
    
    return historique;
  }


  void allonge_tableau_historique (historique_t* hist){ // fonction qui augmente de 1 la taille de la pile
    printf("ALLONGE TABLEAU\n");
    int taille=hist->taille;
    for(int i = taille;i<taille+1;i++){
      hist->tableau[i]=nouveau_niveau(hist->tableau[0]->nb_colonnes,hist->tableau[0]->nb_lignes);
      hist->taille++;
    }
  
  }

  void racourci_tableau_historique (historique_t* hist){// fonction qui racourci la pile de 1
    printf("RACCOURCI TABLEAU\n");  
    int taille=hist->taille;
    for(int i=taille;i<taille;i++){
      free(hist->tableau[i]);
    }
    
    hist->taille--;
  }


  void sauvegarde_un_coup (historique_t* hist, niveau_t* niveau){//fonction qui ajoute la copie dans l'historique 
    if(hist->dernier==hist->taille-1){
      allonge_tableau_historique(hist);
    }
    hist->tableau[hist->dernier+1] = copie_du_niveau(niveau);
    hist->dernier++;
  }

  niveau_t* coup_precedent (historique_t* hist){  //fonction qui permet de retourner en arrière en retournant l'état du plateau précedent
    niveau_t* tmp = copie_du_niveau(hist->tableau[hist->dernier]);
    hist->tableau[hist->dernier]=NULL;
    hist->dernier--;
    racourci_tableau_historique (hist);
    return tmp;
  }        
  

//----------------------Fonctions du score-----------------------------


tableau_score* initialise_tableau_score(void){//fonction qui initialise le tableau des scores (peu etre faire en sorte de gérer les niveau 0 et 1 (seul le 0 est geré pour le moment))
  tableau_score* tableau = malloc(sizeof(tableau_score));
  tableau->taille = 5; 
  tableau->tab = malloc(sizeof(score_s)*5);
  for(int i =0;i<5;i++){
    if (tableau->tab[i].score == -1)
    {
    tableau->tab[i].nom = "None";
      tableau->tab[i].score = -1;
    }
  }
  return tableau;
}



void remplire_tableau_score (int quel_niveau, tableau_score* tableau){ // remplit le tableau score avec le contenu du fichier lu 
  FILE* fichier;
  int scoretmp;
  char* nomtmp;
  int cpt =0;
  char d1[50];
  sprintf(d1,"./Scores/score_multi_%d",quel_niveau);
  fichier=fopen(d1, "ab+"); //censé creer le fichier s'il n'existe pas
  fichier=fopen(d1,"r");

  while(cpt<tableau->taille){
    nomtmp =malloc(sizeof(char)*25);
    if(fscanf(fichier, "%d %s", &scoretmp, nomtmp)!=2){
      tableau->tab[cpt].nom = "None";
    tableau->tab[cpt].score = -1;
    }else{
    tableau->tab[cpt].nom = nomtmp;
    tableau->tab[cpt].score = scoretmp;
    }
    
    cpt++;
  }
  printf("\n");
  fclose(fichier);
}
  

int lecture_du_score (int quel_niveau, tableau_score* tableau){//fonction qui va lire le meilleur score (fait une segmentaion fault du fait que le tableau soit vide au depart)
  int tmp=-2;
  char* nom=malloc(sizeof(char)*25); 
  tmp=tableau->tab[0].score;
  nom=tableau->tab[0].nom;
  if(tmp==-1){
    printf("Ce niveau n'a pas encore de meilleur score !");
  }else{
    printf("Le meilleur score pour ce niveau est: %d mouvements par %s\n",tmp , nom);
  }  
  return tmp;
}

int pire_score (int quel_niveau, tableau_score* tableau){//fonction qui comme lecture_score , va retourner le pire score 
 if(tableau->tab[4].score==-1){
  return -1;
 }
 return tableau->tab[4].score;
}


int nombre_de_joueurs(int quel_niveau, tableau_score* tableau){//fonction qui va retourner le nombre de joueurs qui sont sur le tableau des scores 
  int res;
  for (int i = 0; i < 5; ++i)
  {
    if (tableau->tab[i].score < 0)
    {
      return i;
    }
    res = i;
  }
  return res;
}

  int trouver_doublon(int quel_niveau, char* nouveauNom, int nouveauScore, tableau_score* tableau){//fonction qui parcourt le tableau des scores et si elle trouve un doublon remplace juste le score de celui-ci
    int i = 0;
    char* nom=malloc(sizeof(char)*25); 
    while(i<tableau->taille){
      nom = tableau->tab[i].nom;
      if (strcmp(nom,nouveauNom)==0  && tableau->tab[i].score > nouveauScore)
      {
        for (int j = i; j < tableau->taille-1; ++j)
        {
          tableau->tab[j].score = tableau->tab[j+1].score;
          tableau->tab[j].nom = tableau->tab[j+1].nom;
        }
        OrdreScore(quel_niveau, nouveauNom, nouveauScore, tableau);
        return i;
      }else if (tableau->tab->nom == nouveauNom  && tableau->tab[i].score <= nouveauScore)
      {
        return -1;
      }
      i++;
    }
    return -1;
  }

  int OrdreScore(int quel_niveau, char* nouveauNom, int nouveauScore, tableau_score* tableau){//fonction qui remet dans l'ordre les scores 
    tableau_score* nouveauTableau = initialise_tableau_score();
    
      if (nouveauScore < tableau->tab[0].score || tableau->tab[0].score==-1)
      {
        printf("GG, tu es en 1er place\n");
        ecriture_du_score(quel_niveau, nouveauScore, nouveauNom, nouveauTableau, 0);
        ecriture_du_score(quel_niveau, tableau->tab[0].score, tableau->tab[0].nom, nouveauTableau, 1);
        ecriture_du_score(quel_niveau, tableau->tab[1].score, tableau->tab[1].nom, nouveauTableau, 2);
        ecriture_du_score(quel_niveau, tableau->tab[2].score, tableau->tab[2].nom, nouveauTableau, 3);
        ecriture_du_score(quel_niveau, tableau->tab[3].score, tableau->tab[3].nom, nouveauTableau, 4);
        tableau->tab = nouveauTableau->tab;
        tableau->taille = nouveauTableau->taille;
        return 1;
      }else if (nouveauScore < tableau->tab[1].score || tableau->tab[1].score==-1)
      {
        printf("GG, tu es en 2eme place\n");
        ecriture_du_score(quel_niveau, tableau->tab[0].score, tableau->tab[0].nom, nouveauTableau, 0);
        ecriture_du_score(quel_niveau, nouveauScore, nouveauNom, nouveauTableau, 1);
        ecriture_du_score(quel_niveau, tableau->tab[1].score, tableau->tab[1].nom, nouveauTableau, 2);
        ecriture_du_score(quel_niveau, tableau->tab[2].score, tableau->tab[2].nom, nouveauTableau, 3);
        ecriture_du_score(quel_niveau, tableau->tab[3].score, tableau->tab[3].nom, nouveauTableau, 4);
        tableau->tab = nouveauTableau->tab;
        tableau->taille = nouveauTableau->taille;
        return 1;
      }else if (nouveauScore < tableau->tab[2].score || tableau->tab[2].score==-1)
      {
        printf("GG, tu es en 3eme place\n");
        ecriture_du_score(quel_niveau, tableau->tab[0].score, tableau->tab[0].nom, nouveauTableau, 0);
        ecriture_du_score(quel_niveau, tableau->tab[1].score, tableau->tab[1].nom, nouveauTableau, 1);
        ecriture_du_score(quel_niveau, nouveauScore, nouveauNom, nouveauTableau, 2);
        ecriture_du_score(quel_niveau, tableau->tab[2].score, tableau->tab[2].nom, nouveauTableau, 3);
        ecriture_du_score(quel_niveau, tableau->tab[3].score, tableau->tab[3].nom, nouveauTableau, 4);
        tableau->tab = nouveauTableau->tab;
        tableau->taille = nouveauTableau->taille;
        return 1;
      }else if (nouveauScore < tableau->tab[3].score || tableau->tab[3].score==-1)
      {
        printf("GG, tu es en 4eme place\n");
        ecriture_du_score(quel_niveau, tableau->tab[0].score, tableau->tab[0].nom, nouveauTableau, 0);
        ecriture_du_score(quel_niveau, tableau->tab[1].score, tableau->tab[1].nom, nouveauTableau, 1);
        ecriture_du_score(quel_niveau, tableau->tab[2].score, tableau->tab[2].nom, nouveauTableau, 2);
        ecriture_du_score(quel_niveau, nouveauScore, nouveauNom, nouveauTableau, 3);
        ecriture_du_score(quel_niveau, tableau->tab[3].score, tableau->tab[3].nom, nouveauTableau, 4);
        tableau->tab = nouveauTableau->tab;
        tableau->taille = nouveauTableau->taille;
        return 1;
      }else if (nouveauScore < tableau->tab[4].score || tableau->tab[4].score==-1)
      {
        printf("GG, tu es en 5eme place\n");
        ecriture_du_score(quel_niveau, tableau->tab[0].score, tableau->tab[0].nom, nouveauTableau, 0);
        ecriture_du_score(quel_niveau, tableau->tab[1].score, tableau->tab[1].nom, nouveauTableau, 1);
        ecriture_du_score(quel_niveau, tableau->tab[2].score, tableau->tab[2].nom, nouveauTableau, 2);
        ecriture_du_score(quel_niveau, tableau->tab[3].score, tableau->tab[3].nom, nouveauTableau, 3);
        ecriture_du_score(quel_niveau, nouveauScore, nouveauNom, nouveauTableau, 4);
        tableau->tab = nouveauTableau->tab;
        tableau->taille = nouveauTableau->taille;
        return 1;
      }else{
        printf("le score est insufisant, il te faut plus de %d pour entrer dans le top 5 \n", tableau->tab[4].score);
        return -1;
      }
  }


  void tri(int quel_niveau, char* nouveauNom, int nouveauScore, tableau_score* tableau){ //fonction qui regroupera toutes les fonction de tri pour pouvoir gérer le score 
    if (trouver_doublon(quel_niveau, nouveauNom, nouveauScore , tableau) != -1)
    {
      printf("Ton score a été mis à jour !\n");
    }else{
      if (OrdreScore(quel_niveau, nouveauNom, nouveauScore, tableau) != -1)
      {
        printf("Bien jouer , tu viens de rentrer dans le top 5 !\n");
      }else{
        int pire = pire_score (quel_niveau, tableau);
        if(pire !=-1){
          printf("Domage ton score n'est pas suffisant pour rejoindre le top 5, le pire score est %d\n", pire);
        }        
      }
    }
  }


  void reset_score (int quel_niveau, int flag){//fonction qui reset le fichier demandé 
  FILE* fichier;
  char d1[50];
  sprintf(d1,"./Scores/score_multi_%d",quel_niveau);
  fichier=fopen(d1,"w");
  fprintf(fichier, " ");
  if(flag==0){
    system("clear"); 
    printf("Score reset for lvl %d done\n",quel_niveau);
  }
  
  fclose(fichier);
  }

void MAJ_du_score (int quel_niveau, tableau_score* tableau){ //remplit le fichier avec le contenu du tableau 
  reset_score(quel_niveau,1);
  FILE* fichier;
  char d1[50];
  sprintf(d1,"./Scores/score_multi_%d",quel_niveau);
  fichier=fopen(d1,"a");
  for (int i = 0; i < 5; ++i)
  {
    fprintf(fichier, "%d %s\n", tableau->tab[i].score,tableau->tab[i].nom);
  }
  fclose(fichier);

}


void ecriture_du_score(int quel_niveau, int score, char* nom, tableau_score* tableau,int idx){ //fonction qui va mettre le score dans le tableau si il y a de la place (plus une fonction test qu'autre chose)
  if (nombre_de_joueurs(quel_niveau, tableau) < 5)
  {
    tableau->tab[idx].score = score;
    tableau->tab[idx].nom = nom;
  }
}

char* nom_du_joueur(void){  //demande le nom du joueur , retourne un char*
  printf("Entrez votre nom/pseudo (8 caractères maximum)\n");
  char tab[26];
  char* tabdef;
  int length=10;
  fgets(tab,25,stdin);
  while(length >8){ 
    fgets(tab,25,stdin);   
    length= strlen(tab);
    tab[length-1]='\0';
    if(length >8){
      printf("pseudo trop long\n");
      printf("%s est ton pseudo \n",tab );
    }
  }
  tabdef = malloc(sizeof(char)*length);
  strcpy(tabdef,tab);
  return tabdef;
}

niveau_t* lecture_du_niveau(int quel_niveau){ // fonction qui lit le niveau du fichier pour le mettre dans la stucture niveau
  FILE* fichier;
  char d1[50];
  sprintf(d1,"./Niveaux/niveau_%d",quel_niveau);
  fichier=fopen(d1,"r");
  int nb_colonnes;
  int nb_lignes;
  fscanf(fichier,"%d %d",&nb_colonnes ,&nb_lignes);
  char car = getc(fichier);
  car = getc(fichier);
  niveau_t* niveau = nouveau_niveau(nb_colonnes, nb_lignes); 
  for(int i=0;car!=EOF&&i<niveau->nb_lignes;i++){    
    for(int j=0;car!='\n'&&j<niveau->nb_colonnes;j++){    
      niveau->terrain[(j*niveau->nb_lignes)+i]=car;  
      car = getc(fichier); 
    }
    if(car!='\n'){
      while(car!='\n'&&car!=EOF){
  car = getc(fichier); //pour skip si jamais il y a des espaces en trop
      }
    }
    car = getc(fichier);
  }

  fclose(fichier);
  initialise_perso(niveau);
  return niveau;
}


int niveau_termine (niveau_t* niveau){ // dit si le niveau est terminé 
  for(int i=0;i<niveau->nb_lignes;i++){
    for(int j=0;j<niveau->nb_colonnes;j++){
      if(niveau->terrain[(j*niveau->nb_lignes)+i]=='$'){
        return 0;
      }   
    }      
  }
  return 1;
}

//--------------------------Fonctions de Libération de mémoire ----------------------------------

void liberation_du_niveau (niveau_t* niveau){ // libeartion de la mémoire de niveau
  free(niveau->terrain);
  free(niveau->perso);
  free(niveau);  
}

void liberation_du_tableau_des_score (tableau_score* tableau){ // libeartion de la mémoire du tableau de score
  free(tableau->tab);
  free(tableau);
}

void liberation_historique(historique_t* historique){ // libeartion de la mémoire de l'historique 
  free(historique->tableau);
  free(historique);
}


//-----------------------------Fonctions de déplacement----------------------------------------

char entree_du_joueur (void){ // demande au joueur de rentrer une touche pour pouvoir exécuter une action 
  char c = '^';
  while(c != 'a'&&c != 'z' && c != 'q' && c !='s' && c != 'd'){
    scanf("%c", &c);
  }
  return c;
}


int deplacement (niveau_t* niveau,char direction){ // fonction qui prend en paramètre l'entrée du joueur pour exécuter le déplacement 
  if (direction == 'z'){

    return verifDeplacement(-1,0, niveau);
  }else if(direction == 's'){

   return verifDeplacement(1,0,niveau);    
  }else if(direction == 'q'){

    return verifDeplacement(0,-1,niveau);

  }else if(direction == 'd'){
    return verifDeplacement(0,1,niveau);
   }
   return -1;
}



int verifDeplacement(int x, int y, niveau_t* niveau){
    if (!(lecture_du_terrain (niveau, niveau->perso->colonne + y, niveau->perso->ligne + x) == '#')){//si c'est pâs un mur

      if (lecture_du_terrain (niveau, niveau->perso->colonne + y, niveau->perso->ligne + x) == '.'){//si c'est une place de rangement 

        if(lecture_du_terrain (niveau, niveau->perso->colonne, niveau->perso->ligne) == '+'){ // on est sur une place de rangement
          place_sur_terrain (niveau, niveau->perso->colonne + y, niveau->perso->ligne + x, '+');
          place_sur_terrain (niveau, niveau->perso->colonne, niveau->perso->ligne, '.');
          niveau->perso->ligne+=x;
          niveau->perso->colonne+=y;
          return 0;
        }else{
          place_sur_terrain (niveau, niveau->perso->colonne + y, niveau->perso->ligne + x, '+');
          place_sur_terrain (niveau, niveau->perso->colonne, niveau->perso->ligne, ' ');
          niveau->perso->ligne +=x;
          niveau->perso->colonne +=y;
          return 0;
        }
      }else if (lecture_du_terrain (niveau, niveau->perso->colonne + y, niveau->perso->ligne + x) == '$'){//c'est une caisse

        if (!(lecture_du_terrain (niveau, niveau->perso->colonne + y+y, niveau->perso->ligne +   x + x) == '#')&&!(lecture_du_terrain (niveau, niveau->perso->colonne+y+y, niveau->perso->ligne+x+x) == '$')){//c'est pas un mur après la caisse

          if (lecture_du_terrain (niveau, niveau->perso->colonne + y + y, niveau->perso->ligne + x + x) == '.'){//si c'est une place de rangement après la caisse

            place_sur_terrain (niveau, niveau->perso->colonne + y + y, niveau->perso->ligne  +x +x, '*');
           place_sur_terrain (niveau, niveau->perso->colonne + y, niveau->perso->ligne + x, '@');
             place_sur_terrain (niveau, niveau->perso->colonne, niveau->perso->ligne, ' ');
        niveau->perso->ligne +=x;
        niveau->perso->colonne +=y;
        return 0;
          }else{//si c'est du vide 
            if(lecture_du_terrain (niveau, niveau->perso->colonne, niveau->perso->ligne) == '+'){//si on est sur un rangement et qu'a près c'est une caisse et ensuite du vide
              place_sur_terrain (niveau, niveau->perso->colonne+y+y, niveau->perso->ligne+x+x, '$');
        place_sur_terrain (niveau, niveau->perso->colonne + y, niveau->perso->ligne + x, '@');
        place_sur_terrain (niveau, niveau->perso->colonne, niveau->perso->ligne, '.');
        niveau->perso->ligne +=x;
        niveau->perso->colonne +=y;
        return 0;
      }else{
        place_sur_terrain (niveau, niveau->perso->colonne+y+y, niveau->perso->ligne+x+x, '$');
        place_sur_terrain (niveau, niveau->perso->colonne + y, niveau->perso->ligne+x, '@');
        place_sur_terrain (niveau, niveau->perso->colonne, niveau->perso->ligne, ' ');
        niveau->perso->ligne +=x;
        niveau->perso->colonne +=y;
        return 0;
      }
    }
  }
      }else if (lecture_du_terrain (niveau, niveau->perso->colonne + y, niveau->perso->ligne + x) == '*'){//c'est une caisse sur un rangement

        if (!(lecture_du_terrain (niveau, niveau->perso->colonne + y + y, niveau->perso->ligne + x + x) == '#')){//c'est pas un mur

          if (lecture_du_terrain (niveau, niveau->perso->colonne + y + y, niveau->perso->ligne + x + x) == '.'){//si c'est une place de rangement 

            if (lecture_du_terrain (niveau, niveau->perso->colonne, niveau->perso->ligne) == '+'){ // si le joueur est sur une place de rangement
        place_sur_terrain (niveau, niveau->perso->colonne + y + y, niveau->perso->ligne + x + x, '*');
        place_sur_terrain (niveau, niveau->perso->colonne + y, niveau->perso->ligne + x, '+');
        place_sur_terrain (niveau, niveau->perso->colonne, niveau->perso->ligne, '.');
        niveau->perso->ligne +=x;
        niveau->perso->colonne +=y;
        return 0;
      }else{
        place_sur_terrain (niveau, niveau->perso->colonne + y + y, niveau->perso->ligne + x + x, '*');
        place_sur_terrain (niveau, niveau->perso->colonne + y, niveau->perso->ligne + x, '+');
        place_sur_terrain (niveau, niveau->perso->colonne, niveau->perso->ligne, ' ');
        niveau->perso->ligne +=x;
        niveau->perso->colonne+=y;
        return 0;
      }

          }else{//si c'est du vide 
      place_sur_terrain (niveau, niveau->perso->colonne+y+y, niveau->perso->ligne+x+x, '$');
      place_sur_terrain (niveau, niveau->perso->colonne+y, niveau->perso->ligne+x, '+');
      place_sur_terrain (niveau, niveau->perso->colonne, niveau->perso->ligne, ' ');
      niveau->perso->ligne+=x;
      niveau->perso->colonne+=y;
      return 0;
    }
  }else{       
    printf("déplacement impossible\n");
    return -1;
  }
      }else{
  if(lecture_du_terrain (niveau, niveau->perso->colonne, niveau->perso->ligne) == '+'){         
    place_sur_terrain (niveau, niveau->perso->colonne+y, niveau->perso->ligne+x, '@');
    place_sur_terrain (niveau, niveau->perso->colonne, niveau->perso->ligne, '.');
    niveau->perso->ligne+=x;
    niveau->perso->colonne+=y;
    return 0;
  }else{
    place_sur_terrain (niveau, niveau->perso->colonne+y, niveau->perso->ligne+x, '@');
    place_sur_terrain (niveau, niveau->perso->colonne, niveau->perso->ligne, ' ');
    niveau->perso->ligne+=x;
    niveau->perso->colonne+=y;
    return 0;
  }

      }
    }else{
      printf("déplacement impossible\n");
      return -1;
    }
    return-1;
}