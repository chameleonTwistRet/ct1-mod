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
extern s32 D_802478E0;

typedef struct tempStructDebug {
    char unk0[0x28];
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
}tempStructDebug;

extern s32 sDebugInt;
// typedef struct unk80174880 {
// /* 0x00 */ s32 unk_00[4];
// /* 0x10 */ char unk_10[0x18];
// /* 0x28 */ s32 unk_28[4];
// /* 0x38 */ char unk_38[0x18];
// /* 0x50 */ s32 unk_50[4];
// /* 0x60 */ char unk_60[0x18];
// /* 0x78 */ s32 unk_78;
// /* 0x7C */ s32 unk_7C;
// } unk80174880;

extern unk80174880 D_80174880[];

void Debug_ChangeRoom(void);                               /* extern */
void Debug_ChangeView(void);                               /* extern */
void Debug_MovePlayer(void);                               /* extern */
s32 func_80080C28(f32, f32, f32, f32, f32, f32, s32, s32, s32, s32); /* extern */
extern f32 D_800F070C;
extern f32 D_800F0710;
extern f32 D_800F0714;
extern s32 D_800F0B40;
extern char D_8010C7C4[];
extern char D_8010C7CC[];
extern char D_8010C7D4[];
extern char D_8010C7E0[];
extern char D_8010C7F0[];
extern char D_8010C800[];
extern char D_8010C80C[];
extern char D_8010C81C[];
extern char D_8010C824[];
extern char D_8010C838[];
extern char D_8010C850[];
extern char D_8010C860[];
extern char D_8010C870[];
extern char D_8010C888[];
extern char D_8010C89C[];
extern f32 D_8010C8F4;
extern f32 D_8010C8F8;

extern s32 D_801748A8;
extern s32 D_80174980;
extern tempStructDebug D_801768A0;
extern playerActor* D_80176B74;
extern s32 D_802478E0;
extern Collision gZoneCollisions[];
extern s32 sDebugCodeSeqStep;

