#include "common.h"
#include "../include/mod_main.h"
#include "../include/menu.h"

void Rumble_StopAll(void);                                 /* extern */
void func_8007B174(void);                                  /* extern */
void func_8008C440(void);                                  /* extern */
void func_8008C464(void);                                  /* extern */
void func_8008C4E8(void);                                  /* extern */
void func_8008C610(void);                                  /* extern */
void func_8008C698(void);                                  /* extern */
void func_8008C6D4(void);                                  /* extern */
void func_800A7844(void);                                  /* extern */
void func_800A78D0(void);                                  /* extern */
extern s16 D_800FF5C4;
extern s32 D_800FF5C8;
extern s16 D_800FF5CC;
extern s16 D_800FF5D8;
extern char D_8010D530[];
extern char D_8010D540[];
extern char D_8010D544[];
extern char D_8010D548[];
extern char D_8010D54C[];
extern char D_8010D550[];
extern char D_8010D558[];
extern char D_8010D55C[];
extern char D_8010D560[];
extern char D_8010D564[];
extern OSMesgQueue D_801192B8;
extern OSMesgQueue D_801192D0;
extern void* D_801B30A0;
extern OSTimer D_801B3148;
extern OSMesgQueue D_801B35A0;

void func_800C0CDC_Hook(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    if (gZoneCollisions[arg1].unk7C != 0) {
        gGameModeState = 3;
        switch (gCurrentStage) {                    /* switch 1 */
        case 0:                                     /* switch 1 */
            D_80174878 = 8;
            return;
        case 1:                                     /* switch 1 */
            D_80174878 = 9;
            return;
        case 2:                                     /* switch 1 */
            D_80174878 = 0xA;
            return;
        case 3:                                     /* switch 1 */
            D_80174878 = 0xB;
            return;
        case 4:                                     /* switch 1 */
            D_80174878 = 0xC;
            return;
        case 5:                                     /* switch 1 */
            D_80174878 = 0xD;
            return;
        case 15:                                    /* switch 1 */
            switch (arg1) {                         /* switch 2 */
            case 1:                                 /* switch 2 */
                D_80174878 = 8;
                return;
            case 3:                                 /* switch 2 */
                D_80174878 = 9;
                return;
            case 2:                                 /* switch 2 */
                D_80174878 = 0xA;
                return;
            case 4:                                 /* switch 2 */
                D_80174878 = 0xB;
                return;
            case 5:                                 /* switch 2 */
                D_80174878 = 0xC;
                return;
            case 6:                                 /* switch 2 */
                D_80174878 = 0xD;
                return;
            }
            break;
        default:                                    /* switch 1 */
            gGameModeState = 2;
        }
    }
    func_800C0760(arg1);
    ChameleonFromDoor(arg0, arg1, arg2, arg3, arg4);
    func_800BFCD0();
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

        if (toggles[TOGGLE_RECORDING] == 1 && i == 0) {
            inputRecordingBuffer.recordingBuffer[0] = gContMain[0];
            if ((s32)gContMain[0].stickAngle == 0xFFFFFFFF) {
                recordingErrorMessageStick = 1;
            }
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
                savestateCurrentSlot ^= 1; //flip from 0 to 1 or vice versa (2 saveslots)
            }
        }

        arg0[i] = gContMain[i];
    }
}

void videoproc_Hook(s32 arg0) {
    s32 sp54;
    OSMesg sp58;
    s32 var_s2;

    var_s2 = 1;
    func_800A7844();
    osCreateMesgQueue(&D_801B3120, &D_801B30A0, 0x14);
    osSetEventMesg(0xEU, &D_801B3120, (void* )7);
    osSetEventMesg(9U, &D_801B3120, (void* )2);
    osSetEventMesg(4U, &D_801B3120, (void* )1);
    osViSetEvent(&D_801B3120, NULL, 2U);
    func_8008C610();

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
                    DummiedPrintf(D_8010D530);
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
                DummiedPrintf(D_8010D540);
                func_8008C6D4();
                D_800FF5F0 = 0;
                osSendMesg(&D_801192B8, (void* )1, 0);
            } else if (D_800FF5F0 == (s16) 1) {
                DummiedPrintf(D_8010D544);
                func_8008C464();
                D_800FF5EC = 0;
                osSendMesg(&D_801192B8, (void* )1, 0);
            } else {
                DummiedPrintf(D_8010D548);
                func_8008C464();
                D_800FF5EC = 0;
            }
            if (D_800FF5C8 != 0) {
                osSendMesg(&D_801B3120, (void* )3, 0);
            }
            continue;
        case 2:
            DummiedPrintf(D_8010D54C);
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
                DummiedPrintf(D_8010D550);
            } else if (D_800FF5F0 != 0) {
                D_800FF5C8 = 1;
                DummiedPrintf(D_8010D558);
            } else {
                DummiedPrintf(D_8010D55C);
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
                DummiedPrintf(D_8010D560);
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
        DummiedPrintf(D_8010D564);
        continue;   
    }
}