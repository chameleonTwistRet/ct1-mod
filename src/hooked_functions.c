#include "common.h"
#include "xstdio.h"
#include "../include/menu.h"
#include "../include/mod_main.h"

extern OSContPad D_80175650[];
extern contMain gContMain[4];
#define NULL 0

s32 isMenuActive = 0;
void check_menu_boot_inputs(s32 i);

//we are going to store global variables in this space
ldiv_t ldiv_Hook(long num, long denom) {
    ldiv_t ret;

    ret.quot = num / denom;
    ret.rem = num - denom * ret.quot;

    if (ret.quot < 0 && ret.rem > 0) {
        ret.quot++;
        ret.rem -= denom;
    }

    return ret;
}

#define curInputFrame inputRecordingBuffer.recordingBuffer[inputRecordingBuffer.totalFrameCount]
#define curPlaybackFrame inputRecordingBuffer.recordingBuffer[inputRecordingBuffer.framePlaybackIndex]

void func_8004E784_Hook(contMain* arg0, s32 arg1, s32* arg2, contMain* arg3) {
    contMain* var_s0;
    contMain* temp_v0_2;
    s32 i;

    osRecvMesg(&gEepromMsgQ, NULL, 1);
    osContGetReadData(&D_80175650[0]);

    for (i = 0; i < arg1; i++) {
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

        check_menu_boot_inputs(i);

        if (toggles[TOGGLE_PLAYBACK] == 1) {
            if (inputRecordingBuffer.framePlaybackIndex == inputRecordingBuffer.totalFrameCount) {
                toggles[TOGGLE_PLAYBACK] == 0;
            } else {
                gContMain[0].buttons0 = curPlaybackFrame.buttons;
                gContMain[0].stickx = curPlaybackFrame.stickx;
                gContMain[0].sticky = curPlaybackFrame.sticky;
                inputRecordingBuffer.framePlaybackIndex++;
            }
        }

        gContMain[i].stickAngle = CalculateAngleOfVector((f32) gContMain[i].stickx, (f32) gContMain[i].sticky);
        gContMain[i].buttons1 = (gContMain[i].buttons0 ^ D_80175678[i]) & gContMain[i].buttons0;
        gContMain[i].buttons2 = (gContMain[i].buttons0 ^ D_801756C0[i]) & gContMain[i].buttons0;

        D_801756C0[i] = gContMain[i].buttons0;

        if (toggles[TOGGLE_RECORDING] == 1 && i == 0) {
            curInputFrame.buttons = gContMain[0].buttons0;
            curInputFrame.stickx = gContMain[0].stickx;
            curInputFrame.sticky = gContMain[0].sticky;
            inputRecordingBuffer.totalFrameCount++;
        }

        if ((gContMain[i].stickx >= -6) && (gContMain[i].stickx < 7)) {
            gContMain[i].stickx = 0;
        }
        
        if ((gContMain[i].sticky >= -6) && (gContMain[i].sticky < 7)) {
            gContMain[i].sticky = 0;
        }

        arg0[i] = gContMain[i];
    }
}