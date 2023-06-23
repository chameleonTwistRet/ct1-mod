#include "include/mod_main.h"
#include "lib/cart.h"
#include "lib/ff.h"
#include "lib/ffconf.h"
#include "include/sd_toggle.h"
#include "include/menu.h"

char pracTwistVersionString[] = "Practwist v0.1";
char textBuffer[0x100] = {'\0'};    // Text buffer set to empty string
void gVideoThreadProcessHook(void);
void videoproc_Hook(s32);
// Patches work the same way as 81 and 80 GameShark Codes
void s16patch(void* patchAddr, s16 patchInstruction) {
    *(s16*)patchAddr = patchInstruction;
}
void s8patch(void* patchAddr, s8 patchInstruction) {
    *(s8*)patchAddr = patchInstruction;
}

void textPrint(f32 xPos, f32 yPos, f32 scale, void *text, s32 num) {
    PrintText(xPos, yPos, 0, scale, 0, 0, text, num);
}

//in assets/ you'll find an example of replacing an image
s32 recordingErrorMessageStick = 0;
s32 loadEnemiesBool = 0; //used by `asm_functions.s`
volatile s32 saveOrLoadStateMode = 0;
volatile s32 savestateCurrentSlot = 0;
s32 savestate1Size = 0;
s32 savestate2Size = 0;
volatile s32 isSaveOrLoadActive = 0;
s32 stateCooldown = 0;
s32 currentlyPressedButtons = 0;
s32 previouslyPressedButtons = 0;
FATFS FatFs;
char *path = "ct1State.bin"; //example file for SD card writing
FIL sdsavefile = {0};

void loadEnemyObjectsHook(void);
void crash_screen_init(void);
void func_8004E784_Hook(contMain* arg0, s32 arg1, s32* arg2, contMain* arg3);

FRESULT initFatFs(void) {
	//Mount SD Card
	return f_mount(&FatFs, "", 0);
}

InputRecording inputRecordingBuffer; //1200 frames
u32 recordingInputIndex = 0;

void checkIfRecordInputs(void) {
    if (toggles[TOGGLE_RECORDING] == 0) {
        return;
    }

    inputRecordingBuffer.recordingBuffer[recordingInputIndex++] = gContMain[0]; //copy player 1's inputs
    inputRecordingBuffer.totalFrameCount = recordingInputIndex;
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
        // fileres = f_open(&sdsavefile, path, FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
        // f_write(&sdsavefile, testString, ALIGN4(sizeof(testString)), &filebytesread);
        // f_close(&sdsavefile);
    #endif

    // example of setting up text to print
    _bzero(&textBuffer, sizeof(textBuffer)); // clear text
    convertAsciiToText(&textBuffer, "test text");

    hookCode((s32*)0x8002D660, &loadEnemyObjectsHook); //toggle enemies spawning/not spawning
    hookCode((s32*)0x8004E784, &func_8004E784_Hook); //hook controller reading of overworld gamemode
    hookCode((s32*)0x800C0CDC, &func_800C0CDC_Hook); //hook load boss function
    //hookCode((s32*)0x80084C08, &gVideoThreadProcessHook); //hook video process to pause on loadstate
    hookCode((s32*)0x80084b30, &videoproc_Hook); //hook video process to pause on loadstate
    
    //

    patchInstruction((void*)0x800A1030, 0x10000002); //add black chameleon to story patch 1
    patchInstruction((void*)0x800A1084, 0x10000004); //add black chameleon to story patch 2
    
}

f32 fabsf(f32 x) {
    if (x < 0.0f) {
        return -x;
    }
    return x;
}

f32 clampTo45Degrees(f32 theta) {
    if (fabsf(theta) > 45.0f) {
        s32 thetaInt = theta;

        if (thetaInt >= 0) {
            thetaInt = -(thetaInt / 45 * 45) + thetaInt;
        } else {
            thetaInt = -thetaInt;
            thetaInt = (thetaInt / 45 * 45) - thetaInt;
        }

        return thetaInt;
    }

    do {
        if (theta < 0.0f) {
            theta += 45.0f;
        }
        if (theta >= 45.0f) {
            theta -= 45.0f;
        }
    } while (!(theta >= 0.0f) || !(theta < 45.0f));

    return theta;
}


