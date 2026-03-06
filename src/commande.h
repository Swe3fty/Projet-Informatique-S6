#ifndef COMMANDE_H
#define COMMANDE_H

#include <windows.h>
#include <stdint.h>
#include "ftd2xx.h"
#include "define.h"

void envoie_cmd(DWORD device_index, float cmd);

#endif