#include "common.h"
#include "xstdio.h"
#include "../include/menu.h"
#include "../include/mod_main.h"

extern OSContPad D_80175650[];
extern contMain gContMain[4];
#define NULL 0

// u16 buttons0;
// u16 buttons1;
// u16 buttons2;
// s16 stickx;
// s16 sticky;

s32 isMenuActive = 0;
void WriteFileToSDCard(void);
void check_menu_boot_inputs(s32 i);

//if first button is held and second button pressed, flip bool var
// #define IF_HELD_AND_PRESSED(R_TRIG, U_JPAD, var) \
//     if ((gContMain[0].buttons0 & (R_TRIG)) && (gContMain[0].buttons2 & (U_JPAD))) { var ^= 1;}

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

extern void* D_801B30A0;
extern OSTimer D_801B3148;
extern OSMesgQueue D_801192B8;
extern s32 D_800FF5C8;
extern s16 D_800FF5CC;
extern s16 D_800FF5D8;
extern OSMesgQueue D_801192D0;
extern s16 D_800FF5C4;
extern OSMesgQueue D_801B35A0;

void videoproc_Hook(s32 arg0) {
    s32 sp54;
    OSMesg sp58;
    s32 var_s2;

    var_s2 = 1;
    // func_800A7844();
    // osCreateMesgQueue(&D_801B3120, &D_801B30A0, 0x14);
    // osSetEventMesg(0xEU, &D_801B3120, (void* )7);
    // osSetEventMesg(9U, &D_801B3120, (void* )2);
    // osSetEventMesg(4U, &D_801B3120, (void* )1);
    // osViSetEvent(&D_801B3120, NULL, 2U);
    // func_8008C610();

    while (1) {
        while (isSaveOrLoadActive != 0) {}
        osRecvMesg(&D_801B3120, &sp58, 1);
        if ((u32) sp58 >= 8U) {
            continue;
        }
        while (isSaveOrLoadActive != 0) {}
        switch ((u32) sp58) {
        case 0:
            sp54 = var_s2 + 1;
            osSetTimer(&D_801B3148, 0x8F184, 0, &D_801B3120, (void*) 6);
            if (sp54 != 0) {
                if (osSendMesg(&D_801192E8, NULL, 0) == -1) {
                    DummiedPrintf("Gfx送信失敗\n");
                } else {
                    var_s2 ^= 1;
                }
            } else {
                var_s2 ^= 1;
            }
            func_800A78D0();
            continue;
        case 1:
            if (D_800FF5F0 == 2) {
                DummiedPrintf("Ae");
                func_8008C6D4();
                D_800FF5F0 = 0;
                osSendMesg(&D_801192B8, (void* )1, 0);
            } else if (D_800FF5F0 == (s16) 1) {
                DummiedPrintf("Gy");
                func_8008C464();
                D_800FF5EC = 0;
                osSendMesg(&D_801192B8, (void* )1, 0);
            } else {
                DummiedPrintf("Ge");
                func_8008C464();
                D_800FF5EC = 0;
            }
            if (D_800FF5C8 != 0) {
                osSendMesg(&D_801B3120, (void* )3, 0);
            }
            continue;
        case 2:
            DummiedPrintf("D");
            if (D_800FF5CC != 0) {
                D_800FF5CC -= 1;
                if (D_800FF5CC == 0) {
                    osViBlack(0U);
                }
            }
            if (D_800FF5D8 == (s16) 1) {
                D_800FF5D8 = 0;
            }
            osSendMesg(&D_801192D0, (void* )2, 0);
            func_8008C4E8();
            continue;
        case 3:
            if (D_800FF5C4 != 0) {
                DummiedPrintf("Res ");
            } else if (D_800FF5F0 != 0) {
                D_800FF5C8 = 1;
                DummiedPrintf("Sw ");
            } else {
                DummiedPrintf("Gs ");
                D_800FF5C8 = 0;
                osWritebackDCacheAll();
                osSpTaskLoad(D_801B3138);
                osSpTaskStartGo(D_801B3138);
                func_8008C440();
                D_800FF5EC = (s16) 1;
                D_800FF5D8 = (s16) 1;
            }
            continue;
        case 4:
            continue;
        case 5:
            if (D_800FF5C4 == 0) {
                osRecvMesg(&D_801192B8, NULL, 0);
                DummiedPrintf("As");
                D_800FF5F0 = 2;
                osWritebackDCacheAll();
                osSpTaskLoad((OSTask* ) D_801B3140);
                osSpTaskStartGo((OSTask* ) D_801B3140);
                func_8008C698();
            }
            continue;
        case 7:
            D_800FF5C4 = 1;
            osViBlack(1);
            osViSetYScale(1.0f);
            func_8007B174();
            Rumble_StopAll();
            continue;
        }
        D_800FF5F0 = 1;
        if (osSendMesg(&D_801B35A0, NULL, 0) != -1) {
            continue;
        }
        DummiedPrintf("Audio送信失敗\n");
        continue;   
    }
}