extern s32 isMenuActive;
extern s32 currPageNo;
extern s32 currOptionNo;
void func_80089BA0(void);

void printCustomDebugText(void) {
    char messageBuffer[20];
    char convertedMessageBuffer[sizeof(messageBuffer) * 2];
    f32 freeCamAngle = 0.0f;
    f32 clampedAngle = 0.0f;
    s32 textColor;

    switch (toggles[TOGGLE_CUSTOM_DEBUG_TEXT]) {
    case 1:
        colorTextWrapper(textGreenMatColor);
        _bzero(messageBuffer, sizeof(messageBuffer)); //clear buffer
        _sprintf(messageBuffer, "SPD: %.2f", gPlayerActors[0].forwardVel);
        _bzero(convertedMessageBuffer, sizeof(convertedMessageBuffer)); //clear buffer
        convertAsciiToText(&convertedMessageBuffer, (char*)&messageBuffer);
        textPrint(13.0f, 158.0f, 0.5f, &convertedMessageBuffer, 1);

        colorTextWrapper(textCyanColor);
        _bzero(messageBuffer, sizeof(messageBuffer)); //clear buffer
        _sprintf(messageBuffer, "ANG: %.2f", gPlayerActors[0].yAngle);
        _bzero(convertedMessageBuffer, sizeof(convertedMessageBuffer)); //clear buffer
        convertAsciiToText(&convertedMessageBuffer, (char*)&messageBuffer);
        textPrint(13.0f, 170.0f, 0.5f, &convertedMessageBuffer, 1);

        colorTextWrapper(textWhiteColor);
        _bzero(messageBuffer, sizeof(messageBuffer)); //clear buffer
        _sprintf(messageBuffer, "X: %.2f", gPlayerActors[0].pos.x);
        _bzero(convertedMessageBuffer, sizeof(convertedMessageBuffer)); //clear buffer
        convertAsciiToText(&convertedMessageBuffer, (char*)&messageBuffer);
        textPrint(13.0f, 182.0f, 0.5f, &convertedMessageBuffer, 1);

        colorTextWrapper(textWhiteColor);
        _bzero(messageBuffer, sizeof(messageBuffer)); //clear buffer
        _sprintf(messageBuffer, "Z: %.2f", gPlayerActors[0].pos.z);
        _bzero(convertedMessageBuffer, sizeof(convertedMessageBuffer)); //clear buffer
        convertAsciiToText(&convertedMessageBuffer, (char*)&messageBuffer);
        textPrint(13.0f, 194.0f, 0.5f, &convertedMessageBuffer, 1);
        break;
    case 2:
        freeCamAngle = gCamera[0].f1.x;
        clampedAngle = clampTo45Degrees(freeCamAngle);
        if (clampedAngle > 6.17f && clampedAngle < 8.56f) { //check exact ranges
            colorTextWrapper(textGreenMatColor);
        } else {
            colorTextWrapper(textWhiteColor);
        }
        _bzero(messageBuffer, sizeof(messageBuffer)); //clear buffer
        _sprintf(messageBuffer, "CAM: %.2f", freeCamAngle);
        _bzero(convertedMessageBuffer, sizeof(convertedMessageBuffer)); //clear buffer
        convertAsciiToText(&convertedMessageBuffer, (char*)&messageBuffer);
        textPrint(13.0f, 194.0f, 0.5f, &convertedMessageBuffer, 1);
        break;

    }


}

extern u32 nextZone;
u8 timesvaulted = 0;

