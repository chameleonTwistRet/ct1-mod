#include "common.h"
#include "../../include/menu.h"
#include "../../include/mod_main.h"

char string_ON[] = {
    "ON"
};

char string_OFF[] = {
    "OFF"
};

char string_IGT[] = {
    "NORMAL IGT"
};

char string_IGT_MS[] = {
    "IGT W/ MS"
};

char string_IGT_FRAMES[] = {
    "IGT W/ FRAMES"
};

char string_RTA_MS[] = {
    "RTA TIMING"
};


char* ONAndOFF[] = {
    string_OFF,
    string_ON,
};

char* InGameTimerText[] = {
    string_OFF,
    string_IGT,
    string_IGT_MS,
    string_IGT_FRAMES,
    string_RTA_MS
};

char manip1Text[] = {
    "MANIP 1"
};

char manip2Text[] = {
    "MANIP 2"
};

char* manipTexts[] = {
    string_OFF,
    manip1Text,
    manip2Text
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

char* CustomTextMain[] = {
    string_OFF,
    variousStatsText,
    freeCamText,
    rngSeedText,
    rngSeed2Text,
    quintellaSpinCancelText,
    frameCountText
};

char** page0Strings[] = {
    ONAndOFF, //Savestate Text Active text
    ONAndOFF, //Savestate no compression text
    InGameTimerText, //In Game Timer Active text
    CustomTextMain, //CustomDebugText
    ONAndOFF, // TOGGLE_INFINITE_HEALTH
};

char** page4Strings[] = {
    ONAndOFF, //Cave skip practice
    ONAndOFF, //yolo pillar practice
    ONAndOFF, //backvault practice
    ONAndOFF, //enemy spawns off
    NULL, // TOGGLE_LOAD_BOSS    
};

char** page1Strings[] = {
    NULL, //Adv Rng
    NULL, //Rev Rng
    manipTexts, //Set JL RNG Seed
    manipTexts, //Set BL RNG Seed
    ONAndOFF, //Set BL IL RNG Seed
    ONAndOFF, //Set KL RNG Seed
    ONAndOFF, //Set GC RNG Seed
};

char** page3Strings[] = {
    ONAndOFF,
    ONAndOFF,
    ONAndOFF,
    ONAndOFF,
};