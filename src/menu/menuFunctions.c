#include "common.h"
#include "../../include/menu.h"
#include "../../include/mod_main.h"


s32 DisplayIndividualRoomTimeToggle(void) {
    toggles[TOGGLE_DISPLAY_INDIVIDUAL_ROOM_TIME] ^= 1;
    return 0;
}

s32 DisplayTASComparison(void) {
    toggles[TOGGLE_TAS_COMPARISON] ^= 1;
    return 0;
}

s32 DisplayTongueRetractionTime(void) {
    toggles[TOGGLE_TAS_COMPARISON] ^= 1;
    return 0;
}

s32 FrameAdvanceToggle(void) {
    toggles[TOGGLE_FRAME_ADVANCE] ^= 1;
    return 0;
}

s32 NoCompressionToggle(void) {
    toggles[TOGGLE_NO_COMPRESSION_SAVESTATES] ^= 1;
    savestate1Size = 0;
    savestate2Size = 0;
    return 0;
}

s32 advanceGuRNG(void) {
    guRandom(); //advance rng once
    return 0;
}

s32 revGuRNG(void) {
    guRandomRev(); //advance rng backwards
    return 0;
}

s32 setJLSeed(void) {
    toggles[TOGGLE_SET_SEED_JL]++;
    if (toggles[TOGGLE_SET_SEED_JL] >= 3) {
        toggles[TOGGLE_SET_SEED_JL] = 0;
    }
    return 0;
}

s32 setBLSeed(void) {
    toggles[TOGGLE_SET_SEED_BL]++;
    if (toggles[TOGGLE_SET_SEED_BL] >= 3) {
        toggles[TOGGLE_SET_SEED_BL] = 0;
    }
    return 0;
}

s32 setBLILSeed(void) {
    toggles[TOGGLE_SET_SEED_IL_BL] ^= 1;
    return 0;
}

s32 setKLSeed(void) {
    toggles[TOGGLE_SET_SEED_KL] ^= 1;
    return 0;
}

s32 setGCSeed(void) {
    toggles[TOGGLE_SET_SEED_GC] ^= 1;
    return 0;
}

s32 toggleYoloPillarPractice(void) {
    toggles[TOGGLE_YOLO_PILLAR_PRACTICE] ^= 1;
    return 0;
}

s32 toggleBackVaultPractice(void) {
    toggles[TOGGLE_BACKVAULT_PRACTICE] ^= 1;
    return 0;
}

s32 toggleHideSavestateText(void) {
    toggles[TOGGLE_HIDE_SAVESTATE_TEXT] ^= 1;
    return 1;
}

s32 toggleHideIGT(void) {
    toggles[TOGGLE_DISPLAY_IGT]++;
    if (toggles[TOGGLE_DISPLAY_IGT] >= 5) {
        toggles[TOGGLE_DISPLAY_IGT] = 0;
    }
    return 1;
}

s32 toggleInfiniteHealth(void) {
    toggles[TOGGLE_INFINITE_HEALTH] ^= 1;
    return 1;
}

s32 toggleStorage(void) {
    toggles[TOGGLE_STORAGE] ^= 1;
    return 1;
}

// s32 toggleSpeed(void) {
//     toggles[TOGGLE_SPEED] ^= 1;
//     return 1;
// }

s32 toggleCustomDebugText(void) {
    toggles[TOGGLE_CUSTOM_DEBUG_TEXT]++;
    if (toggles[TOGGLE_CUSTOM_DEBUG_TEXT] >= 8) {
        toggles[TOGGLE_CUSTOM_DEBUG_TEXT] = 0;
    }
    return 1;
}

s32 toggleCaveSkipPractice(void) {
    toggles[TOGGLE_CAVE_SKIP_PRACTICE] ^= 1;
    return 1;
}

s32 toggleObjectSpawnsOff(void) {
    toggles[TOGGLE_ENEMY_SPAWNS_OFF] ^= 1;
    loadEnemiesBool ^= 1;
    return 1;
}

void enterBossRoom2(s32 unk) {
    int *new_var2;
    int new_var;
    func_800CFDC8(&gPlayerActors[0]);

    if (D_80236974 == 0) {
        func_800C2A00();
        return;
    }

    gGameModeState = 3;
    D_80174878 = unk;
}

