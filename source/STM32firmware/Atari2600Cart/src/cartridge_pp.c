#include <stdbool.h>

#include "cartridge_pp.h"
#include "cartridge_firmware.h"

static void setupSegments(uint8_t* buffer, uint8_t** segments, uint8_t zero, uint8_t one, uint8_t two, uint8_t three) {
    segments[0] = buffer + (zero << 10);
    segments[1] = buffer + (one  << 10);
    segments[2] = buffer + (two << 10);
    segments[3] = buffer + (three << 10);
}

static void switchLayout(uint8_t* buffer, uint8_t** segments, uint8_t index) {
    switch (index) {
        case 0:
            return setupSegments(buffer, segments, 0, 0, 1 ,2);

        case 1:
            return setupSegments(buffer, segments, 0, 1, 3 ,2);

        case 2:
            return setupSegments(buffer, segments, 4, 5, 6, 7);

        case 3:
            return setupSegments(buffer, segments, 7, 4, 3, 2);

        case 4:
            return setupSegments(buffer, segments, 0, 0, 6, 7);

        case 5:
            return setupSegments(buffer, segments, 0, 1, 7, 6);

        case 6:
            return setupSegments(buffer, segments, 3, 2, 4, 5);

        case 7:
            return setupSegments(buffer, segments, 6, 0, 5, 1);
    }
}

void emulate_pp_cartridge(uint32_t image_size, uint8_t* buffer, uint8_t* ram) {
    uint8_t* segmentLayout[32];

    bool bankswitch_pending = false;
    uint8_t pending_bank = 0;
    uint8_t bankswitch_counter = 0;

    uint16_t addr, addr_prev = 0, addr_prev2 = 0, data = 0, data_prev = 0;

    for (int i = 0; i <= 7; i++) {
        switchLayout(buffer, &segmentLayout[4*i], i);
    }

    if (!reboot_into_cartridge()) return;
    __disable_irq();

    uint8_t** segments = segmentLayout;

    while (1) {
        while (((addr = ADDR_IN) != addr_prev) || (addr != addr_prev2))
		{
			addr_prev2 = addr_prev;
			addr_prev = addr;
		}

        if (bankswitch_pending && bankswitch_counter-- == 0) {
            segments = &segmentLayout[pending_bank * 4];
            bankswitch_pending = false;
        }

        if (addr & 0x1000) {
            uint16_t caddr = addr & 0x0fff;

            if (caddr < 0x40) {
                data = ram[caddr];

                DATA_OUT = ((uint16_t)data)<<8;
	    		SET_DATA_MODE_OUT

                while (ADDR_IN == addr);
                SET_DATA_MODE_IN
            }
            else if (caddr < 0x80) {
                while (ADDR_IN == addr) { data_prev = data; data = DATA_IN; }

                ram[caddr - 0x40] = data_prev >> 8;
            }
            else {
                data = segments[caddr >> 10][caddr & 0x03ff];

                DATA_OUT = ((uint16_t)data)<<8;
	    		SET_DATA_MODE_OUT

                while (ADDR_IN == addr) ;
                SET_DATA_MODE_IN
            }
        } else {
            uint8_t zaddr = addr & 0xff;

            if (zaddr >= 0x30 && zaddr <= 0x3f) {
                bankswitch_pending = true;
                pending_bank = zaddr & 0x07;
                bankswitch_counter = 3;
            }
        }
    }
}
