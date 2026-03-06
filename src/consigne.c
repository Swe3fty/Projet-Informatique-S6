#include "consigne.h"

//Fonction qui lit une température de consigne dans le fichier consigne.txt et la renvoie
float consigne(float csgn){ 
   float csgn_f = 0.0;
   char csgn_file_name[] = "consigne.txt";


   // Test de l'existence d'un fichier verrou qui empêcherait la lecture ou l'écriture dans consigne.txt
   FILE * verrou_file = fopen(".verrouConsigne","r");

   //Fichier verrou inexistant
   if(verrou_file == NULL){
      verrou_file = fopen(".verrouConsigne","w");
      fclose(verrou_file);
      FILE * csgn_file = fopen(csgn_file_name,"r");

      //création d'un fichier .verrouConsigne pour protéger et garantir une bonne lecture
      if(csgn_file == NULL){
         exit(EXIT_FAILURE);
         
      } else {

         //Lecture de la consigne dans consigne.txt et stockage dans une variable "csgn_f"
         fscanf(csgn_file, "%f", &csgn_f);
         fclose(csgn_file);
         remove(".verrouConsigne");
         return csgn_f;
      }

   //Impossible d'ouvrir le fichier consigne.txt, son utilisation est en cours
   } else {
            return csgn;
   }
}
