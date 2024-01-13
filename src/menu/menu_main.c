#include "common.h"
#include "../../include/menu.h"
#include "../../include/mod_main.h"

menuPage* pageList[] = {
    &page0,
    &page1,
};

s32 pageListTotal = ARRAY_COUNT(pageList);

// toggles[TOGGLE_PLAYBACK] = 0;
// toggles[TOGGLE_RECORDING] = 1;

s8 toggles[] = {
    2,  // NO_TOGGLE
    //page 0
    1,  // TOGGLE_HIDE_SAVESTATE_TEXT
    //page 1
    0, //TOGGLE_RECORDING
    0, //TOGGLE_PLAYBACK
    0, //EXPORT_RECORDING
    //page 2
    0, //TOGGLE_SAVESTATE_SLOT
};

void pageMainDisplay(s32 currPageNo, s32 currOptionNo) {
    char menuOptionBuffer[32];
    char menuOptionBufferConverted[sizeof(menuOptionBuffer) * 2];
    int i;
    menuPage* currPage = pageList[currPageNo];
    f32 xPos = 20.0f;
    f32 yPos = 35.0f;
    s32 strLength;
    
    for (i = 0; i < currPage->optionCount; i++) {
        _bzero(&menuOptionBuffer, sizeof(menuOptionBuffer)); //clear buffer
        if (currPage->options[i] == 0) {
            _sprintf(menuOptionBuffer, "OPTION NULL");
        } else {
            _sprintf(menuOptionBuffer, "%s", currPage->options[i]);
        }
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