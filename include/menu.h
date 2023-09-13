#ifndef _MENU_H
#define _MENU_H

#include "common.h"

typedef struct InputRecording {
    u32 totalFrameCount;
    contMain recordingBuffer[1200];
} InputRecording;

// typedef struct unkIsChange {
//     /* 0x00 */ char unk_00[8];
//     /* 0x08 */ s32 unk_08;
//     /* 0x0C */ s32 unk_0C;
//     /* 0x10 */ s32 unk_10;
//     /* 0x14 */ s32 unk_14;
//     /* 0x18 */ s32 unk18;
//     /* 0x1C */ char unk_1C[0x24];
//     /* 0x40 */ f32 unk40;
//     /* 0x44 */ f32 unk44;
//     /* 0x48 */ f32 unk48;
//     /* 0x4C */ f32 unk4C;
//     /* 0x50 */ f32 unk50;
//     /* 0x54 */ f32 unk54;
//     /* 0x58 */ char unk_58[0x7C];
//     /* 0xD4 */ s32 unk_D4;
//     /* 0xD8 */ s32 unk_D8;
// } unkIsChange;

extern void _sprintf(void* destination, void* fmt, ...);
extern void convertAsciiToText(void* buffer, char* source);
extern s32 recordingErrorMessageStick;
void pageMainDisplay(s32 currPageNo, s32 currOptionNo);
s32 menuDisplay(void);
s32 menuProcFunc(void);
s32 teleportToStageBoss(void);
void colorTextWrapper(s32* color);
void updateCustomInputTracking(void);
extern s32 recordingMode;
extern volatile s32 isSaveOrLoadActive;
extern s32 savestateRecordingSize;
extern s32 currentlyPressedButtons;
extern s32 previouslyPressedButtons;
extern s8 toggles[];
extern InputRecording inputRecordingBuffer;
extern u32 recordingInputIndex;
extern s32 textCyanColor[];
extern s32 textGreenColor[];
extern s32 textGreenMatColor[];
extern s32 textWhiteColor[];
extern s32 textRedColor[];
extern OSContPad D_80175650[MAXCONTROLLERS];
enum Pages {
    PAGE_MAIN = 0,
    PAGE_JL = 1,
    PAGE_RECORDING = 2
};

enum Toggles {
    NO_TOGGLE = 0,
    //page 0
    TOGGLE_HIDE_SAVESTATE_TEXT,
    TOGGLE_HIDE_IGT,
    TOGGLE_CUSTOM_DEBUG_TEXT,
    TOGGLE_CAVE_SKIP_PRACTICE,
    TOGGLE_ENEMY_SPAWNS_OFF,

    //page 1
    TOGGLE_LOAD_BOSS,
    TOGGLE_INFINITE_HEALTH,
    TOGGLE_ADV_RNG,
    TOGGLE_REV_RNG,
    //TOGGLE_SPEED,
    TOGGLE_SET_SEED,

    //page 2
    TOGGLE_RECORDING,
    TOGGLE_PLAYBACK
};


#endif
