#include <unistd.h>
#include "consigne.h"
#include "visualisationT.h"
#include "regulation.h"
#include "visualisationC.h"
#include "simulateur.h"



int main(void){
    
    temp_t temperatures;
    temperatures.interieure = 15.0;
    temperatures.exterieure = 14.0;
    float cmd = 0.0;
    float csgn = 0.0;
    float prev_error = -999.0;
    float I = 0.0;
    struct simParam_s* param_ps = simConstruct(temperatures);

    while(1){
        //Lecture de la consigne
        csgn = consigne(temperatures.interieure);

        //Visualisatoin temp
        visualisationT(temperatures);

        //Calcul de la régulation
        cmd = regulation(2,csgn,temperatures.interieure,&I,&prev_error);

        //Afficher témoin
        visualisationC(cmd);

        //Envoie de la commande au simulateur
        temperatures = simCalc(cmd, param_ps);

        printf("Temperature interieure : %.1f°C, Temperature exterieure : %.1f°C\n", temperatures.interieure, temperatures.exterieure);
        usleep(1000000);

    }
    simDestruct(param_ps);
    

    /*float tabT[] = {15.0, 14.0, 13.0, 12.0, 11.0};
    int taille = sizeof(tabT)/sizeof(float);

    visualisationT(temperatures);
    printf("Valeur consigne : %.1f\n",csgn);

    cmd = regulationTest(2,csgn,tabT,taille);
    printf("La commande du chauffage est : %f\n",cmd);

    visualisationC(csgn);*/

    return 0;
}