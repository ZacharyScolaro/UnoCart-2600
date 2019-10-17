#ifndef CARTRIDGE_PP_H
#define CARTRIDGE_PP_H

#include <stdint.h>

void emulate_pp_cartridge(uint32_t image_size, uint8_t* buffer, uint8_t* ram);

#endif // CARTRIDGE_PP_H
