#ifndef CARTRDIGE_DF_H
#define CARTRIDGE_DF_H

#include <stdint.h>

void emulate_df_cartridge(const char* filename, uint32_t image_size, uint8_t* buffer);

void emulate_dfsc_cartridge(const char* filename, uint32_t image_size, uint8_t* buffer);

#endif // CARTRIDGE_DF_H
