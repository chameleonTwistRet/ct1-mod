#include "common.h"
#include "../../include/menu.h"
#include "xstdio.h"
#include "../../include/mod_main.h"
#include "../../lib/cart.h"
#include "../../lib/ff.h"
#include "../../lib/ffconf.h"
#include "mod.h"

menuPage page2 = {
    4, // optionCount
    PAGE_RECORDING, // pageIndex
    { // options
        "Recording",
        "Export Recording",
        "Import Recording",
        "Play Recording"
    },
    { // menuProc
        &StartRecording,
        &ExportRecording,
        &ImportRecording,
        &PlayRecording
    },
    { // flags
        TOGGLE_RECORDING,
        NO_TOGGLE,
        NO_TOGGLE,
        TOGGLE_PLAYBACK
    },
    {
        page2Strings, //update when implemented
    },
};

menuPage page0 = {
    6, //optionCount
    PAGE_JL, //pageIndex
    { //options
        "Savestate Text",
        "No compression savestates",
        "In Game Timer",
        "Misc",
        "Infinite Health",
        "Toggle Storage"
    },
    { //menuProc
        &toggleHideSavestateText,
        &NoCompressionToggle,
        &toggleHideIGT,
        &toggleCustomDebugText,
        &toggleInfiniteHealth,
        &toggleStorage
    },
    { //flags
        TOGGLE_HIDE_SAVESTATE_TEXT,
        TOGGLE_NO_COMPRESSION_SAVESTATES,
        TOGGLE_DISPLAY_IGT,
        TOGGLE_CUSTOM_DEBUG_TEXT,
        TOGGLE_INFINITE_HEALTH,
        TOGGLE_STORAGE
    },
    {
        page0Strings,
    }
};

menuPage page4 = {
    5, //optionCount
    PAGE_PRACTICE, //pageIndex
    { //options
        "Cave Skip Practice",
        "Yolo Pillar Practice",
        "Back Vault Practice",
        "Disable Actors",
        "Load Boss\n"
    },
    { //menuProc
        &toggleCaveSkipPractice,
        &toggleYoloPillarPractice,
        &toggleBackVaultPractice,
        &toggleObjectSpawnsOff,
        &teleportToStageBoss,
    },
    { //flags
        TOGGLE_CAVE_SKIP_PRACTICE,
        TOGGLE_YOLO_PILLAR_PRACTICE,
        TOGGLE_BACKVAULT_PRACTICE,
        TOGGLE_ENEMY_SPAWNS_OFF,
        -1
    },
    {
        page4Strings,
    }
};

menuPage page1 = {
    7, //optionCount
    PAGE_MAIN, //pageIndex
    { //options
        "Adv RNG\n",
        "Rev RNG\n",
        "Set JL RNG Seed",
        "Set BL RNG Seed",
        "Set BL IL RNG Seed",
        "Set KL RNG Seed",
        "Set GC RNG Seed",
        //"Speed\n"
    },
    { //menuProc
        &advanceGuRNG,
        &revGuRNG,
        &setJLSeed,
        &setBLSeed,
        &setBLILSeed,
        &setKLSeed,
        &setGCSeed
        //&toggleSpeed
    },
    { //flags
        -1,
        -1,
        TOGGLE_SET_SEED_JL,
        TOGGLE_SET_SEED_BL,
        TOGGLE_SET_SEED_IL_BL,
        TOGGLE_SET_SEED_KL,
        TOGGLE_SET_SEED_GC,
        //TOGGLE_REV_RNG
    },
    {
        page1Strings,
    },
};

menuPage page3 = {
    5, // optionCount
    PAGE_MISC, // pageIndex
    { // options
        "STAGE START RESET TIMER",
        "DISPLAY RTA TIME IN ROOM",
        "DISPLAY TAS COMPARISON",
        "DISPLAY TIME OF RETRACTION",
        "FRAME ADVANCE ON P2",
    },
    { // menuProc
        &ResetTimerToggle,
        &DisplayIndividualRoomTimeToggle,
        &DisplayTASComparison,
        &FrameAdvanceToggle,
        &DisplayTongueRetractionTime
    },
    { // flags
        TOGGLE_RTA_TIMER_RESET,
        TOGGLE_DISPLAY_INDIVIDUAL_ROOM_TIME,
        TOGGLE_TAS_COMPARISON,
        TOGGLE_RETRACTION_TIME,
        TOGGLE_FRAME_ADVANCE
    },
    {
        page3Strings,
    },
};