#ifndef _MENU_H
#define _MENU_H

#include "common.h"
#include "../lib/ff.h"

typedef struct InputRecording {
    u32 totalFrameCount;
    contMain recordingBuffer[1200];
} InputRecording;

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

s32 DisplayIndividualRoomTimeToggle(void);
s32 DisplayTASComparison(void);
s32 FrameAdvanceToggle(void);
s32 NoCompressionToggle(void);
s32 advanceGuRNG(void);
s32 revGuRNG(void);
s32 setBLSeed(void);
s32 setKLSeed(void);
s32 setGCSeed(void);
s32 toggleYoloPillarPractice(void);
s32 toggleBackVaultPractice(void);
s32 toggleHideSavestateText(void);
s32 toggleHideIGT(void);
s32 toggleInfiniteHealth(void);
// s32 toggleSpeed(void);
s32 toggleCustomDebugText(void);
s32 toggleCaveSkipPractice(void);
s32 toggleObjectSpawnsOff(void);
void enterBossRoom2(s32 unk);
s32 teleportToStageBoss(void);
void savestateMainStartRecording(void);
void loadstateRecordingMain(void);
s32 ResetTimerToggle(void);
s32 StartRecording(void);
s32 ExportRecording(void);
s32 ImportRecording(void);
s32 PlayRecording(void);

enum Pages {
    PAGE_MAIN = 0,
    PAGE_JL = 1,
    PAGE_RECORDING = 2,
    PAGE_MISC = 3,
    PAGE_PRACTICE = 4
};

enum Toggles {
    NO_TOGGLE = 0,
    //page 0
    TOGGLE_HIDE_SAVESTATE_TEXT,
    TOGGLE_NO_COMPRESSION_SAVESTATES,
    TOGGLE_DISPLAY_IGT,
    TOGGLE_CUSTOM_DEBUG_TEXT,
    TOGGLE_INFINITE_HEALTH,

    //page 4
    TOGGLE_CAVE_SKIP_PRACTICE,
    TOGGLE_YOLO_PILLAR_PRACTICE,
    TOGGLE_BACKVAULT_PRACTICE,
    TOGGLE_ENEMY_SPAWNS_OFF,
    TOGGLE_LOAD_BOSS,

    //page 1
    TOGGLE_ADV_RNG,
    TOGGLE_REV_RNG,
    TOGGLE_SET_SEED_BL,
    TOGGLE_SET_SEED_KL,
    TOGGLE_SET_SEED_GC,
    //TOGGLE_SPEED,

    //page 2
    TOGGLE_RECORDING,
    TOGGLE_PLAYBACK,

    //page 3
    TOGGLE_RTA_TIMER_RESET,
    TOGGLE_DISPLAY_INDIVIDUAL_ROOM_TIME,
    TOGGLE_TAS_COMPARISON,
    TOGGLE_FRAME_ADVANCE
};

extern char string_ON[];
extern char string_OFF[];
extern char string_IGT[];
extern char string_IGT_MS[];
extern char string_IGT_FRAMES[];
extern char string_RTA_MS[];
extern char* ONAndOFF[];
extern char* InGameTimerText[];
extern char variousStatsText[];
extern char freeCamText[];
extern char rngSeedText[];
extern char quintellaSpinCancelText[];
extern char frameCountText[];
extern char* CustomTextMain[];
extern char** page0Strings[];
extern char** page4Strings[];
extern char** page1Strings[];
extern char** page3Strings[];
extern FATFS FatFs;
extern char *path; //"ct1State.bin"; //example file for SD card writing
extern FIL sdsavefile;

#endif
