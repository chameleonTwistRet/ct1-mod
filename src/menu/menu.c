#include "common.h"
#include "../../include/menu.h"
#include "xstdio.h"
#include "../../include/mod_main.h"
#include "../../lib/cart.h"
#include "../../lib/ff.h"
#include "../../lib/ffconf.h"
#include "mod.h"

typedef s32 (*menuProc) (void);

// shift x value per print per length of string (8px per letter) then print
#define X_COORD_PER_LETTER 4.5
#define FUNCS_PER_PAGE 6
#define ARRAY_COUNT(arr) (s32)(sizeof(arr) / sizeof(arr[0]))
#define ARRAY_COUNT_INDEX(arr) ARRAY_COUNT(arr) - 1

extern u32 recordingInputIndex;
s32 savestateRecordingSize = 0;

s8 toggleSpawnsOff = 0;  // 0 = on, 1 = off | Objects / Enemies Spawning

s8 toggles[] = {
    2,  // NO_TOGGLE
    //page 0
    1,  // TOGGLE_HIDE_SAVESTATE_TEXT
    0,  // TOGGLE_NO_COMPRESSION_SAVESTATES
    0,  // TOGGLE_DISPLAY_IGT
    0,  // TOGGLE_CUSTOM_DEBUG_TEXT
    0,  // TOGGLE_INFINITE_HEALTH

    // page 4
    0,  // TOGGLE_CAVE_SKIP_PRACTICE
    0,  // TOGGLE_YOLO_PILLAR_PRACTICE
    0,  // TOGGLE_BACKVAULT_PRACTICE
    0,  // TOGGLE_ENEMY_SPAWNS_OFF
    -1, // TOGGLE_LOAD_BOSS

    //page 1
    -1, // TOGGLE_ADV_RNG
    -1, // TOGGLE_REV_RNG
    0, // TOGGLE_SET_SEED_BL
    0, // TOGGLE_SET_SEED_KL
    0, // TOGGLE_SET_SEED_GC
    //0, // TOGGLE_SPEED

    //page 2
    0,  // TOGGLE_RECORDING
    0,  // TOGGLE_PLAYBACK

    //page 3
    1, // TOGGLE_RTA_TIMER_RESET
    1, // TOGGLE_DISPLAY_INDIVIDUAL_ROOM_TIME
    1, // TOGGLE_TAS_COMPARISON
    0, // TOGGLE_FRAME_ADVANCE
};

typedef struct menuPage {
    /* 0x00 */ s32 optionCount;
    /* 0x04 */ s32 pageIndex;
    /* 0x08 */ char* options[FUNCS_PER_PAGE];
    /* 0x28 */ s32 (*menuProc[FUNCS_PER_PAGE]) (void);
    /* 0x48 */ s8 flags[FUNCS_PER_PAGE];
    /* 0x50 */ char*** selectionText;
} menuPage;

menuPage page2 = {
    4, // optionCount
    PAGE_RECORDING, // pageIndex
    { // options
        "Recording",
        "Export Recording",
        "Import Recording",
        "Play Recording"
    },
    { // menuProc
        &StartRecording,
        &ExportRecording,
        &ImportRecording,
        &PlayRecording
    },
    { // flags
        TOGGLE_RECORDING,
        NO_TOGGLE,
        NO_TOGGLE,
        TOGGLE_PLAYBACK
    },
    {
        page0Strings, //update when implemented
    },
};

menuPage page0 = {
    5, //optionCount
    PAGE_JL, //pageIndex
    { //options
        "Savestate Text",
        "No compression savestates",
        "In Game Timer",
        "Custom Debug Text",
        "Infinite Health"
    },
    { //menuProc
        &toggleHideSavestateText,
        &NoCompressionToggle,
        &toggleHideIGT,
        &toggleCustomDebugText,
        &toggleInfiniteHealth
    },
    { //flags
        TOGGLE_HIDE_SAVESTATE_TEXT,
        TOGGLE_NO_COMPRESSION_SAVESTATES,
        TOGGLE_DISPLAY_IGT,
        TOGGLE_CUSTOM_DEBUG_TEXT,
        TOGGLE_INFINITE_HEALTH
    },
    {
        page0Strings,
    }
};

