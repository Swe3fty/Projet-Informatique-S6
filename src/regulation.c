#include <stdio.h>
#include <stdlib.h>
#include "regulation.h"

float regulation(int regul, float consigne, float temperature, float * I, float* prev_error) {
    float cmd = 0.0;
    float Kp = 1.1;
    float Ki = 0.2;
    float Kd = 0.15;
    float P = 0.0;
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
        P = Kp * (consigne - temperature);

        if(*prev_error != -999.0) {
            *I += Ki * (consigne - temperature);
            D = Kd * ((consigne - temperature) - *prev_error);
        }

        cmd = P + *I + D;
        if(cmd > 100.0){
 			cmd = 100.0;
		}
        if(cmd < 0.0){
			cmd = 0.0;
		}

        *prev_error = (consigne - temperature);
    }
    return cmd;
}

float regulationTest(int regul, float consigne, float* tabT, int nT) {
    float cmd = 100.0;
    float prev_error = -999.0;
	float I = 0.0;

    for(int i = 0; i < nT; i++) {
        cmd = regulation(regul, consigne, tabT[i], &I, &prev_error);
    }

    return cmd;
}