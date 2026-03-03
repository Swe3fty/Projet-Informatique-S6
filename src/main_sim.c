#include "consigne.h"
#include "visualisationT.h"
//#include "regulation.h"


int main(void){
    temp_t temperatures = {5,25};
    float csgn = consigne(10.0);
    float cmd = 0.0;

    visualisationT(temperatures);
    printf("Valeur consigne : %.1f",csgn);


    return 0;
}