s32 teleportToStageBoss(void) {
    // Teleports Player to Current World's Boss Stage
    if (gameModeCurrent == GAME_MODE_OVERWORLD && gCurrentStage < 6) {
        func_800C0CDC(&gPlayerActors[0], -1, 0, 0, 1);
    }
    
    return 1;
}
extern s32 saveStateBackupSize;
void savestateMainStartRecording(void) {
    u32 saveMask;
    wait_on_hardware();
    saveMask = __osDisableInt();

    // save uncompressed state
    optimized_memcpy(ramAddrSavestateDataSlot1,  (void*)ramStartAddr, ramEndAddr - ramStartAddr);

    //save compressed state
    //saveStateBackupSize = compress_lz4_ct_default((void*)ramStartAddr, ramEndAddr - ramStartAddr, savestateBackup);

    __osRestoreInt(saveMask);
    prevCount = osGetCount();
    isSaveOrLoadActive = 0; //allow thread 3 to continue
}

void loadstateRecordingMain(void) {
    u32 saveMask;
    wait_on_hardware();
    saveMask = __osDisableInt();

    // load uncompressed state
    optimized_memcpy((void*)ramStartAddr,  ramAddrSavestateDataSlot1, ramEndAddr - ramStartAddr);

    // load compressed state
    //decompress_lz4_ct_default(ramEndAddr - ramStartAddr, saveStateBackupSize, savestateBackup);

    __osRestoreInt(saveMask);
    prevCount = osGetCount();
    isSaveOrLoadActive = 0; //allow thread 3 to continue
}

s32 ResetTimerToggle(void) {
    toggles[TOGGLE_RTA_TIMER_RESET] ^= 1;
    return 0;
}

s32 StartRecording(void) {
    isMenuActive = 0;

    toggles[TOGGLE_RECORDING] ^= 1;

    //if flipped to off, just exit
    if (toggles[TOGGLE_RECORDING] == 0) {
        return 1;
    }

    inputRecordingBuffer.totalFrameCount = 0;
    toggles[TOGGLE_PLAYBACK] = 0; //turn playback off
    isSaveOrLoadActive = 1;

    osCreateThread(&gCustomThread.thread, 255, (void*)savestateMainStartRecording, NULL,
            gCustomThread.stack + sizeof(gCustomThread.stack), 255);
    osStartThread(&gCustomThread.thread);
    stateCooldown = 5;

    while (isSaveOrLoadActive != 0) {}

    return 1;
}

s32 ExportRecording(void) {
    UINT filebytesread;
    void* test = (void*)&inputRecordingBuffer;
    const void* recordingData = (const void*)test;
    FRESULT fileres;

    fileres = f_open(&sdsavefile, path, FA_OPEN_ALWAYS | FA_WRITE);
    f_write(&sdsavefile, recordingData, ALIGN4(sizeof(inputRecordingBuffer)), &filebytesread);
    f_close(&sdsavefile);

    return 1;
}

s32 ImportRecording(void) {
    UINT filebytesread;
    void* test = (void*)&inputRecordingBuffer;
    const void* recordingData = (const void*)test;
    FRESULT fileres;

    fileres = f_open(&sdsavefile, path, FA_OPEN_ALWAYS | FA_READ);
    f_read(&sdsavefile, (void*)recordingData, ALIGN4(sizeof(inputRecordingBuffer)), &filebytesread);
    f_close(&sdsavefile);
    return 1;
}

s32 PlayRecording(void) {
    isMenuActive = 0;
    if (toggles[TOGGLE_PLAYBACK] == 0) {
        inputRecordingBuffer.framePlaybackIndex = 0;
    }
    toggles[TOGGLE_PLAYBACK] ^= 1;
    toggles[TOGGLE_RECORDING] = 0; //turn recording off
    recordingInputIndex = 0;
    isSaveOrLoadActive = 1;
    osCreateThread(&gCustomThread.thread, 255, (void*)loadstateRecordingMain, NULL,
            gCustomThread.stack + sizeof(gCustomThread.stack), 255);
    osStartThread(&gCustomThread.thread);
    stateCooldown = 5;

    while (isSaveOrLoadActive != 0) {}
    return 1;
}