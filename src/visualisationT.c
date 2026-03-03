#include "visualisationT.h"

//sert à mettre à jours les données de températures 
void visualisationT(temp_t myTemp)
{
    char etat[10]; 
    
    // On teste l'existance du fichier .verrouData
    FILE *fichier_verrou = fopen(".verrouData","r");
    
    if (fichier_verrou!=NULL){ 
        printf("Impossible d'ouvrir le fichier data.txt, une utilisation est en cours\n"); 
        fclose(fichier_verrou);
        return; 
    }

    //Si verrourData n'existe pas : la voie est libre
    else {
        //création d'un fichier verrouData
        FILE *fichier_creation_verrou = fopen(".verrouData","w");
        fclose(fichier_creation_verrou);

        //Lecture de data.txt pour récup l'état 
        FILE *fichier_r = fopen("data.txt","r");

        if (fichier_r == NULL){
            printf("Erreur d'ouverture du fichier data.txt en mode r\n");
            remove(".verrouData");
			return;
        }
        else {
            fscanf(fichier_r,"%9s",etat);
            fclose(fichier_r);
        }
        
        //Ré-écriture complète de data.txt

        FILE *fichier_w = fopen("data.txt","w+");

        if (fichier_w == NULL){
            printf("Erreur d'ouverture du fichier data.txt en mode w\n");
            remove(".verrouData");
			return;
        }
        else {            
            fprintf(fichier_w,"%s\n",etat); // true/false

            float t1 = myTemp.interieure;
            fprintf(fichier_w, "%f\n", t1);// écriture temp1          
            
            float t2 = myTemp.exterieure;
            fprintf(fichier_w, "%f\n",t2); // écriture temp2
            
            fclose(fichier_w);
            remove(".verrouData");

            
            }

        }

    }
