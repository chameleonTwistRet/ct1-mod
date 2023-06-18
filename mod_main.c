#include "mod_main.h"
#include "lib/cart.h"
#include "lib/ff.h"
#include "lib/ffconf.h"
//in assets/ you'll find an example of replacing an image

//func_8002D644_patch.s shows an example of adding .s files and using them with a hook
//this specific .s file allows toggling the spawning of enemies as an example

void loadEnemyObjectsHook(void);
void newPrintf(void);
void crash_screen_init(void);

s32 loadEnemiesBool = 0; //used by `func_8002D644_patch.s`

void hookCode(s32* patchAddr, void* jumpLocation) {
    jumpLocation = (void*)(u32)((((u32)jumpLocation & 0x00FFFFFF) >> 2) | 0x08000000);
    patchAddr[0] = (s32)jumpLocation; //write j instruction
    patchAddr[1] = 0; //write nop
}

void patchInstruction(void* patchAddr, s32 patchInstruction) {
    *(s32*)patchAddr = patchInstruction;
}



#define ALIGN4(val) (((val) + 0x3) & ~0x3)
FATFS FatFs;

FRESULT initFatFs(void) {
	//Mount SD Card
	return f_mount(&FatFs, "", 0);
}

char *path = "ct1State.bin"; //example file for SD card writing
FIL sdsavefile = {0};

//mod_boot_func: runs a single time on boot before main game loop starts
void mod_boot_func(void) {
    UINT filebytesread;
    char testString[] = "Testing f_write() call\n";
    FRESULT fileres;
    s32 instructionBuffer[2];
    crash_screen_init();
    cart_init();
    initFatFs();
    fileres = f_open(&sdsavefile, path, FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
    f_write(&sdsavefile, testString, ALIGN4(sizeof(testString)), &filebytesread);
    f_close(&sdsavefile);
    //hookCode((s32*)0x8002D660, &loadEnemyObjectsHook); //example of hooking code
}


//mod_main_per_frame: where to add code that runs every frame before main game loop
void mod_main_per_frame(void) {
    //
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
        case GAME_MODE_JUNGLE_LAND_MENU:
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
        case GAME_MODE_JUNGLE_LAND:
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