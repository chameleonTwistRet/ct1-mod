#include "common.h"
#include "../include/menu.h"
#include "xstdio.h"
#include "../include/mod_main.h"
#include "../lib/cart.h"
#include "../lib/ff.h"
#include "../lib/ffconf.h"
#include "../include/sd_toggle.h"

s8 toggleSpawnsOff = 0;  // 0 = on, 1 = off | Objects / Enemies Spawning
extern void* crash_screen_copy_to_buf(void* dest, const char* src, u32 size);
void textPrint(f32 xPos, f32 yPos, f32 scale, void *text, s32 num);
int __osPiDeviceBusy(void);
s32 compress_lz4_ct_default(const u8* srcData, int srcSize, u8* bufferAddr);
void decompress_lz4_ct_default(int srcSize, int savestateCompressedSize, u8* compressBuffer);
extern s32 isMenuActive;
extern CustomThread gCustomThread;
void func_800C2A00(void);
u32 guRandomRev(void);

#define X_COORD_PER_LETTER 4.5
// shift x value per print per length of string (8px per letter) then print ON/OFF

#define FUNCS_PER_PAGE 8
#define FUNCS_PER_LAST_PAGE 3

typedef s32 (*menuProc) (void);
#define ARRAY_COUNT(arr) (s32)(sizeof(arr) / sizeof(arr[0]))
#define ARRAY_COUNT_INDEX(arr) ARRAY_COUNT(arr) - 1

s8 toggles[] = {
    2,  // NO_TOGGLE
    //page 0
    1,  // TOGGLE_HIDE_SAVESTATE_TEXT
    0,  // TOGGLE_HIDE_IGT
    0,  // TOGGLE_CUSTOM_DEBUG_TEXT
    0,  // TOGGLE_CAVE_SKIP_PRACTICE
    0,  // TOGGLE_ENEMY_SPAWNS_OFF

    //page 1
    -1, // TOGGLE_LOAD_BOSS
    0,  // TOGGLE_INFINITE_HEALTH
    -1, // TOGGLE_ADV_RNG
    -1, // TOGGLE_REV_RNG

    //page 2
    0,  // TOGGLE_RECORDING
    0,  // TOGGLE_PLAYBACK
};

s32 toggleHideSavestateText(void) {
    toggles[TOGGLE_HIDE_SAVESTATE_TEXT] ^= 1;
    return 1;
}

s32 toggleHideIGT(void) {
    toggles[TOGGLE_HIDE_IGT] ^= 1;
    return 1;
}

s32 toggleInfiniteHealth(void) {
    toggles[TOGGLE_INFINITE_HEALTH] ^= 1;
    return 1;
}

