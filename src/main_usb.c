#include <windows.h>
#include "define.h"
#include "ftd2xx.h"
#include "releve.h"
#include "visualisationT.h"
#include "consigne.h"

int main(void) {
    remove(".verrouData");
    remove(".verrouConsigne");
    temp_t temperatures = {20.0, 15.0};
    float csgn = 0.0;
    DWORD deviceIndex = 0;

    if (releve_connexions() == 0) {
        printf("Aucun device FTDI disponible\n");
        return EXIT_FAILURE;
    }

    do {
        temperatures = releve_connect_and_read(deviceIndex);
        csgn = consigne(csgn);
        visualisationT(temperatures);

        printf("T_int=%.1f°C, T_ext=%.1f°C\n",
               temperatures.interieure, temperatures.exterieure);

        Sleep(40);

    } while (csgn > CONSIGNE_MIN);

    return EXIT_SUCCESS;
}