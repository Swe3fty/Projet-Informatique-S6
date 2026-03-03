#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include "visualisationC.h"


   void visualisationC(float puissance_f){
      
      //Vérification d'éxistence du verrouData
      FILE *fichier_verrou = fopen(".verrouData","r");
      if (fichier_verrou != NULL){
         printf("Impossible d'ouvrir le fichier data.txt car une lecture ou écriture est en cours \n");
         fclose(fichier_verrou);
         return; 
      }

      //Pas de fichier verrou -> on peut lire/modifier data.txt
      else {

         //création d'un fichier verrou
         FILE *fichier_creation_verrou = fopen(".verrouData","w");
         fclose(fichier_creation_verrou);   
         

         //Lecture du fichier data en mode read pour recup les températures
         FILE *fichier_r= fopen("data.txt","r");
         if (fichier_r == NULL){
            printf("Erreur d'ouverture du fichier data.tx en mode r\n");
            remove(".verrouData");
            return; 
         }
         //récupération des datas dans fichier.txt
         else {
            char etat[30] = ""; 
            char t1[30] = ""; 
            char t2[30] = "";
         
            fgets(etat, sizeof etat, fichier_r);
            fgets(t1, sizeof t1, fichier_r); 
            fgets(t2, sizeof t2, fichier_r);

            fclose(fichier_r); 

            //ouverture du fichier data.txt en mode wright
            FILE * fichier_w = fopen("data.txt","w");
            if (fichier_w == NULL){
               printf("Erreur d'ouverture du fichier data.txt en mode w\n");
               remove(".verrouData");
               return; 
               
            }
            else {
            
                  // écriture de "true" dans data si chauffage est sup à 0%
                  if (puissance_f > 0){
                     fputs("true\n", fichier_w);
                  }
                  // écriture de "fasle" dans data si chauffage est = à 0%
                  else {
                     fputs("false\n", fichier_w);
                  }
                  //écriture des datas sauvgardées en debut de code
               fputs(t1, fichier_w);
               fputs(t2, fichier_w);;

               remove(".verrouData");
               fclose(fichier_w);
               }
         }
      }
   }
