#include "common.h"
#include "../include/mod_main.h"
#include "../include/menu.h"

extern InputRecording inputRecordingBuffer;
extern menuPage* pageList[];
extern s32 pageListTotal;

u16 prevButtons = 0;
u16 buttons = 0;

void updateMenuInput(void) {
    buttons = (gContMain[0].buttons0 ^ prevButtons) & gContMain[0].buttons0;

    //done this way so it still works while playing back a recording
    if ((gContMain[0].buttons0 & (R_TRIG)) && (buttons & (D_JPAD))) {
        isMenuActive ^= 1;
    }
    
    //if in playback mode, check if user wants to enter inputs starting at X index
    if (toggles[TOGGLE_PLAYBACK] == 1) {
        if ((gContMain[0].buttons0 & (R_TRIG)) && (buttons & (U_JPAD))) {
            toggles[TOGGLE_PLAYBACK] = 0;
            toggles[TOGGLE_RECORDING] = 1;
    
            //start recording inputs from this point forward
            inputRecordingBuffer.totalFrameCount = inputRecordingBuffer.framePlaybackIndex;
        }
    }

    if (isMenuActive == 0) {
        return;
    }

    if (buttons & CONT_UP) {
        if (currOptionNo > 0) {
            currOptionNo--;
        } else {
            currOptionNo = pageList[currPageNo]->optionCount - 1; //wrap menu
        }
    }
    else if (buttons & CONT_DOWN) {
        if (currOptionNo < pageList[currPageNo]->optionCount - 1) {
            currOptionNo++;
        } else {
            currOptionNo = 0; //wrap menu
        }
    }
    else if (buttons & CONT_LEFT) {
        if (currPageNo > 0) {
            currPageNo--;
            currOptionNo = 0;
        }
    }
    else if (buttons & CONT_RIGHT) {
        if (currPageNo < pageListTotal - 1) {
            currPageNo++;
            currOptionNo = 0;
        }
    }
    else if (buttons & B_BUTTON) {
        isMenuActive = 0;
    }
    else if (buttons & A_BUTTON) {
        pageList[currPageNo]->menuProc[currOptionNo]();
    }
}

s32 firstCall = 0;

void check_menu_boot_inputs(s32 i) {
    if (i == 0) {
        updateMenuInput();
        prevButtons = gContMain[0].buttons0;
    }
    
    if (firstCall != 0 || i != 0) {
        return;
    }

    firstCall = 1;
    if (gContMain[0].buttons0 == 0x0010) { //R
        toggles[TOGGLE_PLAYBACK] = 0;
        toggles[TOGGLE_RECORDING] = 1;
        inputRecordingBuffer.totalFrameCount = 0;
    } else if (gContMain[0].buttons0 == 0x0020) { //L
        toggles[TOGGLE_RECORDING] = 0;
        toggles[TOGGLE_PLAYBACK] = 1;
        inputRecordingBuffer.framePlaybackIndex = 0;
    }/*
    else if (gContMain[0].buttons0 == 0x0800) { //Dpad-Up
        //append mode
        append_mode = 1;
        toggles[TOGGLE_RECORDING] = 0;
        toggles[TOGGLE_PLAYBACK] = 1;
        inputRecordingBuffer.framePlaybackIndex = 0;        
    }
    */
}