menuPage page4 = {
    5, //optionCount
    PAGE_PRACTICE, //pageIndex
    { //options
        "Cave Skip Practice",
        "Yolo Pillar Practice",
        "Back Vault Practice",
        "Disable Actors",
        "Load Boss\n"
    },
    { //menuProc
        &toggleCaveSkipPractice,
        &toggleYoloPillarPractice,
        &toggleBackVaultPractice,
        &toggleObjectSpawnsOff,
        &teleportToStageBoss,
    },
    { //flags
        TOGGLE_CAVE_SKIP_PRACTICE,
        TOGGLE_YOLO_PILLAR_PRACTICE,
        TOGGLE_BACKVAULT_PRACTICE,
        TOGGLE_ENEMY_SPAWNS_OFF,
        -1
    },
    {
        page4Strings,
    }
};

menuPage page1 = {
    6, //optionCount
    PAGE_MAIN, //pageIndex
    { //options
        "Adv RNG\n",
        "Rev RNG\n",
        "Set BL RNG Seed",
        "Set BL IL RNG Seed",
        "Set KL RNG Seed",
        "Set GC RNG Seed",
        //"Speed\n"
    },
    { //menuProc
        &advanceGuRNG,
        &revGuRNG,
        &setBLSeed,
        &setBLILSeed,
        &setKLSeed,
        &setGCSeed
        //&toggleSpeed
    },
    { //flags
        -1,
        -1,
        TOGGLE_SET_SEED_BL,
        TOGGLE_SET_SEED_IL_BL,
        TOGGLE_SET_SEED_KL,
        TOGGLE_SET_SEED_GC,
        //TOGGLE_REV_RNG
    },
    {
        page1Strings,
    },
};

menuPage page3 = {
    4, // optionCount
    PAGE_MISC, // pageIndex
    { // options
        "STAGE START RESET TIMER",
        "DISPLAY RTA TIME IN ROOM",
        "DISPLAY TAS COMPARISON",
        "FRAME ADVANCE ON P2",
    },
    { // menuProc
        &ResetTimerToggle,
        &DisplayIndividualRoomTimeToggle,
        &DisplayTASComparison,
        &FrameAdvanceToggle
    },
    { // flags
        TOGGLE_RTA_TIMER_RESET,
        TOGGLE_DISPLAY_INDIVIDUAL_ROOM_TIME,
        TOGGLE_TAS_COMPARISON,
        TOGGLE_FRAME_ADVANCE
    },
    {
        page3Strings,
    },
};

menuPage* pageList[] = {
    &page0,
    &page4,
    &page1,
    &page3,
};

void colorTextWrapper(s32* color) {
    SetTextGradient(color[0], color[1], color[2], color[3], color[4], color[5], color[6], color[7],
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

s32 textOrangeColor[] = {
    0xFF, 0xFF, 0U, 0xFF,
    0xFF, 0x00, 0x00, 0xFF,
    0xFF, 0xFF, 0x00, 0xFF,
    0xFF, 0x00, 0x00, 0xFF
};

s32 textPurpleColor[] = {
    0xFF, 0x40, 0xFF, 0xFF,
    0x00, 0x40, 0xFF, 0xFF,
    0xFF, 0x40, 0xFF, 0xFF,
    0xFF, 0x40, 0xFF, 0xFF
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

        if (toggles[currPage->flags[i]] == 0) {
            colorTextWrapper(textRedColor);
        } else {
            colorTextWrapper(textGreenMatColor);
        }

        if (currPage->selectionText[i][toggles[currPage->flags[i]]] != 0) {
            _sprintf(menuOptionBuffer, currPage->selectionText[i][toggles[currPage->flags[i]]]);
        }

        convertAsciiToText(&menuOptionBufferConverted, (char*)&menuOptionBuffer);
        textPrint(xPos + (strLength * X_COORD_PER_LETTER), (yPos + (i * 15.0f)), 0.5f, &menuOptionBufferConverted, 1);  

    }
}

