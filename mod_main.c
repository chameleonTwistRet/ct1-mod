#include "include/mod_main.h"
#include "include/mod.h"
#include "lib/cart.h"
#include "lib/ff.h"
#include "lib/ffconf.h"

//800EE140 - 800EE1C0 global variables to be saved in savestate
SaveStateSavedVariables* SaveStateVars = (SaveStateSavedVariables*)0x800EE140;

//hooked function signatures
void func_8004E784(contMain* arg0, s32 arg1, s32* arg2, contMain* arg3);
void func_8004E784_Hook(contMain* arg0, s32 arg1, s32* arg2, contMain* arg3);
ldiv_t ldiv_Hook(long num, long denom);

s32 display_string_frames = 0;
char pracTwistVersionString[] = "Practwist v0.1";
char printTextBuffer[0x100] = {'\0'};    // Text buffer set to empty string
char printTextBuffer2[0x100] = {'\0'};   // Text buffer set to empty string
char stringBuffer[0x100] = {'\0'};       // String buffer set to empty for string manipulation

// wrapper for printing text
void textPrint(f32 xPos, f32 yPos, f32 scale, void *text, s32 num) {
    PrintText(xPos, yPos, 0, scale, 0, 0, text, num);
}

//in assets/ you'll find an example of replacing an image

s32 loadEnemiesBool = 0; //used by `asm_functions.s`
volatile s32 saveOrLoadStateMode = 0;
volatile s32 savestateCurrentSlot = 0;
s32 savestate1Size = 0;
s32 savestate2Size = 0;
volatile s32 isSaveOrLoadActive = 0;
s32 stateCooldown = 0;


void InitializeSDCard(void);
extern FATFS FatFs;
extern FIL sdsavefile;
extern s32 status;
extern FRESULT fileres;

//mod_boot_func: runs a single time on boot before main game loop starts
void mod_boot_func(void) {
    crash_screen_init();

    #if USE_SD_CARD == TRUE
        _bzero(&sdsavefile, sizeof(FIL));
        // osCreateThread(&gCustomThread2.thread, 157, (void*)InitializeSDCard, NULL,
        //         gCustomThread2.stack + sizeof(gCustomThread2.stack), 157);
        // osStartThread(&gCustomThread2.thread);
        InitializeSDCard();
    #endif

    // example of setting up text to print
    _bzero(&printTextBuffer2, sizeof(printTextBuffer2)); // clear text
    _bzero(&stringBuffer, sizeof(stringBuffer)); // clear string buffer
    strcat((char*)&stringBuffer, "test");
    _sprintfcat(&stringBuffer, " %d", 1);
    convertAsciiToText(&printTextBuffer2, stringBuffer);

    hookCode((s32*)&func_8004E784, &func_8004E784_Hook); //hook controller reading of overworld gamemode
    hookCode((s32*)&ldiv, &ldiv_Hook); //move this for our custom ram space that gets saved/loaded with savestates
    //hookCode((s32*)0x8002D660, &loadEnemyObjectsHook); //example of hooking code
}

void PrintSaveslot(void) {
    s32 i = 0;

    printTextBuffer[i++] = 0xA3;
    printTextBuffer[i++] = 0xB0 + savestateCurrentSlot;

    printTextBuffer[i++] = 0;
    textPrint(13.0f, 208.0f, 0.65f, &printTextBuffer, 3);
}

void PrintPlaybackInfo(void) {
    char textBuf1[32];
    char textBuf2[64];

    if (toggles[TOGGLE_PLAYBACK] == 1) {
        colorTextWrapper(textGreenMatColor);
        _sprintf(&textBuf1, "%d / %d", inputRecordingBuffer.framePlaybackIndex, inputRecordingBuffer.totalFrameCount);
        convertAsciiToText(&textBuf2, textBuf1);
        textPrint(45.0f, 220.0f, 0.5f, textBuf2, 3);
    } else if (toggles[TOGGLE_RECORDING] == 1) {
        colorTextWrapper(textRedColor);
        _sprintf(&textBuf1, "%d", inputRecordingBuffer.totalFrameCount);
        convertAsciiToText(&textBuf2, textBuf1);
        textPrint(45.0f, 220.0f, 0.5f, textBuf2, 3);
    }
}

extern s32 prevLoadedState;

