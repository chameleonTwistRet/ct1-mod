#include "common.h"
#include "../include/mod_main.h"

CustomThread gCustomThread;

void decompress_lz4_ct_default(int srcSize, int savestateCompressedSize, u8* compressBuffer);
s32 compress_lz4_ct_default(const u8* srcData, int srcSize, u8* bufferAddr);
void optimized_memcpy(void*, void*, s32);
void WriteFileToSDCard(void* buffer, s32 size, char* filePath);
void ReadFileFromSDCard(void* buffer, s32 size, char* filePath);

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

    // always do uncompressed
    optimized_memcpy((void*)ramStartAddr, (void*)ramAddrSavestateDataSlot1, ramEndAddr - ramStartAddr);

    inputRecordingBuffer.framePlaybackIndex = SaveStateVars->playbackIndex;
    inputRecordingBuffer.totalFrameCount = SaveStateVars->recordingFrameCount;
    osWritebackDCacheAll();
    __osRestoreInt(saveMask);
    isSaveOrLoadActive = 0; //allow thread 3 to continue

    // //force uncompressed states if recording is on
    // if (toggles[TOGGLE_RECORDING] == 1 || toggles[TOGGLE_PLAYBACK] == 1) {
    //     // load uncompressed state
    //     optimized_memcpy((void*)ramStartAddr, (void*)ramAddrSavestateDataSlot1, ramEndAddr - ramStartAddr);
    //     inputRecordingBuffer.framePlaybackIndex = SaveStateVars->playbackIndex;
    //     inputRecordingBuffer.totalFrameCount = SaveStateVars->recordingFrameCount;
    //     __osRestoreInt(saveMask);
    //     isSaveOrLoadActive = 0; //allow thread 3 to continue
    //     return;    
    // }

    // switch (savestateCurrentSlot) {
    //     case 0: //dpad left
    //         if (savestate1Size != 0 && savestate1Size != -1) {
    //             decompress_lz4_ct_default(ramEndAddr - ramStartAddr, savestate1Size, ramAddrSavestateDataSlot1);
    //         }
    //         break;
    //     case 1:
    //         if (savestate2Size != 0 && savestate2Size != -1) {
    //             decompress_lz4_ct_default(ramEndAddr - ramStartAddr, savestate2Size, ramAddrSavestateDataSlot2);
    //         }
    //         break;
    // }

    // __osRestoreInt(saveMask);
    // isSaveOrLoadActive = 0; //allow thread 3 to continue
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

    //always do uncompressed
    saveMask = __osDisableInt();
    SaveStateVars->playbackIndex = inputRecordingBuffer.framePlaybackIndex;
    SaveStateVars->recordingFrameCount = inputRecordingBuffer.totalFrameCount;
    optimized_memcpy((void*)ramAddrSavestateDataSlot1,  (void*)ramStartAddr, ramEndAddr - ramStartAddr);
    __osRestoreInt(saveMask);
    isSaveOrLoadActive = 0; //allow thread 3 to continue




    // //force uncompressed states if recording is on
    // if (toggles[TOGGLE_RECORDING] == 1 || toggles[TOGGLE_PLAYBACK] == 1) {
    //     // save uncompressed state
    //     SaveStateVars->playbackIndex = inputRecordingBuffer.framePlaybackIndex;
    //     SaveStateVars->recordingFrameCount = inputRecordingBuffer.totalFrameCount;
    //     optimized_memcpy((void*)ramAddrSavestateDataSlot1,  (void*)ramStartAddr, ramEndAddr - ramStartAddr);
    //     __osRestoreInt(saveMask);
    //     isSaveOrLoadActive = 0; //allow thread 3 to continue
    //     return;    
    // }

    // switch (savestateCurrentSlot) {
    //     case 0:
    //         savestate1Size = compress_lz4_ct_default((void*)ramStartAddr, ramEndAddr - ramStartAddr, ramAddrSavestateDataSlot1);
    //     break;

    //     case 1:
    //         savestate2Size = compress_lz4_ct_default((void*)ramStartAddr, ramEndAddr - ramStartAddr, ramAddrSavestateDataSlot2);
    //     break;
    // }
    
    // __osRestoreInt(saveMask);
    // isSaveOrLoadActive = 0; //allow thread 3 to continue
}