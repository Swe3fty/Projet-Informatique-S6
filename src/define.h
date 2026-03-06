#ifndef DEFINE_H
#define DEFINE_H
#define MAX_DEVICES 4
#define CONSIGNE_MIN 5.0

#include <stdio.h>
#include <stdlib.h>


typedef struct {
	float interieure;
	float exterieure; 
}temp_t;

typedef struct {
    float erreur_precedente;
    float integrale;
	int iteration;
} pid;


#endif
