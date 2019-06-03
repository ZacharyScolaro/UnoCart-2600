
#ifndef CARTRIDGE_ACE
#define CARTRIDGE_ACE CARTRIDGE_ACE
int is_ace_cartridge(unsigned int image_size, uint8_t *buffer);
int launch_ace_cartridge(const char* filename, uint32_t buffer_size, uint8_t *buffer);
#endif
