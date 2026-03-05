#include "define.h"
#include "commande.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Variables GLOBALES partagées avec releve.c
FT_HANDLE ftHandle = NULL;
FT_STATUS ftStatus = FT_DEVICE_NOT_OPENED;

/**
 * Liste tous les devices FTDI connectés
 */
FT_STATUS commande_list_devices(void) {
    FT_STATUS ftStatus = FT_OK;
    DWORD numDevs = 0;
    
    // Compter le nombre de devices FTDI connectés
    ftStatus = FT_ListDevices(&numDevs, NULL, FT_LIST_NUMBER_ONLY);
    
    if (ftStatus != FT_OK) {
        printf("Erreur: impossible de lister les devices FTDI\n");
        return ftStatus;
    }
    
    if (numDevs == 0) {
        printf("Aucun device FTDI detecte\n");
        return FT_DEVICE_NOT_FOUND;
    }
    
    printf("Nombre de devices FTDI detectes: %ld\n", numDevs);
    
    // Lister les descriptions de chaque device
    for (DWORD i = 0; i < numDevs; i++) {
        char desc[256];
        char serial[256];
        
        // Obtenir la description
        ftStatus = FT_ListDevices((PVOID)(uintptr_t)i, desc, FT_LIST_BY_INDEX | FT_OPEN_BY_DESCRIPTION);
        
        // Obtenir le numéro de série
        ftStatus = FT_ListDevices((PVOID)(uintptr_t)i, serial, FT_LIST_BY_INDEX | FT_OPEN_BY_SERIAL_NUMBER);
        
        if (ftStatus == FT_OK) {
            printf("Device %ld: Description=%s, Serial=%s\n", i, desc, serial);
        }
    }
    
    return FT_OK;
}

/**
 * Initialise la connexion USB avec la carte FTDI
 * @param device_index Index du device à ouvrir (0 pour le premier)
 * @return FT_OK si succès, code d'erreur sinon
 */
FT_STATUS commande_init(DWORD device_index) {
    // Ouvrir le device FTDI spécifié
    ftStatus = FT_Open(device_index, &ftHandle);
    
    if (ftStatus != FT_OK) {
        printf("Erreur: impossible d'ouvrir le device FTDI (index %ld)\n", device_index);
        return ftStatus;
    }
    
    // Configurer la vitesse (baud rate) : 115200 bauds selon la doc
    ftStatus = FT_SetBaudRate(ftHandle, 115200);
    if (ftStatus != FT_OK) {
        printf("Erreur: impossible de configurer la vitesse (115200 baud)\n");
        FT_Close(ftHandle);
        ftHandle = NULL;
        return ftStatus;
    }
    
    // Configurer les paramètres de données : 8 bits, 1 stop bit, pas de parité
    ftStatus = FT_SetDataCharacteristics(ftHandle, FT_BITS_8, FT_STOP_BITS_1, FT_PARITY_NONE);
    if (ftStatus != FT_OK) {
        printf("Erreur: impossible de configurer les caractéristiques\n");
        FT_Close(ftHandle);
        ftHandle = NULL;
        return ftStatus;
    }
    
    // Pas de contrôle de flux selon la documentation
    ftStatus = FT_SetFlowControl(ftHandle, FT_FLOW_NONE, 0x00, 0x00);
    if (ftStatus != FT_OK) {
        printf("Erreur: impossible de configurer le flux\n");
        FT_Close(ftHandle);
        ftHandle = NULL;
        return ftStatus;
    }
    
    // Vider les buffers
    ftStatus = FT_Purge(ftHandle, FT_PURGE_RX | FT_PURGE_TX);
    if (ftStatus != FT_OK) {
        printf("Erreur: impossible de vider les buffers\n");
        FT_Close(ftHandle);
        ftHandle = NULL;
        return ftStatus;
    }
    
    printf("Connexion USB etablie avec succes (Device %ld)\n", device_index);
    return FT_OK;
}

/**
 * Envoie la commande de chauffage à la carte électronique
 * @param cmd Puissance du chauffage (0.0 à 100.0)
 * @return cmd (la même valeur reçue) ou -1 en cas d'erreur
 */
float commande(float cmd) {
    unsigned char buffer[3];
    DWORD bytesWritten = 0;
    DWORD bytesAvailable = 0;
    
    // Vérifier que le device est ouvert
    if (ftHandle == NULL || ftStatus != FT_OK) {
        printf("Erreur: Device USB non connecte\n");
        return -1.0;
    }
    
    // Saturer la valeur entre 0 et 100
    if (cmd < 0.0) cmd = 0.0;
    if (cmd > 100.0) cmd = 100.0;
    
    // Convertir la puissance float (0-100) en byte (0-255)
    unsigned char puissance = (unsigned char)((cmd / 100.0) * 255);
    
    // Construire le message selon le protocole (adapter selon votre électronique)
    // Format: [En-tête 0xAA] [Puissance] [Checksum]
    buffer[0] = 0xAA;              // En-tête du message
    buffer[1] = puissance;         // Valeur de puissance (0-255)
    buffer[2] = buffer[0] ^ buffer[1];  // Checksum simple (XOR)
    
    // Vérifier la file d'attente TX
    ftStatus = FT_GetQueueStatus(ftHandle, &bytesAvailable);
    if (ftStatus != FT_OK) {
        printf("Erreur: impossible de lire l'etat de la file TX\n");
        return -1.0;
    }
    
    // Envoyer les données (3 bytes)
    ftStatus = FT_Write(ftHandle, buffer, 3, &bytesWritten);
    
    if (ftStatus != FT_OK) {
        printf("Erreur FT_Write: code %ld\n", ftStatus);
        return -1.0;
    }
    
    if (bytesWritten != 3) {
        printf("Erreur: %ld bytes ecrits au lieu de 3\n", bytesWritten);
        return -1.0;
    }
    
    printf("Commande envoyee: %.1f%% (0x%02X) Checksum=0x%02X\n", cmd, puissance, buffer[2]);
    
    return cmd;
}

/**
 * Ferme la connexion USB
 */
void commande_close(void) {
    if (ftHandle != NULL) {
        ftStatus = FT_Purge(ftHandle, FT_PURGE_RX | FT_PURGE_TX);
        FT_Close(ftHandle);
        ftHandle = NULL;
        ftStatus = FT_DEVICE_NOT_OPENED;
        printf("Connexion USB fermee\n");
    }
}
