#include <stdio.h>
#include <stdlib.h>
#include "regulation.h"


/* Fonction qui prend en argument un type de régulation TOR ou PID, une consigne de température, une température intérieure 
et calcule l'erreur entre les deux pour renvoyer une commande de chauffage bornée entre 0% et 100%
*/
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

	//Régulation P(proportionnel)I(intégrale)D(dérivée)
    } else {
        float P = Kp * erreur;
        float I = 0.0;
        float D = 0.0;
        regulation->integrale += erreur*10;
        
        //Omission de la première itération en raison de l'absence de données antérieures
        if(regulation->iteration > 0) {
            I = (Ki * regulation->integrale);
            D = Kd * (erreur - regulation->erreur_precedente)*0.1;
        }

        //Somme des 3 termes pour déterminer la valeur de la commande
        cmd = P + I + D;
        
        //bornage de la commande entre 0% et 100%
        if(cmd > 100.0){
 			cmd = 100.0;
            regulation->integrale -= erreur;
		}
        if(cmd < 0.0){
			cmd = 0.0;
            regulation->integrale -= erreur;
		}

        //mise à jour des termes nécessaires à l'intégrale / dérivée dans la structure pid
        regulation->erreur_precedente = erreur;
        regulation->iteration++;
    }
    
    return cmd;
}

//Fonction qui teste regulation() sur une série de températures et renvoie la commande finale en fonction du tableau de températures
float regulationTest(int regul, float consigne, float* tabT, int nT) {
    float cmd = 0.0;
    pid PID;
    PID.erreur_precedente = 0.0;
    PID.integrale = 0.0;
    PID.iteration = 0;

    for(int i = 0; i < nT; i++) {
        cmd = regulation(regul, consigne, tabT[i], &PID);
    }

    return cmd;
}