#include "common.h"
#include "../include/mod_main.h"

typedef struct CustomThread {
    /* 0x000 */ OSThread thread;
    /* 0x1B0 */ char stack[0xC000];
    /* 0x9B0 */ OSMesgQueue queue;
    /* 0x9C8 */ OSMesg mesg;
    /* 0x9CC */ u16* frameBuf;
    /* 0x9D0 */ u16 width;
    /* 0x9D2 */ u16 height;
} CustomThread; // size = 0x9D4

CustomThread gCustomThread;

void decompress_lz4_ct_default(int srcSize, int savestateCompressedSize, u8* compressBuffer);
s32 compress_lz4_ct_default(const u8* srcData, int srcSize, u8* bufferAddr);

int __osPiDeviceBusy(void) {
    register u32 stat = IO_READ(PI_STATUS_REG);
    if (stat & (PI_STATUS_DMA_BUSY | PI_STATUS_IO_BUSY))
        return 1;
    return 0;
}

void loadstateMain(void) {
    u32 saveMask;
    //wait on rsp
    while (__osSpDeviceBusy() == 1) {}

    //wait on rdp
    while ( __osDpDeviceBusy() == 1) {}

    //wait on SI
    while (__osSiDeviceBusy() == 1) {}

    //wait on PI
    while (__osPiDeviceBusy() == 1) {}

    //invalidate caches
    osInvalICache((void*)0x80000000, 0x2000);
	osInvalDCache((void*)0x80000000, 0x2000);
    saveMask = __osDisableInt();

    switch (savestateCurrentSlot) {
        case 0: //dpad left
            if (savestate1Size != 0 && savestate1Size != -1) {
                decompress_lz4_ct_default(ramEndAddr - ramStartAddr, savestate1Size, ramAddrSavestateDataSlot1);
            }
            break;
        case 1:
            if (savestate2Size != 0 && savestate2Size != -1) {
                decompress_lz4_ct_default(ramEndAddr - ramStartAddr, savestate2Size, ramAddrSavestateDataSlot2);
            }
            break;
    }

    __osRestoreInt(saveMask);
    isSaveOrLoadActive = 0; //allow thread 3 to continue
}
    
void savestateMain(void) {
    u32 saveMask;
    //wait on rsp
    while (__osSpDeviceBusy() == 1) {}

    //wait on rdp
    while ( __osDpDeviceBusy() == 1) {}

    //wait on SI
    while (__osSiDeviceBusy() == 1) {}

    //wait on PI
    while (__osPiDeviceBusy() == 1) {}

    //invalidate caches
    osInvalICache((void*)0x80000000, 0x2000);
	osInvalDCache((void*)0x80000000, 0x2000);

    saveMask = __osDisableInt();

    switch (savestateCurrentSlot) {
        case 0:
            savestate1Size = compress_lz4_ct_default((void*)ramStartAddr, ramEndAddr - ramStartAddr, ramAddrSavestateDataSlot1);
        break;

        case 1:
            savestate2Size = compress_lz4_ct_default((void*)ramStartAddr, ramEndAddr - ramStartAddr, ramAddrSavestateDataSlot2);
        break;
    }
    
    __osRestoreInt(saveMask);
    isSaveOrLoadActive = 0; //allow thread 3 to continue
}

void checkInputsForSavestates(void) {
    if (stateCooldown != 0){
        return;
    }

    if (gameModeCurrent != GAME_MODE_STAGE_SELECT && gameModeCurrent != GAME_MODE_NEW_GAME_MENU &&
        gameModeCurrent != GAME_MODE_TITLE_SCREEN && gameModeCurrent != GAME_MODE_LEVEL_INTRO &&
        gIsPaused == 0) {
            if (gContMain[0].buttons0 & CONT_LEFT) {
                isSaveOrLoadActive = 1;
                osCreateThread(&gCustomThread.thread, 255, (void*)savestateMain, NULL,
                        gCustomThread.stack + sizeof(gCustomThread.stack), 255);
                osStartThread(&gCustomThread.thread);
                stateCooldown = 5;
            } else if (gContMain[0].buttons0 & CONT_RIGHT) {
                isSaveOrLoadActive = 1;
                osCreateThread(&gCustomThread.thread, 255, (void*)loadstateMain, NULL,
                        gCustomThread.stack + sizeof(gCustomThread.stack), 255);
                osStartThread(&gCustomThread.thread);
                stateCooldown = 5;   
            }
    }
}