#include <stdio.h>
#include <stdlib.h>
#include "regulation.h"

float regulation(int regul, float consigne, float temperature, pid * calcul) {
    float cmd = 0.0;
    float Kp = 1.1;
    float Ki = 0.2;
    float Kd = 0.15;
    float error = consigne - temperature;
    float P = 0.0;
    float I = 0.0;
    float D = 0.0;

	//Regulation Tout ou Rien
    if(regul == 1) {
		if(temperature < consigne){
			cmd = 50.0;
		} else {
			cmd = 0.0;
		}

	//Regulation PID
    } else {
        // Calcul du terme P
        P = Kp * error;

        // Calcul des termes I et D (sauf à la première itération)
        if(calcul->erreur_precedente != -999.0) {
            // Accumulation de l'intégrale (somme des erreurs)
            calcul->integrale += error;
            // Calcul du terme dérivé
            D = Kd * (error - calcul->erreur_precedente);
        }

        // Calcul de la commande : I_term = Ki * integrale accumulée
        I = Ki * calcul->integrale;
        cmd = P + I + D;

        // Saturation linéaire
        if(cmd > 100.0){
 			cmd = 100.0;
		}
        if(cmd < 0.0){
			cmd = 0.0;
		}

        // Mémorisation de l'erreur pour la prochaine itération
        calcul->erreur_precedente = error;
    }
    return cmd;
}

float regulationTest(int regul, float consigne, float* tabT, int nT) {
    float cmd = 0.0;
    pid PID = {-999.0, 0.0};

    for(int i = 0; i < nT; i++) {
        cmd = regulation(regul, consigne, tabT[i], &PID);
    }

    return cmd;
}