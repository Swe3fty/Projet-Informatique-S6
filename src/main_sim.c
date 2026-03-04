#include <unistd.h>
#include "consigne.h"
#include "visualisationT.h"
#include "regulation.h"
#include "visualisationC.h"
#include "simulateur.h"



int main(void){
    
    temp_t temperatures;
    temperatures.interieure = 15.0;
    temperatures.exterieure = 19.0;
    float cmd = 0.0;
    float csgn = 0.0;
    float prev_error = -999.0;
    float I = 0.0;
    struct simParam_s* param_ps = simConstruct(temperatures);
    pid PID = {-999.0, 0.0};

    while(1){
        //Lecture de la consigne
        csgn = consigne(temperatures.interieure);

        //Visualisatoin temp
        visualisationT(temperatures);

        //Calcul de la régulation
        cmd = regulation(2,csgn,temperatures.interieure,&PID);

        //Afficher témoin
        visualisationC(cmd);

        //Envoie de la commande au simulateur
        temperatures = simCalc(cmd, param_ps);

        printf("Temperature interieure : %.1fC, Temperature exterieure : %.1fC\n", temperatures.interieure, temperatures.exterieure);
        usleep(400000);


    }
    simDestruct(param_ps);


    return 0;
}