//mod_main_per_frame: where to add code that runs every frame before main game loop
void mod_main_per_frame(void) {
    s32 index = 0;
    char printTextBuffer[8];

    if (stateCooldown > 0) {
        stateCooldown--;
    }

    if (isMenuActive == 1) {
        pageMainDisplay(currPageNo, currOptionNo);
    }

    PrintSaveslot();
    PrintPlaybackInfo();

    if (display_string_frames > 0) {
        char buffer[32];
        char buffer2[64];

        display_string_frames--;
        // if (val == 0) {
        //     _sprintf(&buffer, "WRITE CALLED");
        // } else {
        //     _sprintf(&buffer, "READ CALLED");
        // }
        
        convertAsciiToText(&buffer2, buffer);
        textPrint(45.0f, 220.0f, 0.5f, buffer2, 3);
    }


    //if a savestate is being saved/loaded, stall thread
    while (isSaveOrLoadActive != 0) {}
}

void PrintBootInfo(void) {
    char mountSD[] = "SD MOUNTED";
    char notMountSD[] = "SD NOT MOUNTED";
    char fileCreated[] = "FILE CREATED";
    char fileNotCreated[] = "FILE NOT CREATED";
    char stringBuf[150];
    char tempBuf[16];
    colorTextWrapper(textGreenMatColor);
    convertAsciiToText(stringBuf, (char*)&pracTwistVersionString);
    textPrint(15.0f, 220.0f, 0.5f, stringBuf, 3);

    if (fileres == FR_OK) { //file created successfully
        colorTextWrapper(textGreenMatColor);
        convertAsciiToText(stringBuf, fileCreated);
        textPrint(15.0f, 160.0f, 0.5f, stringBuf, 3);     
    } else {
        colorTextWrapper(textRedColor);
        convertAsciiToText(stringBuf, fileNotCreated);
        textPrint(15.0f, 160.0f, 0.5f, stringBuf, 3);   
    }
}

//Thread 3 osStartThread function
//this is a decomped version of `MainLoop.s`. It has a call to `mod_main_per_frame` before each gamemode step
//name is hardcoded in configure script. if name is changed, change it there too
void mod_main_func(void) {
    // func_8002D080(); //is already ran in MainLoop.s patch
    
    if (sGameModeStart != -1) {
        gameModeCurrent = sGameModeStart;
    }
    
    gGameModeState = 0;
    osRecvMesg(&D_801192E8, 0, 1);
    SaveData_LoadRecords(&gGameRecords.flags[0]);
    
    if (SaveData_RecordChecksum() != gGameRecords.flags[0]) {
        SaveData_ClearRecords();
    }

    gIsStero = gGameRecords.flags[1] & 1; //gIsStero = gGameRecords.flags0[1] & 1;
    osRecvMesg(&D_801192E8, 0, 1);
    mod_boot_func();
    //step current game mode
    loop:
    mod_main_per_frame();
    switch(gameModeCurrent) {
        case GAME_MODE_OVERWORLD:
            Porocess_Mode0();
            goto loop;
        case GAME_MODE_LEVEL_INTRO_MENU:
            Process_StageSelect();
            goto loop;
        case GAME_MODE_SAVE_MENU:
            FileWork();
            goto loop;
        case GAME_MODE_LOAD_GAME_MENU:
            func_8009C904();
            goto loop;
        case GAME_MODE_DEMO:
            func_800A9F84();
            goto loop;
        case GAME_MODE_DEMO_2:
            func_800AA3F0();
            goto loop;
        case GAME_MODE_CREDITS:
            func_800ADE70();
            goto loop;
        case GAME_MODE_OPENING_CUTSCENE:
            func_800AE4AC();
            goto loop;
        case GAME_MODE_TITLE_SCREEN:
            func_800A2BDC();
            goto loop;
        case GAME_MODE_BATTLE_MENU:
            Process_BattleMenu();
            goto loop;
        case GAME_MODE_OPTIONS_MENU:
            func_800A4320();
            goto loop;
        case GAME_MODE_GAME_OVER:
            Process_GameOver();
            goto loop;
        case GAME_MODE_SUPPLY_SYSTEM_LOGO:
            PrintBootInfo();
            Process_JSSLogo();
            goto loop;
        case GAME_MODE_PRE_CREDITS:
            Process_PreCredits();
            goto loop;
        case GAME_MODE_NEW_GAME_MENU:
            func_800A1D38();
            goto loop;
        case GAME_MODE_LEVEL_INTRO:
            func_800A6DD8();
            goto loop;
        case GAME_MODE_STAGE_SELECT:
            func_800A07E0();
            goto loop;
        case GAME_MODE_RANKING:
            Process_Ranking();
            goto loop;
        case 17:
            func_800557F8();
            goto loop;
        case GAME_MODE_BOOT:
            Process_Boot();
            goto loop;
        case GAME_MODE_SUNSOFT_LOGO:
            Process_SunsoftLogo();
            goto loop;
        case GAME_MODE_UNK_15:
            DummiedPrintf(D_8010DB20, gameModeCurrent);
            goto loop;
    }
}