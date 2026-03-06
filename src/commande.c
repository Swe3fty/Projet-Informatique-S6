#include "commande.h"

/*Fonction qui envoie la puissance en paramètre a la carte*/
void envoie_cmd(DWORD device_index, float cmd) {

    FT_HANDLE localHandle;
    FT_STATUS ftStatus;
    DWORD bytesWritten;

    //Test du lien d'ouverture
    ftStatus = FT_Open(device_index, &localHandle);
    if (ftStatus != FT_OK) {
        printf("Impossible d'ouvrir la carte\n");
        return;  
    }

    //Mettre les paramètres
    ftStatus = FT_SetBaudRate(localHandle, 115200);
    if (ftStatus != FT_OK) {
        printf("Impossible de definir le Baud Rate\n");
        FT_Close(localHandle);
        return;
    }

    //Mettre a jour les caractéristiques
    FT_SetDataCharacteristics(localHandle, FT_BITS_8, FT_STOP_BITS_1, FT_PARITY_NONE);
    FT_SetFlowControl(localHandle, FT_FLOW_NONE, 0, 0);
    FT_SetTimeouts(localHandle, 1000, 1000);

    // Conversion et envoi
    uint8_t puis = (uint8_t)((cmd / 100.0) * 127.0);
    uint8_t octet = puis & 0x7F;  // bit 7 = 0 (chauffage)

    FT_Write(localHandle, &octet, 1, &bytesWritten);
    if (bytesWritten != 1) {
        printf("Erreur d'envoi de la commande\n");
    }

    FT_Close(localHandle);  
}