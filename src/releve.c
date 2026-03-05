#include "define.h"
#include "releve.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

//Déjà défini dans commande.c
extern FT_HANDLE ftHandle;
extern FT_STATUS ftStatus;

//Liste de tous les périphériques connectés
FT_STATUS releve_list_devices(void) {

    FT_STATUS status = FT_OK;
    DWORD numDevs = 0;
    
    status = FT_ListDevices(&numDevs, NULL, FT_LIST_NUMBER_ONLY);
    
    if (status != FT_OK) {
        printf("Erreur: impossible de lister les devices FTDI\n");
        return status;
    }
    
    if (numDevs == 0) {
        printf("Aucune périphérique detecte\n");
        return FT_DEVICE_NOT_FOUND;
    }
    
    printf("Nombre de devices FTDI detectes: %ld\n", numDevs);
    
    // Lister les descriptions de chaque device
    for (DWORD i = 0; i < numDevs; i++) {
        char desc[256];
        char serial[256];
        
        // Obtenir la description
        status = FT_ListDevices((PVOID)(uintptr_t)i, desc, FT_LIST_BY_INDEX | FT_OPEN_BY_DESCRIPTION);
        
        // Obtenir le numéro de série
        status = FT_ListDevices((PVOID)(uintptr_t)i, serial, FT_LIST_BY_INDEX | FT_OPEN_BY_SERIAL_NUMBER);
        
        if (status == FT_OK) {
            printf("Device %ld: Description=%s, Serial=%s\n", i, desc, serial);
        }
    }
    
    return FT_OK;
}


/**
 * Lit les données de température depuis la carte électronique
 * Utilise le handle partagé avec commande.c
 * 
 * Format: [byte0: bit11-8 SOT_ext] [byte1: bit7-4 SOT_ext] [byte2: bit3-0 SOT_ext]
 *         [byte3: bit11-8 SOT_int] [byte4: bit7-4 SOT_int] [byte5: bit3-0 SOT_int]
 * 
 * Conversion: T°C = -39.64 + 0.04 × SOT (SOT = nombre 12 bits)
 * 
 * @return Structure temp_t avec les températures lues
 */
temp_t releve_read(void) {
    temp_t temperatures;
    temperatures.interieure = 0.0;
    temperatures.exterieure = 0.0;
    
    unsigned char buffer[256];
    DWORD bytesRead = 0;
    DWORD bytesAvailable = 0;
    
    // Vérifier que le device est ouvert
    if (ftHandle == NULL || ftStatus != FT_OK) {
        printf("Erreur: Device USB non connecte pour la lecture\n");
        return temperatures;
    }
    
    // Vérifier si des données sont disponibles
    ftStatus = FT_GetQueueStatus(ftHandle, &bytesAvailable);
    if (ftStatus != FT_OK) {
        printf("Erreur: impossible de lire l'etat de la file RX\n");
        return temperatures;
    }
    
    // Si pas de données disponibles, retourner des valeurs par défaut
    if (bytesAvailable == 0) {
        return temperatures;
    }
    
    // Lire au maximum 256 bytes
    DWORD toRead = (bytesAvailable > 255) ? 255 : bytesAvailable;
    ftStatus = FT_Read(ftHandle, buffer, toRead, &bytesRead);
    
    if (ftStatus != FT_OK) {
        printf("Erreur FT_Read: code %ld\n", ftStatus);
        return temperatures;
    }
    
    // Vérifier que nous avons au moins 6 bytes
    if (bytesRead >= 6) {
        // Extraire SOT (12 bits) pour température extérieure
        // SOT_ext[11:8] de byte0[3:0], SOT_ext[7:4] de byte1[3:0], SOT_ext[3:0] de byte2[3:0]
        unsigned int SOT_ext = ((buffer[0] & 0x0F) << 8) | 
                               ((buffer[1] & 0x0F) << 4) | 
                               (buffer[2] & 0x0F);
        
        // Extraire SOT (12 bits) pour température intérieure
        // SOT_int[11:8] de byte3[3:0], SOT_int[7:4] de byte4[3:0], SOT_int[3:0] de byte5[3:0]
        unsigned int SOT_int = ((buffer[3] & 0x0F) << 8) | 
                               ((buffer[4] & 0x0F) << 4) | 
                               (buffer[5] & 0x0F);
        
        // Convertir SOT en degrés Celsius: T = -39.64 + 0.04 * SOT
        temperatures.exterieure = -39.64f + 0.04f * SOT_ext;
        temperatures.interieure = -39.64f + 0.04f * SOT_int;
        
        printf("Releve reussi: T_int=%.2f°C, T_ext=%.2f°C (SOT_int=%u, SOT_ext=%u)\n", 
               temperatures.interieure, temperatures.exterieure, SOT_int, SOT_ext);
    } 
    else if (bytesRead > 0) {
        printf("Avertissement: seulement %ld bytes lus (attend 6)\n", bytesRead);
        printf("Hexa: ");
        for (DWORD i = 0; i < bytesRead; i++) {
            printf("0x%02X ", buffer[i]);
        }
        printf("\n");
    }
    
    return temperatures;
}
