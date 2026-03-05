#ifndef COMMANDE_H
#define COMMANDE_H

#include <windows.h>
#include "ftd2xx.h"

FT_STATUS commande_list_devices(void);
FT_STATUS commande_init(DWORD device_index);
float commande(float cmd);
void commande_close(void);

#endif