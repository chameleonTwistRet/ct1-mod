#include "include/mod_main.h"
#include "include/sd_toggle.h"
#include "lib/cart.h"
#include "lib/ff.h"
#include "lib/ffconf.h"

char pracTwistVersionString[] = "Practwist v0.1";
char printTextBuffer[0x100] = {'\0'};    // Text buffer set to empty string
char printTextBuffer2[0x100] = {'\0'};   // Text buffer set to empty string
char stringBuffer[0x100] = {'\0'};       // String buffer set to empty for string manipulation

// wrapper for printing text
void textPrint(f32 xPos, f32 yPos, f32 scale, void *text, s32 num) {
    PrintText(xPos, yPos, 0, scale, 0, 0, text, num);
}

// Patches work the same way as 81 and 80 GameShark Codes
void s16patch(void* patchAddr, s16 patchInstruction) {
    *(s16*)patchAddr = patchInstruction;
}
void s8patch(void* patchAddr, s8 patchInstruction) {
    *(s8*)patchAddr = patchInstruction;
}

s16 s16Read(void* readAddr) {
    return *(s16*)readAddr;
}
s8 s8Read(void* readAddr) {
    return *(s8*)readAddr;
}

// Print Values from Address (these will be in a new file soon)
void s8print(void* addr) {
    s8 value = s8Read((void*)addr);
    _bzero(&stringBuffer, sizeof(stringBuffer));
    _bzero(&printTextBuffer2, sizeof(printTextBuffer2));
    _sprintf(&stringBuffer, "%X: %d", addr, value);
    convertAsciiToText(&printTextBuffer2, stringBuffer);
    textPrint(13.0f, 30.0f, 1.0f, &printTextBuffer2, 1);
}
void s16print(void* addr) {
    s16 value = s16Read((void*)addr);
    _bzero(&stringBuffer, sizeof(stringBuffer));
    _bzero(&printTextBuffer2, sizeof(printTextBuffer2));
    _sprintf(&stringBuffer, "%X: %d", addr, value);
    convertAsciiToText(&printTextBuffer2, stringBuffer);
    textPrint(13.0f, 30.0f, 1.0f, &printTextBuffer2, 1);
}

//in assets/ you'll find an example of replacing an image

s32 loadEnemiesBool = 0; //used by `asm_functions.s`
volatile s32 saveOrLoadStateMode = 0;
volatile s32 savestateCurrentSlot = 0;
s32 savestate1Size = 0;
s32 savestate2Size = 0;
volatile s32 isSaveOrLoadActive = 0;
s32 stateCooldown = 0;

FATFS FatFs;
char *path = "ct1State.bin"; //example file for SD card writing
FIL sdsavefile = {0};

void loadEnemyObjectsHook(void);
void crash_screen_init(void);
void checkInputsForSavestates(void);

FRESULT initFatFs(void) {
	//Mount SD Card
	return f_mount(&FatFs, "", 0);
}

//mod_boot_func: runs a single time on boot before main game loop starts
void mod_boot_func(void) {
    UINT filebytesread;
    char testString[] = "Testing f_write() call\n";
    FRESULT fileres;
    s32 instructionBuffer[2];
    crash_screen_init();

    #if USE_SD_CARD == TRUE
        //initialize SD card from everdrive, create test file, close
        cart_init();
        initFatFs();
        fileres = f_open(&sdsavefile, path, FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
        f_write(&sdsavefile, testString, ALIGN4(sizeof(testString)), &filebytesread);
        f_close(&sdsavefile);
    #endif

    // example of setting up text to print
    _bzero(&printTextBuffer2, sizeof(printTextBuffer2)); // clear text
    _bzero(&stringBuffer, sizeof(stringBuffer)); // clear string buffer
    strcat(&stringBuffer, "test");
    _sprintfcat(&stringBuffer, " %d", 1);
    convertAsciiToText(&printTextBuffer2, stringBuffer);

    //hookCode((s32*)0x8002D660, &loadEnemyObjectsHook); //example of hooking code
}

//mod_main_per_frame: where to add code that runs every frame before main game loop
void mod_main_per_frame(void) {
    s32 index = 0;
    char printTextBuffer[8];
 
    //example of printing text
    textPrint(13.0f, 30.0f, 1.0f, &printTextBuffer2, 1);

    if (stateCooldown == 0 ) {
        checkInputsForSavestates();
    }

    if (stateCooldown > 0) {
        stateCooldown--;
    }

    
    if (savestateCurrentSlot == 0) {
        printTextBuffer[index++] =  0xA3;
        printTextBuffer[index++] = 0xB1; //prints 1
    } else {
        printTextBuffer[index++] =  0xA3;
        printTextBuffer[index++] = 0xB2; //prints 2         
    }

    printTextBuffer[index++] = 0;
    textPrint(13.0f, 208.0f, 0.65f, &printTextBuffer, 3);

    //if a savestate is being saved/loaded, stall thread
    while (isSaveOrLoadActive != 0) {}
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