#ifndef _MENU_H
#define _MENU_H

#include "common.h"
#include "../lib/ff.h"

typedef s32 (*menuProc) (void);

// shift x value per print per length of string (8px per letter) then print
#define X_COORD_PER_LETTER 4.5
#define FUNCS_PER_PAGE 7
#define ARRAY_COUNT(arr) (s32)(sizeof(arr) / sizeof(arr[0]))
#define ARRAY_COUNT_INDEX(arr) ARRAY_COUNT(arr) - 1

typedef struct InputRecording {
    u32 totalFrameCount;
    u32 framePlaybackIndex;
    contMain recordingBuffer[21600];
} InputRecording;

typedef struct menuPage {
    /* 0x00 */ s32 optionCount;
    /* 0x04 */ s32 pageIndex;
    /* 0x08 */ char* options[FUNCS_PER_PAGE];
    /* 0x28 */ s32 (*menuProc[FUNCS_PER_PAGE]) (void);
    /* 0x48 */ s8 flags[FUNCS_PER_PAGE];
    /* 0x50 */ char*** selectionText;
} menuPage;

extern menuPage page0;
extern menuPage page1;
extern menuPage page2;
extern menuPage page3;
extern menuPage page4;
extern s32 isMenuActive;
extern volatile s32 savestateCurrentSlot;
extern u64 totalElapsedCounts;
extern u64 prevDoorEntryTime;

extern void _sprintf(void* destination, void* fmt, ...);
extern void convertAsciiToText(void* buffer, char* source);
extern s32 recordingErrorMessageStick;
void pageMainDisplay(s32 currPageNo, s32 currOptionNo);
s32 menuDisplay(void);
s32 menuProcFunc(void);
s32 teleportToStageBoss(void);
void colorTextWrapper(s32* color);
void updateCustomInputTracking(void);
void func_8004E784_Hook(contMain* arg0, s32 arg1, s32* arg2, contMain* arg3);
extern s32 recordingMode;
extern volatile s32 isSaveOrLoadActive;
extern s32 savestateRecordingSize;
extern s32 currentlyPressedButtons;
extern s32 previouslyPressedButtons;
extern s32 currPageNo;
extern s32 currOptionNo;
extern s32 pageListTotal;
extern menuPage* pageList[];
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
s32 DisplayTongueRetractionTime(void);
s32 FrameAdvanceToggle(void);
s32 NoCompressionToggle(void);
s32 advanceGuRNG(void);
s32 revGuRNG(void);
s32 setJLSeed(void);
s32 setBLSeed(void);
s32 setBLILSeed(void);
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
    TOGGLE_STORAGE,

    //page 4
    TOGGLE_CAVE_SKIP_PRACTICE,
    TOGGLE_YOLO_PILLAR_PRACTICE,
    TOGGLE_BACKVAULT_PRACTICE,
    TOGGLE_ENEMY_SPAWNS_OFF,
    TOGGLE_LOAD_BOSS,

    //page 1
    TOGGLE_ADV_RNG,
    TOGGLE_REV_RNG,
    TOGGLE_SET_SEED_JL,
    TOGGLE_SET_SEED_BL,
    TOGGLE_SET_SEED_IL_BL,
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
    TOGGLE_RETRACTION_TIME,
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
extern char** page2Strings[];
extern FATFS FatFs;
extern char *path; //"ct1State.bin"; //example file for SD card writing
extern FIL sdsavefile;

#endif
