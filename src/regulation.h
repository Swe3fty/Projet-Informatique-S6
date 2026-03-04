#ifndef REGULATION_H
#define REGULATION_H

#include <stdio.h>
#include <stdlib.h>
#include "define.h" 

typedef struct {
    float erreur_precedente;
    float integrale;
} pid;
    
float regulationTest(int regul,float consigne,float* tabT, int nT);
float regulation(int regul, float consigne, float T, pid * calcul);


#endif