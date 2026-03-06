#ifndef RELEVE_H
#define RELEVE_H

#include <windows.h>
#include <string.h>
#include <stdint.h>
#include "ftd2xx.h"
#include "define.h"


DWORD releve_connexions(void);
temp_t releve_connect_and_read(DWORD device_index);

#endif
