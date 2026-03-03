#include "visualisationT.h"

//sert à mettre à jours les données de températures 
void visualisationT(temp_t myTemp)
{
    char etat[10]; 
    
    FILE *fichier_verrou = fopen(".verrouData","r"); 

    // On teste l'existance du fichier .verrouData
    if (fichier_verrou!=NULL){
        
        printf("Impossible d'ouvrir le fichier data.txt, une utilisation est en cours\n"); 
        fclose(fichier_verrou); 
        return; 
    }

    //Si verrourData n'existe pas : la voie est libre
    else {
        
        FILE *fichier = fopen("data.txt","r");
        fscanf(fichier,"%s",etat);
        fclose(fichier); 

        fichier = fopen("data.txt","w+");

        //gestion d'erreur d'ouverture de fichier
        if (fichier == NULL){
            printf("Erreur d'ouverture du fichier data.txt\n");
        }
        
        //écriture dans data.txt
        else {

            FILE *fichier_creation_verrou = fopen(".verrouData","w");
            fclose(fichier_creation_verrou);
            fprintf(fichier,"%s\n",etat);

            float Tint = myTemp.interieure;
            fprintf(fichier, "%f\n", Tint);           
            
            float Text = myTemp.exterieure;
            fprintf(fichier, "%f\n",Text);  

            remove(".verrouData");

            fclose(fichier);
            }

        }

    }