s32 toggleCustomDebugText(void) {
    //8016AA9C
    toggles[TOGGLE_CUSTOM_DEBUG_TEXT]++;
    if (toggles[TOGGLE_CUSTOM_DEBUG_TEXT] >= 5) {
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

void func_800C0CDC(playerActor* arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern unkIsChange isChange;
void func_800CFDC8(playerActor*);

s32 bossVar = 0x0B;

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

typedef struct menuPage {
    /* 0x08 */ s32 optionCount;
    /* 0x0C */ s32 pageIndex;
    /* 0x10 */ char* options[FUNCS_PER_PAGE];
    /* 0x30 */ s32 (*menuProc[FUNCS_PER_PAGE]) (void);
    /* 0x50 */ s8 flags[FUNCS_PER_PAGE];
} menuPage;

extern u32 recordingInputIndex;
s32 savestateRecordingSize = 0;

void savestateMainStartRecording(void) {
    // u32 saveMask;
    // //wait on rsp
    // while (__osSpDeviceBusy() == 1) {}

    // //wait on rdp
    // while ( __osDpDeviceBusy() == 1) {}

    // //wait on SI
    // while (__osSiDeviceBusy() == 1) {}

    // //wait on PI
    // while (__osPiDeviceBusy() == 1) {}

    // //invalidate caches
    // osInvalICache((void*)0x80000000, 0x2000);
	// osInvalDCache((void*)0x80000000, 0x2000);

    // saveMask = __osDisableInt();

    // savestateRecordingSize = compress_lz4_ct_default((void*)ramStartAddr, ramEndAddr - ramStartAddr, ramAddrSavestateRecording);
    
    // __osRestoreInt(saveMask);
    isSaveOrLoadActive = 0; //allow thread to continue
}

void loadstateRecordingMain(void) {
    // u32 saveMask;
    // //wait on rsp
    // while (__osSpDeviceBusy() == 1) {}

    // //wait on rdp
    // while ( __osDpDeviceBusy() == 1) {}

    // //wait on SI
    // while (__osSiDeviceBusy() == 1) {}

    // //wait on PI
    // while (__osPiDeviceBusy() == 1) {}

    // //invalidate caches
    // osInvalICache((void*)0x80000000, 0x2000);
	// osInvalDCache((void*)0x80000000, 0x2000);
    // saveMask = __osDisableInt();

    // decompress_lz4_ct_default(ramEndAddr - ramStartAddr, savestateRecordingSize, ramAddrSavestateRecording);

    // __osRestoreInt(saveMask);
    isSaveOrLoadActive = 0; //allow thread 3 to continue
}

s32 StartRecording(void) {
    isMenuActive = 0;
    toggles[TOGGLE_RECORDING] ^= 1;
    toggles[TOGGLE_PLAYBACK] = 0; //turn playback off
    recordingInputIndex = 0;
        
    isSaveOrLoadActive = 1;
    osCreateThread(&gCustomThread.thread, 255, (void*)savestateMainStartRecording, NULL,
            gCustomThread.stack + sizeof(gCustomThread.stack), 255);
    osStartThread(&gCustomThread.thread);
    stateCooldown = 5;

    while (isSaveOrLoadActive != 0) {}

    return 1;
}

extern FATFS FatFs;
extern char *path; //"ct1State.bin"; //example file for SD card writing
extern FIL sdsavefile;


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

menuPage page2 = {
    4, //optionCount
    PAGE_RECORDING, //pageIndex
    { //options
        "Recording",
        "Export Recording",
        "Import Recording",
        "Play Recording"
    },
    { //menuProc
        &StartRecording,
        &ExportRecording,
        &ImportRecording,
        &PlayRecording
    },
    { //flags
        TOGGLE_RECORDING,
        NO_TOGGLE,
        NO_TOGGLE,
        TOGGLE_PLAYBACK
    }
};

s32 advanceGuRNG(void) {
    guRandom(); //advance rng once
    return 0;
}

s32 revGuRNG(void) {
    guRandomRev(); //advance rng backwards
    return 0;
}

menuPage page1 = {
    4, //optionCount
    PAGE_MAIN, //pageIndex
    { //options
        "Load Boss\n",
        "Infinite Health\n",
        "Adv RNG\n",
        "Rev RNG\n"
    },
    { //menuProc
        &teleportToStageBoss,
        &toggleInfiniteHealth,
        &advanceGuRNG,
        &revGuRNG
    },
    { //flags
        -1,
        TOGGLE_INFINITE_HEALTH,
        -1,
        -1
    }
};

menuPage page0 = {
    5, //optionCount
    PAGE_JL, //pageIndex
    { //options
        "Savestate Text\n",
        "In Game Timer\n",
        "Custom Debug Text\n",
        "Cave Skip Practice\n",
        "Disable Actors\n"
    },
    { //menuProc
        &toggleHideSavestateText,
        &toggleHideIGT,
        &toggleCustomDebugText,
        &toggleCaveSkipPractice,
        &toggleObjectSpawnsOff
    },
    { //flags
        TOGGLE_HIDE_SAVESTATE_TEXT,
        TOGGLE_HIDE_IGT,
        TOGGLE_CUSTOM_DEBUG_TEXT,
        TOGGLE_CAVE_SKIP_PRACTICE,
        TOGGLE_ENEMY_SPAWNS_OFF
    }
};

menuPage* pageList[] = {
    &page0,
    &page1,
    //&page2,
};

//testing func ptr
s32 printCustomDebugText(void);

void colorTextWrapper(s32* color) {
    setTextGradient(color[0], color[1], color[2], color[3], color[4], color[5], color[6], color[7],
             color[8], color[9], color[10], color[11], color[12], color[13], color[14], color[15]);
}

s32 textCyanColor[] = {
    0x2A, 0xEE, 0xE9, 0xFF, // top
    0x00, 0xC0, 0xDA, 0xFF, // bottom
    0x2A, 0xEE, 0xE9, 0xFF, // top
    0x00, 0xC0, 0xDA, 0xFF  // bottom
};

// Gradient
s32 textGreenColor[] = {
    0x0A, 0xFF, 0x00, 0xFF, // top
    0xFF, 0xFF, 0xFF, 0xFF, // bottom
    0x0A, 0xFF, 0x00, 0xFF, // top
    0xFF, 0xFF, 0x00, 0x0F  // bottom
};

s32 textGreenMatColor[] = {
    0x00, 0xFF, 0x00, 0xFF, // top
    0x00, 0xFF, 0x00, 0xFF, // bottom
    0x00, 0xFF, 0x00, 0xFF, // top
    0x00, 0xFF, 0x00, 0xFF  // bottom
};

s32 textWhiteColor[] = {
    0xFF, 0xFF, 0xFF, 0xFF, // top
    0xFF, 0xFF, 0xFF, 0xFF, // bottom
    0xFF, 0xFF, 0xFF, 0xFF, // top
    0xFF, 0xFF, 0xFF, 0xFF  // bottom
};

s32 textRedColor[] = {
    0xFF, 0x00, 0x00, 0xFF, // top
    0xFF, 0x00, 0x00, 0xFF, // bottom
    0xFF, 0x00, 0x00, 0xFF, // top
    0xFF, 0x00, 0x00, 0xFF  // bottom
};

s32 currPageNo = 0;
s32 currOptionNo = 0;

char menuOptionBuffer[50] = { 0 };  // Buffer for menu options text
char menuOptionBufferConverted[100] = { 0 };  // Buffer for menu options text converted to chameleon twist format

s32 isMenuActive = 0;
s32 inputCooldown = 0;

s32 previouslyHeldButtons = 0;

void updateCustomInputTracking(void) {
    s32 temp;
    temp = previouslyHeldButtons ^ gContMain[0].buttons0; // if holding this frame and last frame, button wasn't pressed
    temp &= gContMain[0].buttons0; // filter out any released buttons
    currentlyPressedButtons = temp;

    previouslyHeldButtons = gContMain[0].buttons0;
    previouslyPressedButtons = currentlyPressedButtons; // store current frame's button presses
}

void updateMenuInput(void) {
    if (currentlyPressedButtons & CONT_UP) {
        if (currOptionNo > 0) {
            currOptionNo--;
            //playSound(0x2A, (void*)0x80168DA8, 0);
        } else {
            currOptionNo = pageList[currPageNo]->optionCount - 1; //wrap menu
        }
    }
    else if (currentlyPressedButtons & CONT_DOWN) {
        if (currOptionNo < pageList[currPageNo]->optionCount - 1) {
            currOptionNo++;
            //playSound(0x2A, (void*)0x80168DA8, 0);
        } else {
            currOptionNo = 0; //wrap menu
        }
    }
    else if (currentlyPressedButtons & CONT_LEFT) {
        if (currPageNo > 0) {
            currPageNo--;
            currOptionNo = 0;
        }
    }
    else if (currentlyPressedButtons & CONT_RIGHT) {
        if (currPageNo < ARRAY_COUNT_INDEX(pageList)) {
            currPageNo++;
            currOptionNo = 0;
        }
    }
    else if (currentlyPressedButtons & B_BUTTON) {
        isMenuActive = 0;
    }
    else if (currentlyPressedButtons & A_BUTTON) {
        pageList[currPageNo]->menuProc[currOptionNo]();
    }
}

void pageMainDisplay(s32 currPageNo, s32 currOptionNo) {
    int i;
    menuPage* currPage = pageList[currPageNo];
    f32 xPos = 20.0f;
    f32 yPos = 35.0f;
    s32 strLength;
    
    for (i = 0; i < currPage->optionCount; i++) {
        _bzero(&menuOptionBuffer, sizeof(menuOptionBuffer)); //clear buffer
        _sprintf(menuOptionBuffer, "%s", currPage->options[i]);
        _bzero(&menuOptionBufferConverted, sizeof(menuOptionBufferConverted)); //clear buffer 2
        convertAsciiToText(&menuOptionBufferConverted, (char*)&menuOptionBuffer);
        strLength = strlen((u8*)&menuOptionBufferConverted);

        
        if (i == currOptionNo) {
            colorTextWrapper(textCyanColor);
        }

        if (currPage->flags[i] == -1) {
            textPrint(xPos, (yPos + (i * 15.0f)), 0.5f, &menuOptionBufferConverted, 1);
            continue;
        }

        //get offset into toggle array we should read
        textPrint(xPos, (yPos + (i * 15.0f)), 0.5f, &menuOptionBufferConverted, 1);
        
        //clear buffers
        _bzero(&menuOptionBuffer, sizeof(menuOptionBuffer));
        _bzero(&menuOptionBufferConverted, sizeof(menuOptionBufferConverted));

        if (toggles[currPage->flags[i]] >= 1) {
            colorTextWrapper(textGreenMatColor);
            _sprintf(menuOptionBuffer, "ON");
        } else if (toggles[currPage->flags[i]] == 0) {
            colorTextWrapper(textRedColor);
            _sprintf(menuOptionBuffer, "OFF");
        } else {
            continue;
        }

        convertAsciiToText(&menuOptionBufferConverted, (char*)&menuOptionBuffer);
        textPrint(xPos + (strLength * X_COORD_PER_LETTER), (yPos + (i * 15.0f)), 0.5f, &menuOptionBufferConverted, 1);  

    }
}

