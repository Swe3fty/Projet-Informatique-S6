#ifndef RELEVE_H
#define RELEVE_H

#include <windows.h>
#include "ftd2xx.h"
#include "define.h"

FT_STATUS releve_list_devices(void);
temp_t releve_read(void);

#endif
