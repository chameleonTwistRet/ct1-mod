#include "common.h"
#include "../../include/menu.h"
#include "xstdio.h"
#include "../../include/mod_main.h"
#include "../../lib/cart.h"
#include "../../lib/ff.h"
#include "../../lib/ffconf.h"
#include "mod.h"

extern u32 recordingInputIndex;

s32 savestateRecordingSize = 0;
s32 currPageNo = 0;
s32 currOptionNo = 0;

char menuOptionBuffer[50] = { 0 };  // Buffer for menu options text
char menuOptionBufferConverted[100] = { 0 };  // Buffer for menu options text converted to chameleon twist format

s32 isMenuActive = 0;
s32 inputCooldown = 0;

s8 toggleSpawnsOff = 0;  // 0 = on, 1 = off | Objects / Enemies Spawning

menuPage* pageList[] = {
    &page0,
    &page4,
    &page1,
    &page3,
};

s32 pageListTotal = ARRAY_COUNT(pageList);

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
    0, // TOGGLE_SET_SEED_JL
    0, // TOGGLE_SET_SEED_BL
    0, // TOGGLE_SET_SEED_IL_BL
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

