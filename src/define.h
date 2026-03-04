#ifndef DEFINE_H
#define DEFINE_H

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
