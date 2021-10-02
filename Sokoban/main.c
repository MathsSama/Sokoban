#include "main.h"

int main(void){
  system("clear");
 // niveau_t* niveauTest=nouveau_niveau(10,4,0);
  //initialise_le_terrain(niveauTest);
  //affichage_niveau(niveauTest);

char choise=' ';
tableau_score* tableauscore = initialise_tableau_score();
printf("Salut toi, prêt à défier Sokoban ?\n");
  while(choise!='3'){
    printf("1. Jouer\n2. Rêgles\n3. Quitter\n");
    choise=getchar();
    while(choise !='1'&&choise!='2'&&choise!='3'&&choise!='9'){
      system("clear");
      printf("Choix incorrecte, choisi entre 1,2 ou 3\n");
      printf("1. Jouer\n2. Rêgles\n3. Quitter\n");
      choise=getchar();
    }
    if(choise=='1'){
      system("clear");
      int lvl=-1;
      while(lvl!=42){
          printf("Quel niveau ? (0 , 1 ou 2) 42 pour quitter: ");
          scanf("%d",&lvl);
          while(getchar() != '\n'); //pour éviter les bug si le scanf a un caractère
          if(lvl!=42){
            if (lvl != 2 && lvl != 1 && lvl != 0)
            {
              printf("pas possible\n");
            }else{
               niveau_t* niveau=lecture_du_niveau(lvl);
               historique_t* historique= initialise_historique_t(niveau->nb_colonnes, niveau->nb_lignes);
            //system("clear");
            affichage_niveau(niveau);
            char c=' ';
            while(!(niveau_termine(niveau)==1)){
              c = entree_du_joueur ();     
                
              if(c=='a'){
                if( historique->dernier==0){
                 printf("Impossible de revenir en arrière\n");
                }else{
                  niveau = copie_du_niveau(coup_precedent(historique));
                  
                }
                
              }else{                                             
               niveau->nb_de_pas++;               
                  sauvegarde_un_coup(historique, niveau);                 
               if(deplacement(niveau, c)==-1){
                historique->dernier--;
               }  

              }              
              //ToDOHere :Copie du niveau dans la pile d'historique
                system("clear");
                //printf("id dernier : %d | taille %d\n",historique->dernier,historique->taille);  
              affichage_niveau(niveau);             
            }
            remplire_tableau_score(lvl, tableauscore);
            //system("clear");
            printf("GJ, tu as réussi, tu as fait ce niveau avec %d mouvements\n",niveau->nb_de_pas);

            lecture_du_score (lvl, tableauscore);  

            char* nom = nom_du_joueur();
            tri(lvl, nom, niveau->nb_de_pas, tableauscore);
            MAJ_du_score(lvl, tableauscore);
            liberation_du_niveau(niveau);
            liberation_historique(historique);
            }
           
            
          }
          
          if(lvl==42){
            system("clear");
          }
      }
    }
    if(choise=='2'){
      system("clear");
      printf("Sokoban (ou « garde d’entrepôt ») est un jeu vidéo de réflexion inventé au Japon en 1982. \nDans ce jeu, le joueur doit ranger des caisses sur des cases cibles. \n");
      printf("Il peut se déplacer dans les quatre directions, et pousser (mais pas tirer)une seule caisse à la fois. \nUne fois toutes les caisses rangées, le niveau est réussi. \n");
      printf("L'idéal est de réussir avec le moins de déplacement possible.\n(Merci Mr Baste et Wikipedia)\n\n");

      printf("(Enter pour continuer)\n");
      choise=getchar();
      choise=getchar();
      choise=' ';
      system("clear");
    }
    if(choise=='9'){
      system("clear");
      printf("Admin Mode\n");
      printf("Password: ");
      char mdp[10];
      fgets(mdp,10,stdin);
      fgets(mdp,10,stdin);
      while(strcmp(mdp,"sokoban\n")!=0&&strcmp(mdp,"exit\n")!=0){      
          printf("Nope, not that, type exit for leave the Admin Mode\n");        
         fgets(mdp,10,stdin);
      }  
      if(strcmp(mdp,"sokoban\n")!=0){
       
      }else{
         system("clear");
         printf("Admin Mode\n");
        printf("What do you want to do?\n");        
        while(choise!='4'){
          printf("1. Reset Score\n");
          printf("4. Leave Admin Mode\n");
          choise=getchar();
          if(choise=='1'){
            system("clear");
            printf("Wich level? 0-1\n");
            int lvlreset;
            scanf("%d",&lvlreset);
            while(lvlreset!=0 && lvlreset!=1){
              system("clear");
              printf("Wrong choise\n");
              printf("Wich level? 0-1\n");
              scanf("%d",&lvlreset);
            }
            reset_score (lvlreset,0);
            choise=getchar();  

          }else if(choise=='4'){
            printf("OK\n"); 
          }else{
            
            printf("Wrong choise\n");           
          }        
          
        }
      }     
      choise=getchar();    
      system("clear");
    }   
}
    system("clear");
    

    printf("Merci d'être passé :) \n");

    //liberation_du_niveau(niveauTest);
    return 0;
  
}
