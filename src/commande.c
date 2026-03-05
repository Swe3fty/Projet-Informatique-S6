#include "define.h"
#include "commande.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// handle et statut FTDI partagés par commandes et relevés
FT_HANDLE ftHandle = NULL;
FT_STATUS ftStatus = FT_OK;

