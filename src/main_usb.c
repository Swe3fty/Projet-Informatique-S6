#include <windows.h>
#include "define.h"
#include "ftd2xx.h"
#include "releve.h"
#include "visualisationT.h"
#include "visualisationC.h"
#include "consigne.h"
#include "regulation.h"
#include "commande.h"

int main(void) {

    remove(".verrouData");
    remove(".verrouConsigne");

    temp_t temperatures = {20.0f, 15.0f};
    float csgn = 20.0f;
    float cmd = 0.0f;
    pid PID = {0.0f, 0.0f, 0};
    DWORD deviceIndex = 0;

    if (releve_connexions() == 0) {
        printf("Aucun device FTDI disponible\n");
        return EXIT_FAILURE;
    }

    do {
        //Releve des temperatures
        temperatures = releve_connect_and_read(deviceIndex);

        //Releve consigne
        csgn = consigne(csgn);

        //Regulation a partir de consigne
        cmd = regulation(2, csgn, temperatures.interieure, &PID);

        //Envoie de la puissance de chauffage
        envoie_cmd(deviceIndex, cmd);

        //Visualisation sur data.txt
        visualisationT(temperatures);
        visualisationC(cmd);

        printf("T_int=%.1f°C, T_ext=%.1f°C, cmd=%.1f%%\n",
               temperatures.interieure, temperatures.exterieure, cmd);

        Sleep(40);

    } while (csgn > CONSIGNE_MIN);

    envoie_cmd(deviceIndex, 0.0);
    visualisationC(0.0);

    return EXIT_SUCCESS;
}