#ifndef CARTRDIGE_BF_H
#define CARTRIDGE_BF_H

#include <stdint.h>

void emulate_bf_cartridge(const char* filename, uint32_t image_size, uint8_t* buffer);

void emulate_bfsc_cartridge(const char* filename, uint32_t image_size, uint8_t* buffer);

#endif // CARTRIDGE_BF_H
