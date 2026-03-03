#include "consigne.h"
#include "visualisationT.h"
#include "regulation.h"
#include "visualisationC.h"



int main(void){
    temp_t temperatures = {5,25};
    float csgn = consigne(10.0);
    float cmd = 0.0;
    float tabT[] = {15.0, 14.0, 13.0, 12.0, 11.0};
    float csgn = 19.0;
    int taille = sizeof(tabT)/sizeof(float);

    visualisationT(temperatures);
    printf("Valeur consigne : %.1f\n",csgn);

    cmd = regulationTest(2,csgn,tabT,taille);
    printf("La commande du chauffage est : %f\n",cmd);

    visualisationC(csgn);

    return 0;
}