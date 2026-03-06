#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include "visualisationC.h"

//* Fonction qui sert à récupérer la donnée contenue dans l'argument "puissance_f" et met à jour l'état du chauffage dans data.txt 
   void visualisationC(float puissance_f){
      
      // Test de l'existence d'un fichier verrou qui empêcherait la lecture ou l'écriture dans data.txt
      FILE *fichier_verrou = fopen(".verrouData","r");
      if (fichier_verrou != NULL){
         fclose(fichier_verrou);
         return; 
      }

      // Lecture puis écriture dans data.txt pour mettre à jours les données
      else {

         //Création d'un fichier verrouData pour éviter les conflits de lecture/écriture
         FILE *fichier_creation_verrou = fopen(".verrouData","w");
         fclose(fichier_creation_verrou);   
         

         //Lecture du fichier data en mode lecture pour récupérer les températures et les stocker dans une variable
         FILE *fichier_r= fopen("data.txt","r");
         if (fichier_r == NULL){
            remove(".verrouData");
            return; 
         }
         
         else {
            char etat[30] = ""; 
            char t1[30] = ""; 
            char t2[30] = "";
         
            fgets(etat, sizeof etat, fichier_r); // récupération de l'état du chauffage
            fgets(t1, sizeof t1, fichier_r); // récupération de la température intérieure
            fgets(t2, sizeof t2, fichier_r); // récupération de la température extérieure

            fclose(fichier_r); 

            //ouverture du fichier data.txt en mode écriture pour écraser les données précédentes par les données à jour   
            FILE * fichier_w = fopen("data.txt","w");
            if (fichier_w == NULL){
               remove(".verrouData");
               return; 
               
            }
            else {
            
                  // écriture de "true" dans data si chauffage est sup à 0%
                  if (puissance_f > 0){
                     fputs("true\n", fichier_w);
                  }
                  // écriture de "false" dans data si chauffage est = à 0%
                  else {
                     fputs("false\n", fichier_w);
                  }
                  //écriture des températures intérieures et extérieures stockées en début de code dans data.txt
               fputs(t1, fichier_w);
               fputs(t2, fichier_w);;

               remove(".verrouData");
               fclose(fichier_w);
               }
         }
      }
   }
