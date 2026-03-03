#include "consigne.h"

float consigne(float csgn){
   float csgn_f = 0.0;
   char csgn_file_name[] = "consigne.txt";


   //Vérification existence fichier verrouConsigne
   FILE * verrou_file = fopen(".verrouConsigne","r");

   //Fichier verrou inexistant
   if(verrou_file == NULL){
      verrou_file = fopen(".verrouConsigne","w");
      fclose(verrou_file);
      FILE * csgn_file = fopen(csgn_file_name,"r");

      if(csgn_file == NULL){
         printf("Impossible d'ouvrir le fichier consigne.txt\n");
         //remove(".verrouConsigne");
         exit(EXIT_FAILURE);
      } else {

         //Lecture de la consigne
         fscanf(csgn_file, "%f", &csgn_f);
         fclose(csgn_file);
         remove(".verrouConsigne");
         return csgn_f;
      }
   } else {
            printf("Impossible d'ouvrir le fichier %s son utilisation est en cours\n");
            return csgn;
   }
}