void debugMain_Hook(void) {
    volatile s32 sp64;
    unk80174880* var_s0;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f14;
    f32 temp_f16;
    f32 temp_f18;
    f32 temp_f20;
    f32 temp_f20_2;
    f32 temp_f22;
    f32 temp_f2;
    f32 temp_f2_2;
    Collision* temp_v0;
    s32 i;

    if (sDebugInt >= 0) {
        if ((D_80174980 == 2) || (D_80174980 == 3) || (D_80174980 == 4) || (D_80174980 == 5)) {
            Debug_ZeroInt();
        }
        switch (gCurrentStage) {                    /* switch 1 */
        case 1:                                     /* switch 1 */
            D_800F070C = 0.0f;
            D_800F0710 = 0.0f;
            D_800F0714 = 0.0f;
            break;
        case 2:                                     /* switch 1 */
            D_800F070C = 0.0f;
            D_800F0710 = 0.0f;
            D_800F0714 = 0.0f;
            break;
        case 4:                                     /* switch 1 */
            D_800F070C = 0.0f;
            D_800F0710 = 0.0f;
            D_800F0714 = 0.0f;
            break;
        case 3:                                     /* switch 1 */
            D_800F070C = 5000.0f;
            D_800F0710 = -10000.0f;
            D_800F0714 = 0.0f;
            break;
        case 6:                                     /* switch 1 */
            D_800F070C = 0.0f;
            D_800F0710 = 0.0f;
            D_800F0714 = 0.0f;
            break;
        case 5:                                     /* switch 1 */
            D_800F070C = 0.0f;
            D_800F0710 = 0.0f;
            D_800F0714 = 0.0f;
            break;
        case 0:                                     /* switch 1 */
            D_800F070C = 0.0f;
            D_800F0710 = 0.0f;
            D_800F0714 = 0.0f;
            break;
        case 7:                                     /* switch 1 */
            D_800F070C = 0.0f;
            D_800F0710 = 0.0f;
            D_800F0714 = 0.0f;
            break;
        case 8:                                     /* switch 1 */
            D_800F070C = 0.0f;
            D_800F0710 = 0.0f;
            D_800F0714 = 0.0f;
            break;
        default:                                    /* switch 1 */
            D_800F070C = 0.0f;
            D_800F0710 = 0.0f;
            D_800F0714 = 0.0f;
            break;
        }
        
        if ((gContMain->buttons0 & 0x20) && (sDebugInt == 1)) {
            setPrimColor(0, 0, 0, 0x80);
            printUISprite(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 80.0f, 192.0f, 0.0f, 0);
            SetTextGradient(0xBF, 0xAU, 0, 0xFF, 0xC8, 0xC8, 0, 0xFF, 0xBF, 0xA, 0, 0xFF, 0xC8, 0xC8, 0, 0xFF);
            switch (gCurrentStage) {                /* switch 2 */
            case 1:                                 /* switch 2 */
                PrintTextWrapper(32.0, 32.0, 0.0f, 1.0f, "ＡＮＴ", 1);
                printNumber(32.0, 64.0, 0.0f, 1.0f, (f32) gCurrentZone, 2, 0);
                break;
            case 2:                                 /* switch 2 */
                PrintTextWrapper(32.0, 32.0, 0.0f, 1.0f, "ＢＯＭ", 1);
                printNumber(32.0f, 64.0, 0.0f, 1.0f, (f32) gCurrentZone, 2, 0);
                break;
            case 4:                                 /* switch 2 */
                PrintTextWrapper(32.0, 32.0, 0.0f, 1.0f, "ＫＩＤＳ", 1);
                printNumber(32.0, 64.0, 0.0f, 1.0f, (f32) gCurrentZone, 2, 0);
                break;
            case 3:                                 /* switch 2 */
                PrintTextWrapper(32.0, 32.0f, 0.0f, 1.0f, "ＤＥＳＥＲＴ", 1);
                printNumber(32.0, 64.0f, 0.0f, 1.0f, (f32) gCurrentZone, 2, 0);
                break;
            case 6:                                 /* switch 2 */
                PrintTextWrapper(32.0, 32.0, 0.0f, 1.0f, "ＯＰＥＮＩＮＧ", 1);
                printNumber(32.0f, 64.0f, 0.0f, 1.0f, (f32) gCurrentZone, 2, 0);
                break;
            case 5:                                 /* switch 2 */
                PrintTextWrapper(32.0, 32.0f, 0.0f, 1.0f, "ＧＨＯＳＴ", 1);
                printNumber(32.0, 64.0, 0.0f, 1.0f, (f32) gCurrentZone, 2, 0);
                break;
            case 0:                                 /* switch 2 */
                PrintTextWrapper(32.0, 32.0, 0.0f, 1.0f, "ＪＵＮＧＬＥ", 1);
                if (D_80236974 == 0) {
                    printNumber(32.0f, 64.0f, 0.0f, 1.0f, (f32) (gCurrentZone + D_800F0B40), 2, 0);
                } else {
                    printNumber(32.0, 64.0f, 0.0f, 1.0f, (f32) gCurrentZone, 2, 0);
                    D_800F0B40 = D_802478E0;
                }
                break;
            case 7:                                 /* switch 2 */
                PrintTextWrapper(32.0f, 32.0f, 0.0f, 1.0f, "ＶＳ", 1);
                printNumber(32.0f, 64.0f, 0.0f, 1.0f, (f32) gCurrentZone, 2, 0);
                break;
            case 8:                                 /* switch 2 */
                PrintTextWrapper(32.0f, 32.0f, 0.0f, 1.0f, "ＴＲＡＮＩＮＩＧ", 1);
                printNumber(32.0f, 64.0f, 0.0f, 1.0f, (f32) gCurrentZone, 2, 0);
                break;
            case 9:                                 /* switch 2 */
                PrintTextWrapper(32.0f, 32.0f, 0.0f, 1.0f, "ＪＵＮＧＬＥＢＯＳＳ", 1);
                printNumber(32.0f, 64.0f, 0.0f, 1.0f, (f32) gCurrentZone, 2, 0);
                break;
            case 10:                                /* switch 2 */
                PrintTextWrapper(32.0f, 32.0f, 0.0f, 1.0f, "ＡＮＴＢＯＳＳ", 1);
                printNumber(32.0f, 64.0f, 0.0f, 1.0f, (f32) gCurrentZone, 2, 0);
                break;
            case 11:                                /* switch 2 */
                PrintTextWrapper(32.0f, 32.0f, 0.0f, 1.0f, "ＢＯＭＢＯＳＳ", 1);
                printNumber(32.0f, 64.0f, 0.0f, 1.0f, (f32) gCurrentZone, 2, 0);
                break;
            case 12:                                /* switch 2 */
                PrintTextWrapper(32.0f, 32.0f, 0.0f, 1.0f, "ＤＥＳＥＲＴＢＯＳＳ", 1);
                printNumber(32.0f, 64.0f, 0.0f, 1.0f, (f32) gCurrentZone, 2, 0);
                break;
            case 13:                                /* switch 2 */
                PrintTextWrapper(32.0f, 32.0f, 0.0f, 1.0f, "ＫＩＤＳＢＯＳＳ", 1);
                printNumber(32.0f, 64.0f, 0.0f, 1.0f, (f32) gCurrentZone, 2, 0);
                break;
            case 14:                                /* switch 2 */
                PrintTextWrapper(32.0, 32.0, 0.0f, 1.0f, "ＧＨＯＳＴＢＯＳＳ", 1);
                printNumber(32.0, 64.0f, 0.0f, 1.0f, (f32) gCurrentZone, 2, 0);
                break;
            }
            printNumberWR(32.0, 96.0f, 0.0f, 0.5, D_80176B74->pos.x + D_800F070C, 6, 0);
            printNumberWR(32.0, 112.0f, 0.0f, 0.5, D_80176B74->pos.y + D_800F0710, 6, 0);
            printNumberWR(32.0, 128.0f, 0.0f, 0.5, D_80176B74->pos.z + D_800F0714, 6, 0);
            temp_f0 = D_801768A0.unk34 - D_801768A0.unk28;
            temp_f2 = D_801768A0.unk38 - D_801768A0.unk2C;
            temp_f14 = D_801768A0.unk3C - D_801768A0.unk30;
            //printNumberWR(32.0, 160.0f, 0.0f, 0.5, __sqrtf(SQ(temp_f0) + SQ(temp_f2) + SQ(temp_f14)), 6, 0);
            
            // for (i = 0, sp64 = 0x20; i < 10; i++) {
            //     temp_f20 = (f32)sp64;
            //     printNumberWR(200.0f, temp_f20, 0.0f, 0.5, D_80174880->unk_00[i], 4, 1);
            //     printNumberWR(230.0f, temp_f20, 0.0f, 0.5, D_80174880->unk_28[i], 4, 1);
            //     printNumberWR(260.0f, temp_f20, 0.0f, 0.5f, D_80174880->unk_50[i], 4, 1);  
            //     sp64 += 0x0c;
            // }

            // printNumberWR(290.0f, 32.0, 0.0f, 0.5, D_80174880->unk_78, 4, 1);
            // printNumberWR(290.0f, 44.0f, 0.0f, 0.5, D_80174880->unk_7C, 4, 1);
            temp_v0 = &gZoneCollisions[gCurrentZone];
            temp_f0_2 = (temp_v0->rect_30.max.x - temp_v0->rect_30.min.x) / 2;
            temp_f16 = (temp_v0->rect_30.max.y - temp_v0->rect_30.min.y) / 2;
            temp_f20_2 = (temp_v0->rect_30.max.z - temp_v0->rect_30.min.z) / 2;
            func_80080C28(temp_v0->rect_30.min.x + temp_f0_2,
                temp_v0->rect_30.min.y + temp_f16, temp_v0->rect_30.min.z + temp_f20_2,
                temp_f0_2, temp_f16, temp_f20_2, 0x64, 0x64, 0x64, 0x64);
        }
        if (sDebugInt == 0) {
            if (func_80055E5C(U_JPAD) != 0) {
                sDebugCodeSeqStep = 1;
            }
            if ((func_80055E5C(D_JPAD) != 0) && (sDebugCodeSeqStep > 0)) {
                sDebugCodeSeqStep = 2;
            }
            if ((func_80055E5C(L_JPAD) != 0) && (sDebugCodeSeqStep >= 2)) {
                sDebugCodeSeqStep = 3;
            }
            if ((func_80055E5C(R_JPAD) != 0) && (sDebugCodeSeqStep >= 3)) {
                sDebugCodeSeqStep = 4;
                sDebugInt = 1;
            }
        } else {
            Debug_MovePlayer();
            Debug_ChangeRoom();
            Debug_NOOP();
            Debug_ChangeView();
        }
    }    
}

