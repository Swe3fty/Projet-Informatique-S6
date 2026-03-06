#include "visualisationT.h"

//* Fonction qui sert à récupérer les données contenues dans une structure "myTemp" et met à jour le fichier data.txt en conservant l'état du chauffage

void visualisationT(temp_t myTemp)
{
    char etat[10]; 
    
    // Test de l'existence d'un fichier verrou qui empêcherait la lecture ou l'écriture dans data.txt
    FILE *fichier_verrou = fopen(".verrouData","r");
    
    if (fichier_verrou!=NULL){ 
        fclose(fichier_verrou);
        return; 
    }

    // Écriture dans data.txt à partir d'une structure contenant les températures intérieurs et extérieurs
    else {
        //Création d'un fichier .verrouData pour éviter les conflits de lecture/écriture
        FILE *fichier_creation_verrou = fopen(".verrouData","w");
        fclose(fichier_creation_verrou);

        //Lecture de data.txt pour récupérer l'état du chauffage et le stocker dans une variable "etat"
        FILE *fichier_r = fopen("data.txt","r");

        if (fichier_r == NULL){
            remove(".verrouData");
			return;
        }
        else {
            //Lecture de l'état du chauffage (true/false pour actif/inactif) sur 9 caractères
            fscanf(fichier_r,"%9s",etat);
            fclose(fichier_r);
        }
        
        //Ré-écriture complète de data.txt en écrasant l'ancien contenu et en le remplaçant par les données mises à jours.

        FILE *fichier_w = fopen("data.txt","w+");

        if (fichier_w == NULL){
            remove(".verrouData");
			return;
        }
        else {            
            fprintf(fichier_w,"%s\n",etat); // écriture du true/false stockée précédemment 

            float t1 = myTemp.interieure;
            fprintf(fichier_w, "%f\n", t1); // écriture de la température intérieur                
            
            float t2 = myTemp.exterieure;
            fprintf(fichier_w, "%f\n",t2); // écriture de la température extérieure
            
            fclose(fichier_w);
            remove(".verrouData");

            
            }

        }

    }
