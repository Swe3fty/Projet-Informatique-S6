#include <stdio.h>
#include <stdlib.h>
#include "regulation.h"

float regulation(int regul, float consigne, float temperature, pid * regulation) {
    float cmd = 0.0;
    float Kp = 1.1;
    float Ki = 0.2;
    float Kd = 0.15;
    float erreur = consigne - temperature;

	//Regulation Tout ou Rien
    if(regul == 1) {
		if(temperature < consigne){
			cmd = 50.0;
		} else {
			cmd = 0.0;
		}

	//Regulation PID
    } else {
        float P = Kp * erreur;
        float I = 0.0;
        float D = 0.0;
        
        if(regulation->iteration > 0) {
            regulation->integrale += erreur;
            I = Ki * regulation->integrale;
            D = Kd * (erreur - regulation->erreur_precedente);
        }
        
        cmd = P + I + D;
        
        if(cmd > 100.0){
 			cmd = 100.0;
		}
        if(cmd < 0.0){
			cmd = 0.0;
		}

        regulation->erreur_precedente = erreur;
        regulation->iteration++;
    }
    
    return cmd;
}

float regulationTest(int regul, float consigne, float* tabT, int nT) {
    float cmd = 0.0;
    pid PID;
    PID.erreur_precedente = -999.0;
    PID.integrale = 0.0;
    PID.iteration = 0;

    for(int i = 0; i < nT; i++) {
        cmd = regulation(regul, consigne, tabT[i], &PID);
    }

    return cmd;
}