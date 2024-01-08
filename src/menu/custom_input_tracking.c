#include "common.h"
#include "../../include/menu.h"

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
        if (currPageNo < pageListTotal - 1) {
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

// typedef struct contMain {
//     u16 buttons0;
//     u16 buttons1;
//     u16 buttons2;
//     s16 stickx;
//     s16 sticky;
//     u16 unk_0A; //align
//     f32 stickAngle;
// } contMain; //sizeof 0x10

contMain p1ControllerCopy = {0, 0, 0, 0, 0, 0, 0};

void WritegContMain(s32 i) {
    gContMain[i].stickAngle = CalculateAngleOfVector((f32) gContMain[i].stickx, (f32) gContMain[i].sticky);
    gContMain[i].buttons1 = (gContMain[i].buttons0 ^ D_80175678[i]) & gContMain[i].buttons0;
    gContMain[i].buttons2 = (gContMain[i].buttons0 ^ D_801756C0[i]) & gContMain[i].buttons0;
    D_801756C0[i] = gContMain[i].buttons0;
    if ((gContMain[i].stickx >= -6) && (gContMain[i].stickx < 7)) {
        gContMain[i].stickx = 0;
    }
    
    if ((gContMain[i].sticky >= -6) && (gContMain[i].sticky < 7)) {
        gContMain[i].sticky = 0;
    }

    if (i == 0) {
        updateCustomInputTracking();
        if (isMenuActive == 1) {
            updateMenuInput();
            gContMain[0].buttons0 &= ~A_BUTTON;
            gContMain[0].buttons0 &= ~B_BUTTON;
            gContMain[0].buttons1 &= ~A_BUTTON;
            gContMain[0].buttons1 &= ~B_BUTTON;
            gContMain[0].buttons2 &= ~A_BUTTON;
            gContMain[0].buttons2 &= ~B_BUTTON;
        }
        if ((gContMain[0].buttons0 & R_TRIG) && (currentlyPressedButtons & CONT_UP)) {
            sDebugInt ^= 1;
        } else if ((gContMain[0].buttons0 & R_TRIG) && (currentlyPressedButtons & CONT_DOWN)) {
            isMenuActive ^= 1;
        } else if (currentlyPressedButtons & CONT_DOWN) {
            if (!(isMenuActive)) {
                savestateCurrentSlot ^= 1; //flip from 0 to 1 or vice versa (2 saveslots)
            }
        }
    }
}


void func_8004E784_Hook(contMain* arg0, s32 arg1, s32* arg2, contMain* arg3) {
    contMain* var_s0;
    contMain* var_s1;
    s32 i;

    osRecvMesg(&gEepromMsgQ, NULL, 1);
    osContGetReadData(&D_80175650[0]);
    
    i = 0;

    // if (toggles[TOGGLE_PLAYBACK] == 1) {
    //     if (recordingInputIndex > inputRecordingBuffer.totalFrameCount) {
    //         toggles[TOGGLE_PLAYBACK] ^= 1; //turn recording off as it has reached the end
    //     } else {
    //         gContMain[0] = inputRecordingBuffer.recordingBuffer[recordingInputIndex];
    //         arg0[0] = inputRecordingBuffer.recordingBuffer[recordingInputIndex];
    //         recordingInputIndex++;
    //         i = 1; //skip player 0
    //     }
    // }

    // for each controller
    for (i; i < arg1; i++) {
        if ((arg2 == NULL) || (arg2[i] == 0)) {
            if (D_80175668[i] == -1) {
                Controller_Zero(&gContMain[i]);
                continue;
            }
            gContMain[i].buttons0 = D_80175650[D_80175668[i]].button;
            gContMain[i].stickx = D_80175650[D_80175668[i]].stick_x;
            gContMain[i].sticky = D_80175650[D_80175668[i]].stick_y;
        } else {
            gContMain[i].buttons0 = arg3[i].buttons0;
            gContMain[i].stickx = arg3[i].stickx;
            gContMain[i].sticky = arg3[i].sticky;
        }

        if (toggles[TOGGLE_PLAYBACK] == 1) {
            if (inputRecordingBuffer.framePlaybackIndex > inputRecordingBuffer.totalFrameCount) {
                toggles[TOGGLE_PLAYBACK] = 0;
                WritegContMain(i);
            } else {
                gContMain[i] = inputRecordingBuffer.recordingBuffer[inputRecordingBuffer.framePlaybackIndex++];
            }
            //get inputs from array buffer
        } else {
            WritegContMain(i);
        }

        if (toggles[TOGGLE_RECORDING] == 1 && i == 0) {
            inputRecordingBuffer.recordingBuffer[inputRecordingBuffer.totalFrameCount] = gContMain[0];
            inputRecordingBuffer.totalFrameCount++;
        }

        arg0[i] = gContMain[i];


    }
}