#include "releve.h"

/*Fonction qui releve toutes les connections avec certains périphérique et renvoie le nombre de connexions*/
DWORD releve_connexions(void){
    FT_STATUS ftStatus;
    DWORD numDevs = 0; //définit avec window.h -> Double Word 32bits uint, utilisé pour les api windows
    char * BufPtrs[MAX_DEVICES+1];
    char Buffer0[64];
    char Buffer1[64];
    char Buffer2[64];
    char Buffer3[64];

    BufPtrs[0] = Buffer0;
    BufPtrs[1] = Buffer1;
    BufPtrs[2] = Buffer2;
    BufPtrs[3] = Buffer3;
    BufPtrs[4] = NULL;


    //Connaître le nombre de périphériques connectés a l'ordinateur
    ftStatus = FT_ListDevices(BufPtrs,&numDevs,FT_LIST_ALL|FT_OPEN_BY_DESCRIPTION);
    if(ftStatus == FT_OK){
        printf("Nombre de périphériques connectés : %ld\n",numDevs);
        for(DWORD i = 0; i<numDevs && i<MAX_DEVICES;i++);
    } else {
        printf("Erreur dans la lectures des périphériques\n");
    }





    return numDevs;
}

temp_t releve_connect_and_read(DWORD device_index){
    //variables locales
    temp_t temperatures = {0.0,0.0};
    FT_HANDLE localHandle;
    FT_STATUS ftStatus;
    uint8_t buffer[6];
    DWORD bytesreceived;

    //bytes :
    uint16_t SOT_ext;
    uint16_t SOT_int;

    //Ouverture de communication avec la carte
    ftStatus = FT_Open(device_index,&localHandle);
    if(ftStatus != FT_OK){
        printf("Impossible d'ouvrir la carte\n");
        return temperatures;
    }

    //Vitesse de liaison
    ftStatus = FT_SetBaudRate(localHandle,115200);
    if(ftStatus != FT_OK){
        printf("Impossible de definir le Baud Rate\n");
        return temperatures;
    }

    //Caractéristiques de la liaison
    ftStatus = FT_SetDataCharacteristics(localHandle, FT_BITS_8, FT_STOP_BITS_1,FT_PARITY_NONE);
    ftStatus = FT_SetFlowControl(localHandle,FT_FLOW_NONE,0,0);
    ftStatus = FT_SetTimeouts(localHandle,1000,1000); //Timeout écriture lecture

    //Lecture des bytes reçu
    ftStatus = FT_Read(localHandle, buffer, 6, &bytesreceived);
    if(ftStatus != FT_OK && bytesreceived!=6){
        printf("Erreur de lecture de la carte\n");
        FT_Close(localHandle);
        return temperatures;
    }

    //Conversion en SOT et renvoie
    SOT_ext = ((buffer[0] & 0x0F) << 8) | ((buffer[1] & 0x0F) << 4) | (buffer[2] & 0x0F);
    SOT_int = ((buffer[3] & 0x0F) << 8) | ((buffer[4] & 0x0F) << 4) | (buffer[5] & 0x0F);

    temperatures.interieure = -39.64f + 0.04f * SOT_int;
    temperatures.exterieure = -39.64f + 0.04f * SOT_ext;
    
    FT_Close(localHandle);
    return temperatures;
}
