#include "common.h"
#include "../../include/menu.h"
#include "../../include/mod_main.h"

s32 currPageNo = 0;
s32 currOptionNo = 0;

#define PAGE_JL 0
#define PAGE_RECORDING 1

char string_ON[] = {
    "ON"
};

char string_OFF[] = {
    "OFF"
};


char* ONAndOFF[] = {
    string_OFF,
    string_ON,
};

char* InGameTimerText[] = {
    string_OFF,
    // string_IGT,
    // string_IGT_MS,
    // string_IGT_FRAMES,
    // string_RTA_MS
};

char variousStatsText[] = {
    "VARIOUS"
};

char freeCamText[]= {
    "CAM ANGLE SNAP"
};

char rngSeedText[] = {
    "RNG CALLS/SEED"
};

char rngSeed2Text[] = {
    "RNG2 CALLS/SEED"
};

char quintellaSpinCancelText[] = {
    "SPIN CANCEL TIMER"
};

char frameCountText[] = {
    "IGT TOTAL FRAMES"
};

char StickPositionText[] =  {
    "STICK POSITION"
};

char* CustomTextMain[] = {
    string_OFF,
    variousStatsText,
    freeCamText,
    rngSeedText,
    rngSeed2Text,
    quintellaSpinCancelText,
    frameCountText,
    StickPositionText,
};

char** page0Strings[] = {
    ONAndOFF, //Savestate Text Active text
    // ONAndOFF, //Savestate no compression text
    // InGameTimerText, //In Game Timer Active text
    // CustomTextMain, //CustomDebugText
    // ONAndOFF, // TOGGLE_INFINITE_HEALTH
    // ONAndOFF, // TOGGLE_STORAGE
};

char recordingString[] = "RECORDING";
char* RecordingStrings[] = {
    string_OFF,
    recordingString,
};

char** page1Strings[] = {
    NULL, // EXPORT RECORDING
    NULL, //Savestate Text Active text
    NULL, //
    // ONAndOFF, //Savestate no compression text
    // InGameTimerText, //In Game Timer Active text
    // CustomTextMain, //CustomDebugText
    // ONAndOFF, // TOGGLE_INFINITE_HEALTH
    // ONAndOFF, // TOGGLE_STORAGE
};

menuPage page0 = {
    1, //optionCount
    PAGE_JL, //pageIndex
    { //options
        "Savestate Text",
    },
    { //menuProc
        &toggleHideSavestateText,
    },
    { //flags
        TOGGLE_HIDE_SAVESTATE_TEXT,
    },
    {
        page0Strings,
    }
};

s32 ExportRecording(void);
s32 ImportRecording(void);

s32 EndRecording(void) {

}
    // TOGGLE_RECORDING,
    // TOGGLE_PLAYBACK,
    // EXPORT_RECORDING
menuPage page1 = {
    3, // optionCount
    PAGE_RECORDING, // pageIndex
    { // options
        "EXPORT RECORDING",
        "IMPORT RECORDING",
        "END RECORDING"
        // "Export Recording",
        // "Import Recording",
        // "Play Recording"
    },
    { // menuProc
        &ExportRecording,
        &ImportRecording,
        &EndRecording,
    },
    { // flags
        -1,
        -1,
        -1
    },
    {
        page1Strings, //update when implemented
    },
};