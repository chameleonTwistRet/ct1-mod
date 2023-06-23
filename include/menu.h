#ifndef _MENU_H
#define _MENU_H

#include "common.h"

typedef struct InputRecording {
    u32 totalFrameCount;
    contMain recordingBuffer[1200];
} InputRecording;

typedef struct unkIsChange {
    /* 0x00 */ char unk_00[8];
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s32 unk_0C;
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ s32 unk_14;
    /* 0x18 */ char unk_18[0xBC];
    /* 0xD4 */ s32 unk_D4;
    /* 0xD8 */ s32 unk_D8;
} unkIsChange;

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
extern OSContPad D_80175650[];
extern s32 currentlyPressedButtons;
extern s32 previouslyPressedButtons;
extern u8 toggles[];
extern InputRecording inputRecordingBuffer;
extern u32 recordingInputIndex;
extern s32 textCyanColor[];
extern s32 textGreenColor[];
extern s32 textGreenMatColor[];
extern s32 textWhiteColor[];
extern s32 textRedColor[];

enum Pages {
    PAGE_MAIN = 0,
    PAGE_JL = 1,
    PAGE_RECORDING = 2
};

enum Toggles {
    NO_TOGGLE,
    TOGGLE_HIDE_IGT,
    TOGGLE_HIDE_SAVESTATE_TEXT,
    TOGGLE_INFINITE_HEALTH,
    TOGGLE_CUSTOM_DEBUG_TEXT,
    TOGGLE_CAVE_SKIP_PRACTICE,
    TOGGLE_ENEMY_SPAWNS_OFF,
    TOGGLE_RECORDING,
    TOGGLE_PLAYBACK
};


#endif