s32 caveSkipPractice(void) {
    f32 caveAngleDiff;
    f32 caveAngleDiffAbs;
    f32 caveZDiff;
    f32 caveZDiffAbs;

    char messageBuffer[20];
    char convertedMessageBuffer[sizeof(messageBuffer) * 2];

    if ((gameModeCurrent == GAME_MODE_OVERWORLD) && (gCurrentStage == 0) && (nextZone == 0) && (toggles[TOGGLE_CAVE_SKIP_PRACTICE] == 1)) {
        // Printout Location
        f32 xPos = 20.0f;
        f32 yPos = 35.0f;
        f32 scale = 0.5f;
        s32 style = 3;

        // Data Needed
        if (gTongues[0].vaultTime == 1) {
            timesvaulted++;
        }

        if (timesvaulted > 0) {
            caveAngleDiff = (gPlayerActors[0].yAngle - 90.0f);
            caveAngleDiffAbs = ((caveAngleDiff < 0) ? -caveAngleDiff : caveAngleDiff);
            if (caveAngleDiffAbs < 5.0f) {
                colorTextWrapper(textGreenColor);
            }
            else {
                colorTextWrapper(textRedColor);
            }
            _bzero(&messageBuffer, sizeof(messageBuffer)); //clear buffer
            _sprintf(messageBuffer, "ANGLE OFF BY: %.4f\n", caveAngleDiffAbs);
		    _bzero(&convertedMessageBuffer, sizeof(convertedMessageBuffer));
		    convertAsciiToText(&convertedMessageBuffer, (char*)&messageBuffer);
		    PrintText(xPos, yPos, 0, scale, 0, 0, convertedMessageBuffer, style);

            caveZDiff = (2925.0f - gPlayerActors[0].pos.z);
            caveZDiffAbs = ((caveZDiff < 0) ? -caveZDiff : caveZDiff);

            if ((gPlayerActors[0].pos.z < 2930.0f) && (gPlayerActors[0].pos.z > 2920.0f)) {
                colorTextWrapper(textGreenColor);
            }
            else {
                colorTextWrapper(textRedColor);
            }
            _sprintf(messageBuffer, "Z OFF BY: %.4f\n", caveZDiffAbs);
			_bzero(&convertedMessageBuffer, sizeof(convertedMessageBuffer)); //clear buffer
			convertAsciiToText(&convertedMessageBuffer, (char*)&messageBuffer);
			PrintText(xPos, (yPos += 10.0f), 0, scale, 0, 0, convertedMessageBuffer, style);
        }
    }
    return 1;
}


extern u8 gLevelAccessBitfeild;
extern u8 gLevelClearBitfeild;

//mod_main_per_frame: where to add code that runs every frame before main game loop
void mod_main_per_frame(void) {
    s32 index = 0;
    char textBuffer[8];
    char convertedVersionBuffer[sizeof(pracTwistVersionString) * 2];

    gLevelAccessBitfeild = 0xFF;
    gLevelClearBitfeild = 0xFF;
    gGameRecords.flags[1] = 0x04; //give white

    if (sDebugInt == -1) {
        sDebugInt = 0;
    }

    if (toggles[TOGGLE_INFINITE_HEALTH] == 1) {gPlayerActors[0].hp = 0x0A;}

    checkIfRecordInputs();
    caveSkipPractice();
    //updateCustomInputTracking();

    if (stateCooldown > 0) {
        stateCooldown--;
    }

    if (isMenuActive == 1) {
        pageMainDisplay(currPageNo, currOptionNo);
        //updateMenuInput();
    }

    if (toggles[TOGGLE_HIDE_SAVESTATE_TEXT] == 1) {
            if (savestateCurrentSlot == 0) {
            textBuffer[index++] =  0xA3;
            textBuffer[index++] = 0xB1; //prints 1
        } else {
            textBuffer[index++] =  0xA3;
            textBuffer[index++] = 0xB2; //prints 2         
        }

        textBuffer[index++] = 0;

        textPrint(13.0f, 208.0f, 0.65f, &textBuffer, 3);
    }

    if (toggles[TOGGLE_HIDE_IGT] == 1) {
        if (gameModeCurrent == GAME_MODE_OVERWORLD){
            if (gIsPaused == 0) {
                func_80089BA0();
            }
        }
    }

    if (toggles[TOGGLE_CUSTOM_DEBUG_TEXT] != 0) {
        printCustomDebugText();
    }

    if (gameModeCurrent == GAME_MODE_SUPPLY_SYSTEM_LOGO) {
        colorTextWrapper(textGreenMatColor);
        convertAsciiToText(&convertedVersionBuffer, (char*)&pracTwistVersionString);
        textPrint(15.0f, 220.0f, 0.5f, &convertedVersionBuffer, 3);
    }

    if (isMenuActive == 0 ) {
        checkInputsForSavestates();
    }

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