#include "common.h"
#include "../include/mod_main.h"
#include "xstdio.h"
#include "../include/menu.h"

CustomThread gCustomThread;

void decompress_lz4_ct_default(int srcSize, int savestateCompressedSize, u8* compressBuffer);
s32 compress_lz4_ct_default(const u8* srcData, int srcSize, u8* bufferAddr);
s32 saveStateBackupSize = 0;

int __osPiDeviceBusy(void) {
    register u32 stat = IO_READ(PI_STATUS_REG);
    if (stat & (PI_STATUS_DMA_BUSY | PI_STATUS_IO_BUSY))
        return 1;
    return 0;
}

void loadstateMainBackup(void) {
    u32* prevCurrentStageTimeRTA = (u32*)0x80109DD4;
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

    decompress_lz4_ct_default(ramEndAddr - ramStartAddr, saveStateBackupSize, savestateBackup);
    //osSetCount(*savestateCount);
    *prevCurrentStageTimeRTA = osGetCount();
    __osRestoreInt(saveMask);
    isSaveOrLoadActive = 0; //allow thread 3 to continue
}

void loadstateMain(void) {
    u32* prevCurrentStageTimeRTA = (u32*)0x80109DD4;
    u32* savestateCount = (u32*)0x807FFFFC;
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
            if (savestate1Size != 0 || savestate1Size != -1) {
                decompress_lz4_ct_default(ramEndAddr - ramStartAddr, savestate1Size, ramAddrSavestateDataSlot1);
            }
        break;
        case 1:
            if (savestate2Size != 0 || savestate2Size != -1) {
                decompress_lz4_ct_default(ramEndAddr - ramStartAddr, savestate2Size, ramAddrSavestateDataSlot2);
            }
        break;
    }
    //osSetCount(*savestateCount);
    *prevCurrentStageTimeRTA = osGetCount();
    __osRestoreInt(saveMask);
    
    isSaveOrLoadActive = 0; //allow thread 3 to continue
}
    
void savestateMain(void) {
    u32 saveMask;
    u32* savestateCount = (u32*)0x807FFFFC;
    
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
    *savestateCount = osGetCount();
    switch (savestateCurrentSlot) {
        case 0:
            if (savestate1Size == 0) {
                if (savestate2Size == 0) {
                    //both states blank, create state
                    savestate1Size = compress_lz4_ct_default((void*)ramStartAddr, ramEndAddr - ramStartAddr, ramAddrSavestateDataSlot1);
                } else {
                    //savestate 1 isn't initialized but savestate 2 is, therefore backup state 2
                    optimized_memcpy(savestateBackup, ramAddrSavestateDataSlot2, savestate2Size);
                    saveStateBackupSize = savestate2Size;
                    savestate1Size = compress_lz4_ct_default((void*)ramStartAddr, ramEndAddr - ramStartAddr, ramAddrSavestateDataSlot1);
                }
            } else {
                //savestate 1 is already created, therefore backup state 1
                optimized_memcpy(savestateBackup, ramAddrSavestateDataSlot1, savestate1Size);
                saveStateBackupSize = savestate1Size;
                savestate1Size = compress_lz4_ct_default((void*)ramStartAddr, ramEndAddr - ramStartAddr, ramAddrSavestateDataSlot1);                
            }
        break;

        case 1:
            if (savestate2Size == 0) {
                if (savestate1Size == 0) {
                    //both states blank, create state
                    savestate2Size = compress_lz4_ct_default((void*)ramStartAddr, ramEndAddr - ramStartAddr, ramAddrSavestateDataSlot2);
                } else {
                    //savestate 2 isn't initialized but savestate 1 is, therefore backup state 1
                    optimized_memcpy(savestateBackup, ramAddrSavestateDataSlot1, savestate1Size);
                    saveStateBackupSize = savestate1Size;
                    savestate2Size = compress_lz4_ct_default((void*)ramStartAddr, ramEndAddr - ramStartAddr, ramAddrSavestateDataSlot2);
                }
            } else {
                //savestate 2 is already created, therefore backup state 2
                optimized_memcpy(savestateBackup, ramAddrSavestateDataSlot2, savestate2Size);
                saveStateBackupSize = savestate2Size;
                savestate2Size = compress_lz4_ct_default((void*)ramStartAddr, ramEndAddr - ramStartAddr, ramAddrSavestateDataSlot2);                
            }
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
        gameModeCurrent != GAME_MODE_TITLE_SCREEN && gameModeCurrent != GAME_MODE_LEVEL_INTRO_MENU &&
        gIsPaused == 0)
    {
        if (D_80175650[0].button & CONT_LEFT) {
            isSaveOrLoadActive = 1;
            Controller_Zero(&gContMain[0]);
            osCreateThread(&gCustomThread.thread, 255, (void*)savestateMain, NULL,
                    gCustomThread.stack + sizeof(gCustomThread.stack), 255);
            osStartThread(&gCustomThread.thread);
            stateCooldown = 5;
        } else if (D_80175650[0].button & CONT_RIGHT) {
            isSaveOrLoadActive = 1;
            osCreateThread(&gCustomThread.thread, 255, (void*)loadstateMain, NULL,
                    gCustomThread.stack + sizeof(gCustomThread.stack), 255);
            osStartThread(&gCustomThread.thread);
            stateCooldown = 5;   
        } else if (D_80175650[0].button & CONT_UP) {
            if (saveStateBackupSize != 0) {
                isSaveOrLoadActive = 1;
                osCreateThread(&gCustomThread.thread, 255, (void*)loadstateMainBackup, NULL,
                        gCustomThread.stack + sizeof(gCustomThread.stack), 255);
                osStartThread(&gCustomThread.thread);
                stateCooldown = 5;
            }
        }
    }
}