void Debug_ChangeRoom_Hook(void) {
    s32 shouldChangeRoom = FALSE;

    //if c-left
    if (func_80055E5C(2) != 0) {
        shouldChangeRoom = TRUE;
        gCurrentZone--;
    }
    //if c-right
    if (func_80055E5C(1) != 0) {
        shouldChangeRoom = TRUE;
        gCurrentZone++;
    }
    //if less than 0, go to current room count max - 1
    if (gCurrentZone < 0) {
        gCurrentZone = D_802478E0 - 1;
    }
    //if greater than or equal to max, set to 0
    if (gCurrentZone >= D_802478E0) {
        gCurrentZone = 0;
    }
    if (shouldChangeRoom == TRUE) {
        if (D_80236974 == 0) {
            func_800C1458(gCurrentZone);
            return;
        }
        func_800C29D8(gCurrentZone);
    }
}


void func_800C0CDC_Hook(playerActor* arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32* stageFrameCount = (s32*)0x801749A0;
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
                if (!(isMenuActive)) {
                    savestateCurrentSlot ^= 1; //flip from 0 to 1 or vice versa (2 saveslots)
                }
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

extern s32 D_801749AC;
void AddCarrot(s32);
void func_80078294(f32, f32, s32, f32, s32);
s32 isntNegative(s32);
extern s32 currentStageCrowns;
extern s32 D_80247904;
extern s16 D_800FFEBC;
extern s16 D_80200B38;
extern s32 D_80236978;
extern char D_8010DB1C[];
//static SaveFile gSaveFile;
extern SaveFile gSaveFile; //should be static in matching code, but we dont want to declare this again
s32 SaveData_ReadFile(SaveFile*);
void func_8002CE54(void);
void func_800546F0(void);
void func_8008800C(s32);
void func_8008BE14(void);
void func_8008FD68(void);
void func_8008FE00(void);
void func_8008FEA8(s32, u8);
void func_800B4574(u8*, s16*);
void func_800C1458(s32);
void func_800C1510(u8, u8);
void func_800C2820(u8, playerActor*, SaveFile*);

char* ParseIntToBase10(s32, s32*);                         /* extern */
extern char D_8010D8E8[];
extern char D_8010D8F8[];
extern char D_8010D8FC[];
s32 func_80080430(f32, f32, f32, f32, f32, f32, char*, s32);

void func_80089BA0_Hook(void) {
    s32 unk;
    s32 sp78;
    s32 seconds;
    s32 milliseconds; // Added milliseconds variable
    char timeString[12]; // Assuming a maximum of 99:59.999
    char convertedBuffer[24];    
    s32 sp64;
    f32 sp5C;
    s32 minutes;
    s32 xPos;
    char* var_v1_2;

    switch (toggles[TOGGLE_HIDE_IGT]) {
        case 0:
            break;
        case 1: //original IGT display
            if ((gameModeCurrent == 0) && (gCurrentStage != 8)) {
                minutes = gCurrentStageTime / 1800; //(30 frames * 60 seconds)
                seconds = (gCurrentStageTime % 1800) / 30;
                func_800610A8();
                SetTextGradient(0xFF, 0xFF, 0U, 0xFF, 0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0, 0, 0xFF);
                //print ' character
                PrintText(220.0f, 208.0f, 0.0f, 0.5f, 0.0f, 0.0f, D_8010D8E8, 1);
                xPos = 220;
                if (minutes > 99) {
                    minutes = 99;
                    seconds = 59;
                }
                if (minutes < 10) {
                    PrintText(0xDC, 0xD0, 0.0f, 0.5f, 0.0f, 0.0f, D_8010D8F8, 1);
                    xPos = 228;
                }
                //var_v1_2 = ParseIntToBase10(minutes, &unk);
                PrintText((f32) xPos, 0xD0, 0.0f, 0.5f, 0.0f, 0.0f, ParseIntToBase10(minutes, &unk), 1);
                xPos = 244;
                if (seconds < 10) {
                    PrintText((f32) 0xF4, 0xD0, 0.0f, 0.5f, 0.0f, 0.0f, D_8010D8FC, 1);
                    xPos = 252;
                }
                PrintText((f32) xPos, 0xD0, 0.0f, 0.5f, 0.0f, 0.0f, ParseIntToBase10(seconds, &unk), 1);
            }
            break;
        case 2: //modified IGT display
            if ((gameModeCurrent == 0) && (gCurrentStage != 8)) {
                s32 totalFrames = gCurrentStageTime;
                minutes = totalFrames / 1800; // (30 frames * 60 seconds)
                seconds = (totalFrames % 1800) / 30;
                milliseconds = (totalFrames % 30) * 33; // Assuming 30 frames per second
                func_800610A8();
                SetTextGradient(0xFF, 0xFF, 0U, 0xFF, 0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0, 0, 0xFF);

                // Use sprintf to format the time components into a single string
                _sprintf(timeString, "%02d:%02d:%03d", minutes, seconds, milliseconds);
                convertAsciiToText(&convertedBuffer, (char*)&timeString);

                // Print the formatted time string
                PrintText(220.0f, 208.0f, 0.0f, 0.5f, 0.0f, 0.0f, convertedBuffer, 1);
            }

    }
}

// void func_80089BA0_Hook(void) {
//     s32 unk;
//     s32 sp78;
//     s32 seconds;
//     s32 milliseconds; // Added milliseconds variable
    
//     s32 sp64;
//     f32 sp5C;
//     s32 minutes;
//     s32 xPos;
//     char* var_v1_2;

//     if ((gameModeCurrent == 0) && (gCurrentStage != 8)) {
//         s32 totalFrames = gCurrentStageTime;
//         minutes = totalFrames / 1800; // (30 frames * 60 seconds)
//         seconds = (totalFrames % 1800) / 30;
//         milliseconds = (totalFrames % 30) * 33; // Assuming 30 frames per second
//         func_800610A8();
//         SetTextGradient(0xFF, 0xFF, 0U, 0xFF, 0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0, 0, 0xFF);
//         // Print ' character
//         PrintText(220.0f, 208.0f, 0.0f, 0.5f, 0.0f, 0.0f, D_8010D8E8, 1);
//         xPos = 220;
//         if (minutes > 99) {
//             minutes = 99;
//             seconds = 59;
//             milliseconds = 999; // Cap milliseconds at 999 if minutes exceed 99
//         }
//         if (minutes < 10) {
//             PrintText(0xDC, 0xD0, 0.0f, 0.5f, 0.0f, 0.0f, D_8010D8F8, 1);
//             xPos = 228;
//         }
//         PrintText((f32) xPos, 0xD0, 0.0f, 0.5f, 0.0f, 0.0f, ParseIntToBase10(minutes, &unk), 1);
//         xPos = 244;
//         if (seconds < 10) {
//             PrintText((f32) 0xF4, 0xD0, 0.0f, 0.5f, 0.0f, 0.0f, D_8010D8FC, 1);
//             xPos = 252;
//         }
//         PrintText((f32) xPos, 0xD0, 0.0f, 0.5f, 0.0f, 0.0f, ParseIntToBase10(seconds, &unk), 1);

//         xPos = 268;
//         // Print milliseconds with leading zeros
//         PrintText((f32) xPos, 0xD0, 0.0f, 0.5f, 0.0f, 0.0f, ParseIntToBase10(milliseconds / 100, &unk), 1);
//         xPos += 8;
//         PrintText((f32) xPos, 0xD0, 0.0f, 0.5f, 0.0f, 0.0f, ParseIntToBase10((milliseconds / 10) % 10, &unk), 1);
//         xPos += 8;
//         PrintText((f32) xPos, 0xD0, 0.0f, 0.5f, 0.0f, 0.0f, ParseIntToBase10(milliseconds % 10, &unk), 1);
//     }
// }

void Porocess_Mode0_Hook(void) {
    u32 temp_s0;
    s32 sp28;
    s32 sp24;
    s32 i;

    switch (gGameModeState) {
    case 0:
        D_800FFDF0 = 3;
        DMAStruct_Print();
        D_80174878 += 1;
        
        if (D_800F06EC >= 0) {
            D_80174878 = D_800F06EC;
        }
        
        D_80174878 = loadStageByIndex(D_80174878);
        //new code to set rng manip stuff
        //if entering bomb land, and set seed option is ON
        if (gCurrentStage == 2 && toggles[TOGGLE_SET_SEED]) { 
            u32* seed = (u32*)0x80109DC0;
            u32* calls = (u32*)0x80109DC4;
            //the game will advance the seed 4 times after we set it
            //so we set it to what we want - 4
            *seed = 0x14B0B9CB;
            *calls = 702;
        }
        if (gCurrentStage == 7) {
            D_80168DA0 = (u32) gControllerNo;
            D_801749AC = 2;
        } else {
            D_801749AC = 0;
            D_80168DA0 = 1;
        }

        //required 1 liner to match (changed in this since we dont need matching code)
        for (i = 0; i < D_80168DA0; i++) {
            gPlayerActors[i].active = 1;
        }

        for (; i < 4; i++) {
            gPlayerActors[i].active = 0;
        }

        for (i = 0; i < 4; i++) {
            _bzero(&gTongues[i], sizeof(Tongue));
        }
    
        func_8002E0CC();
        InitField();
        func_80056EB4();
        aa1_InitHead();
        func_8005C9B8();
        func_80084788();
        
        D_80174980 = 0;
        if (gCurrentStage == 7) {
            func_800546F0();
        } else {
            func_8008FE00();
        }
        if (gCurrentStage == 2) {
            loadPlayerEyes(4);
            setPlayerContextEyes(4, 0, 0);
            freePlayerEyes(4);
        }
        TaskInit();
        if ((gCurrentStage == 0xF) || (gCurrentStage == 8)) {
            func_800C1458(0);
        }
        func_8008BE14();
        func_8008800C(8);
        gGameModeState += 1;
        func_8008F114();
        gCurrentStageTime = 0;
        return;
    case 1:
        func_8002CE54();
        return;
    case 2:
        gGameModeState = 1;
        return;
    case 3:
        D_801749AC = 0;
        temp_s0 = gPlayerActors->hp;
        sp28 = currentStageCrowns;
        sp24 = D_80247904;
        DMAStruct_Print();
        D_80174878 += 1;
        if (D_800F06EC >= 0) {
            D_80174878 = D_800F06EC;
        }
        D_80174878 = loadStageByIndex(D_80174878);
        func_8002E0CC();
        InitField();
        gPlayerActors->hp = temp_s0;
        func_80056EB4();
        aa1_InitHead();
        func_8005C9B8();
        func_80084788();
        TaskInit();
        if (D_800FFEBC != 0) {
            func_800C1458(1);
        } else {
            func_800C1458(0);
        }
        gGameModeState = 1;
        func_8008F114();
        currentStageCrowns = sp28;
        D_80247904 = sp24;
        func_8008FE00();
        if (gCurrentStage == 2) {
            loadPlayerEyes(4);
            setPlayerContextEyes(4, 0, 0);
            freePlayerEyes(4);
            return;
        }
    default:
        return;
    case 4:
        for (i = 0; i < 4; i++) {
            _bzero(&gTongues[i], sizeof(Tongue));
        }
        
        gPlayerActors[0].active = 1;
        for (i = 1; i < 4; i++) {
            gPlayerActors[i].active = 0;
        }
        
        gNoHit = 0;
        gOneRun = 0;
        D_80200B38 = 0;
        D_801749AC = 0;
        SaveData_ReadFile(&gGameState);
        D_80174878 = gCurrentStage - 1;
        func_8008FD68();
        SaveData_ReadFile(&gGameState);
        if (D_80236974 == 1) {
            if (gCurrentStage == 0) {
                D_80236978 = 1;
            }
            func_800C2820(gGameState.gCurrentZone, &gPlayerActors[0], &gGameState);
        } else {
            D_80236978 = 0;
            func_800C1510(gGameState.gCurrentZone, gGameState.unk33);
            func_800B4574(&gGameState.unk2, &gGameState.unk_22);
            func_800C0760(gGameState.gCurrentZone);
        }
        currentStageCrowns = (s32) gGameState.stageCrowns;
        DummiedPrintf(D_8010DB1C);
        func_8008FEA8(gCurrentStage, gGameState.gCurrentZone);
        gGameModeState = 1;
        func_8008F114();
        func_8008FE00();
        if (gCurrentStage == 2) {
            loadPlayerEyes(4);
            setPlayerContextEyes(4, 0, 0);
            freePlayerEyes(4);
        }
        func_8008800C(8);
        return;
    case 5:
        setProcessType(1);
        func_8008F114();
        return;
    case 6:
        setProcessType(6);
        return;
    case 7:
        gNoHit = 0;
        gOneRun = 0;
        D_80200B38 = 0;
        D_80168DA0 = 1;
        D_801749AC = 0;
        SaveData_ReadFile(&gSaveFile);
        D_80174878 = gCurrentStage - 1;
        for (i = 0; i < 4; i++) {
            _bzero(&gTongues[i], sizeof(Tongue));
        }
        func_8008FD68();
        SaveData_ReadFile(&gSaveFile);
        if (D_80236974 == 1) {
            if (gCurrentStage == 0) {
                D_80236978 = 1;
            }
            func_800C2820(gSaveFile.gCurrentZone, &gPlayerActors[0], &gSaveFile);
        } else {
            D_80236978 = 0;
            func_800C1510(gSaveFile.gCurrentZone, gSaveFile.unk33);
            func_800B4574(&gSaveFile.flags, &gSaveFile.unk_22);
            func_800C0760(gSaveFile.gCurrentZone);
        }
        func_8008FEA8(gCurrentStage, gSaveFile.gCurrentZone);
        currentStageCrowns = (s32) gSaveFile.stageCrowns;
        gGameModeState = 1;
        func_8008F114();
        func_8008FE00();
        if (gCurrentStage == 2) {
            loadPlayerEyes(4);
            setPlayerContextEyes(4, 0, 0);
            freePlayerEyes(4);
        }
        break;
    }
}