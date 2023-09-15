#include "include/mod_main.h"
#include "lib/cart.h"
#include "lib/ff.h"
#include "lib/ffconf.h"
#include "include/sd_toggle.h"
#include "include/menu.h"

char pracTwistVersionString[] = "Practwist v1.1.3.1";
char textBuffer[0x100] = {'\0'};    // Text buffer set to empty string
void gVideoThreadProcessHook(void);
void videoproc_Hook(s32);
u32 xSeed2 = 174823885;
u32 calls = 0;


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

void Debug_ChangeRoom_Hook(void);
void Debug_ChangeRoom(void);
void debugMain(void);
void debugMain_Hook(void);
int guRandom_Hook(void);
void Porocess_Mode0_Hook(void);
void func_80089BA0_Hook(void);
void ChameleonFromDoor_Hook(playerActor* player, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_800C54F8_Hook(Vec2s*, s32* arg1);
void setTimerParametersBool(void);
void setFreezeTimerAsm(void);

//mod_boot_func: runs a single time on boot before main game loop starts
void mod_boot_func(void) {
    u32* startingCount = (u32*)0x80109DC8;
    UINT filebytesread;
    char testString[] = "Testing f_write() call\n";
    FRESULT fileres;
    s32 instructionBuffer[2];
    crash_screen_init();
    *startingCount = osGetCount();

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
    hookCode((s32*)&Debug_ChangeRoom, &Debug_ChangeRoom_Hook);
    hookCode((s32*)&debugMain, &debugMain_Hook);
    hookCode((s32*)&guRandom, &guRandom_Hook);
    hookCode((s32*)&Porocess_Mode0, Porocess_Mode0_Hook);
    hookCode((s32*)&func_80089BA0, &func_80089BA0_Hook);
    hookCode((s32*)&ChameleonFromDoor, &ChameleonFromDoor_Hook);
    hookCode((s32*)&func_800C54F8, &func_800C54F8_Hook);
    hookCode((s32*)0x800C11C8, &setTimerParametersBool);
    hookCode((s32*)0x800C11FC, &setFreezeTimerAsm);
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

    case 3:
        //print call count
        colorTextWrapper(textWhiteColor);
        _bzero(messageBuffer, sizeof(messageBuffer)); //clear buffer
        _sprintf(messageBuffer, "%d", *(u32*)0x80109DC4);
        _bzero(convertedMessageBuffer, sizeof(convertedMessageBuffer)); //clear buffer
        convertAsciiToText(&convertedMessageBuffer, (char*)&messageBuffer);
        textPrint(13.0f, 182.0f, 0.5f, &convertedMessageBuffer, 1);

        //print guSeed
        colorTextWrapper(textWhiteColor);
        _bzero(messageBuffer, sizeof(messageBuffer)); //clear buffer
        _sprintf(messageBuffer, "%08X", *(u32*)0x80109DC0);
        _bzero(convertedMessageBuffer, sizeof(convertedMessageBuffer)); //clear buffer
        convertAsciiToText(&convertedMessageBuffer, (char*)&messageBuffer);
        textPrint(13.0f, 194.0f, 0.5f, &convertedMessageBuffer, 1);
        break;
    case 4:
        //print quintella spin frames
        if (*(s32*)0x8016AD80 >= 92) {
            colorTextWrapper(textGreenMatColor);
        } else {
            colorTextWrapper(textWhiteColor);
        }
        _bzero(messageBuffer, sizeof(messageBuffer)); //clear buffer
        _sprintf(messageBuffer, "%d", *(s32*)0x8016AD80);
        _bzero(convertedMessageBuffer, sizeof(convertedMessageBuffer)); //clear buffer
        convertAsciiToText(&convertedMessageBuffer, (char*)&messageBuffer);
        textPrint(13.0f, 194.0f, 0.5f, &convertedMessageBuffer, 1);
        break;
    case 5:
        _bzero(messageBuffer, sizeof(messageBuffer)); //clear buffer
        _sprintf(messageBuffer, "%d", *(s32*)0x8017499C);
        _bzero(convertedMessageBuffer, sizeof(convertedMessageBuffer)); //clear buffer
        convertAsciiToText(&convertedMessageBuffer, (char*)&messageBuffer);
        textPrint(13.0f, 194.0f, 0.5f, &convertedMessageBuffer, 1);
        break;
    }
}

int guRandom_Hook(void)
{
    unsigned int x;
    u32* seed = (u32*)0x80109DC0;
    u32* calls = (u32*)0x80109DC4;

    x = (*seed<<2) + 2;

    x *= (x+1);
    x = x >> 2;

    *seed = x;
    (*calls)++;

    return( x );
}

u32 guRandomRev(void) {
    int i;
    u32* seed = (u32*)0x80109DC0;
    u32* calls = (u32*)0x80109DC4;
    u32 y, z;
    *seed = ((*seed)<<2) + 2;

    y = 2, z = 1;
    for (i = 0; i < 4; i++) {
        y += ((*seed) - y*(y+1)) * z;
        z = 2*z - z*z*(2*y+1);
    }

    (*calls)--;
    *seed = y >> 2;
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
extern u8 D_80200B68;

enum TrackTimers {
    PP_UNKNOWN,
    PP_COLLISION,
    PP_LIGHT,
    PP_ENVMAP,
    PP_HUD,
    PP_DMA,
    PP_PAD,
    PP_TEXT,
    PP_TEXTURES,
    PP_SHADOW,
    PP_AI,
    PP_WEATHER,
    PP_WAVES,
    PP_DIALOGUE,
    PP_CAMERA,
    PP_MATRIX,
    PP_BACKGROUND,
    PP_BILLBOARD,
    PP_VOID,
    PP_SORTING,

    PP_RSP_GFX,
    PP_RSP_AUD,

    PP_RDP_BUS,
    PP_RDP_BUF,
    PP_RDP_TMM,

    PP_TIMES_TOTAL
};

#define NUM_OBJECT_PRINTS 128
#define NUM_PERF_ITERATIONS 32
#define PERF_AGGREGATE NUM_PERF_ITERATIONS
#define PERF_TOTAL NUM_PERF_ITERATIONS + 1
#define NUM_THREAD_ITERATIONS 8
#define NUM_LOG_LINES 32
typedef u32 PPTimer[NUM_PERF_ITERATIONS + 2];
enum MainTimers {
    PP_LEVELGFX,
    PP_OBJGFX,
    PP_OBJECTS,
    PP_RACER,
    PP_PARTICLEGFX,
    PP_PROFILER_CALC,
    PP_PROFILER_DRAW,

    PP_MAIN_TIMES_TOTAL,
    PP_MAIN_TIME_OFFSET = 3 // This exists to skip the profiler times.
};
enum PPProfilerEvent {
    idk,
    idk2,
    THREAD5_START,
    THREAD5_END,
    THREAD3_START,
    THREAD3_END,
    THREAD4_START,
    THREAD4_END,
    THREAD30_START,
    THREAD30_END,

    NUM_THREAD_TIMERS
};
struct PuppyPrint {
    PPTimer cpuTime; // Sum of multiple CPU timings, and what will be displayed.
    u32 rspTime; // Sum of multiple RSP timings, and publicly shamed on the street.
    u32 rdpTime; // Sum of multiple RDP timings, and hung by its entrails for all to see.
    u32 rspPauseTime; // Buffer that keeps track of the halt time of the Gfx task.
    u32 rspGfxBufTime; // Buffer that keeps track of the current Gfx task.
    u32 rspAudioBufTime; // Buffer that keeps track of the current Audio task.
    PPTimer timers[PP_TIMES_TOTAL]; // Large collection of timers for various things.
    PPTimer coreTimers[PP_MAIN_TIMES_TOTAL]; // Large collection of timers for various things.
    PPTimer audTime; // Normalised total for audio processing time.
    PPTimer gameTime; // Normalised total for game processing time.
    u32 threadTimes[NUM_THREAD_ITERATIONS][NUM_THREAD_TIMERS]; // Timers for individual threads.
    u16 objTimers[NUM_OBJECT_PRINTS][NUM_PERF_ITERATIONS + 2]; // Timers for individual object IDs
    u8 objCounts[NUM_OBJECT_PRINTS];
    u32 mainTimerPoints[2][PP_MAIN_TIMES_TOTAL]; // Timers for individual threads.
    u16 menuScroll; // Page menu scroll value to offset the text.
    s16 pageScroll; // Generic scroller var for a page. Reset when swapped.
    u16 textureLoads; // Tracked number of texture loads.
    u8 threadIteration[NUM_THREAD_TIMERS / 2]; // Number of times the respective thread has looped.
    u8 enabled; // Show the profiler
    u8 menuOpen; // Whether the page menu's open
    u8 page; // Current viewed page.
    s8 menuOption; // Current option in the page menu.
    char logText[NUM_LOG_LINES][127];
};
struct PuppyPrint gPuppyPrint;
u8 perfIteration = 0;
f32 gFPS = 0;
u8 gWidescreen = 0;
s32 sTriCount = 0;
s32 sVtxCount = 0;
s32 prevTime = 0;
u32 sTimerTemp = 0;
u8 gShowHiddenGeometry = FALSE;
u8 gShowHiddenObjects = FALSE;
u32 gFreeMem[12];
u8 sPrintOrder[PP_RSP_GFX];
u16 sObjPrintOrder[NUM_OBJECT_PRINTS];
enum DebugPages {
    PAGE_MINIMAL,
    PAGE_OVERVIEW,
    PAGE_BREAKDOWN,
    PAGE_OBJECTS,
    PAGE_MEMORY,
    PAGE_AUDIO,
    PAGE_LOG,
    PAGE_COVERAGE,

    PAGE_COUNT
};

s32 find_thread_interrupt_offset(u32 lowTime, u32 highTime) {
    s32 i;
    u32 offsetTime = 0;
    // Find if there's been an audio thread update during this thread.
    for (i = 0; i < gPuppyPrint.threadIteration[THREAD3_END / 2]; i++) {
        if (gPuppyPrint.threadTimes[i][THREAD3_END] < highTime &&
            gPuppyPrint.threadTimes[i][THREAD3_START] > lowTime) {
            offsetTime += gPuppyPrint.threadTimes[i][THREAD3_END] - gPuppyPrint.threadTimes[i][THREAD3_START];
        }
    }
    return offsetTime;
}

void calculate_core_timers(void) {
    s32 i;
    s32 lowTime;
    s32 highTime;
    s32 offsetTime = 0;


    for (i = 0; i < PP_MAIN_TIMES_TOTAL; i++) {
        offsetTime = 0;
        lowTime = gPuppyPrint.mainTimerPoints[0][i];
        highTime = gPuppyPrint.mainTimerPoints[1][i];
        offsetTime = find_thread_interrupt_offset(lowTime, highTime);
        gPuppyPrint.coreTimers[i][PERF_AGGREGATE] -= gPuppyPrint.coreTimers[i][perfIteration];
        gPuppyPrint.coreTimers[i][perfIteration] = MIN(highTime - lowTime - offsetTime, (s32) OS_USEC_TO_CYCLES(99999));
        gPuppyPrint.coreTimers[i][PERF_AGGREGATE] += gPuppyPrint.coreTimers[i][perfIteration];
        gPuppyPrint.coreTimers[i][PERF_TOTAL] = OS_CYCLES_TO_USEC(gPuppyPrint.coreTimers[i][PERF_AGGREGATE]) / NUM_PERF_ITERATIONS;
    }

    gPuppyPrint.coreTimers[PP_OBJECTS][perfIteration] -= gPuppyPrint.coreTimers[PP_RACER][perfIteration];
    gPuppyPrint.coreTimers[PP_OBJECTS][PERF_AGGREGATE] -= gPuppyPrint.coreTimers[PP_RACER][perfIteration];
    gPuppyPrint.coreTimers[PP_LEVELGFX][perfIteration] -= gPuppyPrint.coreTimers[PP_OBJGFX][perfIteration];
    gPuppyPrint.coreTimers[PP_LEVELGFX][PERF_AGGREGATE] -= gPuppyPrint.coreTimers[PP_OBJGFX][perfIteration];
    gPuppyPrint.coreTimers[PP_LEVELGFX][perfIteration] -= gPuppyPrint.coreTimers[PP_PARTICLEGFX][perfIteration];
    gPuppyPrint.coreTimers[PP_LEVELGFX][PERF_AGGREGATE] -= gPuppyPrint.coreTimers[PP_PARTICLEGFX][perfIteration];
    gPuppyPrint.coreTimers[PP_LEVELGFX][perfIteration] += gPuppyPrint.coreTimers[PP_PARTICLEGFX][perfIteration];
    gPuppyPrint.coreTimers[PP_LEVELGFX][PERF_AGGREGATE] += gPuppyPrint.coreTimers[PP_PARTICLEGFX][perfIteration];
}
void calculate_individual_thread_timers(void) {
    s32 i;
    s32 j;
    u32 highTime;
    u32 lowTime;
    u32 normalTime;
    u32 offsetTime;

    // Audio thread is basically top prio, so no need to do any further shenanigans
    highTime = 0;
    lowTime = 0xFFFFFFFF;
    for (i = THREAD3_START; i <= THREAD3_END; i++) {
        for (j = 0; j < gPuppyPrint.threadIteration[i / 2]; j++) {
            // If an iteration crosses over mid read, the low time could be zero, which would ruin this whole gig.
            if (gPuppyPrint.threadTimes[gPuppyPrint.threadIteration[j]][i] < lowTime && 
                gPuppyPrint.threadTimes[gPuppyPrint.threadIteration[j]][i] != 0) {
                lowTime = gPuppyPrint.threadTimes[gPuppyPrint.threadIteration[j]][i];
            }
            if (gPuppyPrint.threadTimes[gPuppyPrint.threadIteration[j]][i] > highTime) {
                highTime = gPuppyPrint.threadTimes[gPuppyPrint.threadIteration[j]][i];
            }
        }
    }
    gPuppyPrint.audTime[PERF_AGGREGATE] -= gPuppyPrint.audTime[perfIteration];
    gPuppyPrint.audTime[perfIteration] = MIN(highTime - lowTime, (s32) OS_USEC_TO_CYCLES(99999));
    gPuppyPrint.audTime[PERF_AGGREGATE] += gPuppyPrint.audTime[perfIteration];
    gPuppyPrint.audTime[PERF_TOTAL] = OS_CYCLES_TO_USEC(gPuppyPrint.audTime[PERF_AGGREGATE]) / NUM_PERF_ITERATIONS;
    // Game thread, unfortunately, does not. We have to take the times of the audio too, so we can offset the values for accuracy.
    highTime = 0;
    offsetTime = 0;
    lowTime = 0xFFFFFFFF;
    for (i = THREAD4_START; i <= THREAD4_END; i++) {
        for (j = 0; j < gPuppyPrint.threadIteration[i / 2]; j++) {
            // If an iteration crosses over mid read, the low time could be zero, which would ruin this whole gig.
            normalTime = gPuppyPrint.threadTimes[gPuppyPrint.threadIteration[j]][i];
            if (normalTime < lowTime &&  normalTime != 0) {
                lowTime = normalTime;
            }
            offsetTime += find_thread_interrupt_offset(lowTime, normalTime);
            if (normalTime > highTime) {
                highTime = normalTime;
            }
        }
    }
    gPuppyPrint.gameTime[PERF_AGGREGATE] -= gPuppyPrint.gameTime[perfIteration];
    gPuppyPrint.gameTime[perfIteration] = MIN(highTime - lowTime - offsetTime, (s32) OS_USEC_TO_CYCLES(99999));
    gPuppyPrint.gameTime[PERF_AGGREGATE] += gPuppyPrint.gameTime[perfIteration];
    gPuppyPrint.gameTime[PERF_TOTAL] = OS_CYCLES_TO_USEC(gPuppyPrint.gameTime[PERF_AGGREGATE]) / NUM_PERF_ITERATIONS;
}
/// Add whichever times you wish to create aggregates of.
void puppyprint_calculate_average_times(void) {
    s32 i;
    s32 j;
    u32 highTime = 0;
    u32 lowTime = 0xFFFFFFFF;

    gPuppyPrint.timers[PP_RSP_AUD][PERF_TOTAL] = OS_CYCLES_TO_USEC(gPuppyPrint.timers[PP_RSP_AUD][PERF_AGGREGATE]) / NUM_PERF_ITERATIONS;
    gPuppyPrint.timers[PP_RSP_GFX][PERF_TOTAL] = OS_CYCLES_TO_USEC(gPuppyPrint.timers[PP_RSP_GFX][PERF_AGGREGATE]) / NUM_PERF_ITERATIONS;

    for (i = 1; i < PP_RDP_BUS; i++) {
        gPuppyPrint.timers[i][PERF_TOTAL] = OS_CYCLES_TO_USEC(gPuppyPrint.timers[i][PERF_AGGREGATE]) / NUM_PERF_ITERATIONS;
    }
    
    for (i = 1; i < NUM_OBJECT_PRINTS; i++) {
        gPuppyPrint.objTimers[i][PERF_TOTAL] = (gPuppyPrint.objTimers[i][PERF_AGGREGATE]) / NUM_PERF_ITERATIONS;
    }
    gPuppyPrint.timers[PP_RDP_BUF][PERF_TOTAL] = (gPuppyPrint.timers[PP_RDP_BUF][PERF_AGGREGATE] * 10) / (625*NUM_PERF_ITERATIONS);
    gPuppyPrint.timers[PP_RDP_BUS][PERF_TOTAL] = (gPuppyPrint.timers[PP_RDP_BUS][PERF_AGGREGATE] * 10) / (625*NUM_PERF_ITERATIONS);
    gPuppyPrint.timers[PP_RDP_TMM][PERF_TOTAL] = (gPuppyPrint.timers[PP_RDP_TMM][PERF_AGGREGATE] * 10) / (625*NUM_PERF_ITERATIONS);
    gPuppyPrint.rspTime = gPuppyPrint.timers[PP_RSP_AUD][PERF_TOTAL] + gPuppyPrint.timers[PP_RSP_GFX][PERF_TOTAL];
    gPuppyPrint.rdpTime = MAX(gPuppyPrint.timers[PP_RDP_BUF][PERF_TOTAL], gPuppyPrint.timers[PP_RDP_BUS][PERF_TOTAL]);
    gPuppyPrint.rdpTime = MAX(gPuppyPrint.timers[PP_RDP_TMM][PERF_TOTAL], gPuppyPrint.rdpTime);
    // Find the earliest snapshot and the latest snapshot.
    for (i = 0; i < NUM_THREAD_TIMERS; i++) {
        for (j = 0; j < gPuppyPrint.threadIteration[i / 2]; j++) {
            // If an iteration crosses over mid read, the low time could be zero, which would ruin this whole gig.
            if (gPuppyPrint.threadTimes[gPuppyPrint.threadIteration[j]][i] < lowTime && 
                gPuppyPrint.threadTimes[gPuppyPrint.threadIteration[j]][i] != 0) {
                lowTime = gPuppyPrint.threadTimes[gPuppyPrint.threadIteration[j]][i];
            }
            if (gPuppyPrint.threadTimes[gPuppyPrint.threadIteration[j]][i] > highTime) {
                highTime = gPuppyPrint.threadTimes[gPuppyPrint.threadIteration[j]][i];
            }
        }
    }
    calculate_core_timers();
    calculate_individual_thread_timers();
    gPuppyPrint.cpuTime[PERF_AGGREGATE] -= gPuppyPrint.cpuTime[perfIteration];
    gPuppyPrint.cpuTime[perfIteration] = MIN(highTime - lowTime, OS_USEC_TO_CYCLES(99999));
    gPuppyPrint.cpuTime[PERF_AGGREGATE] += gPuppyPrint.cpuTime[perfIteration];
    gPuppyPrint.cpuTime[PERF_TOTAL] = OS_CYCLES_TO_USEC(gPuppyPrint.cpuTime[PERF_AGGREGATE]) / NUM_PERF_ITERATIONS;
    _bzero(&gPuppyPrint.threadIteration, sizeof(gPuppyPrint.threadIteration));
    _bzero(&gPuppyPrint.threadTimes, sizeof(gPuppyPrint.threadTimes));
    _bzero(&gPuppyPrint.mainTimerPoints, sizeof(gPuppyPrint.mainTimerPoints));
    // if (gPuppyPrint.enabled) {
    //     if (gPuppyPrint.page == PAGE_BREAKDOWN) {
    //         calculate_print_order();
    //     } else if (gPuppyPrint.page == PAGE_OBJECTS) {
    //         calculate_obj_print_order();
    //     }
    // }
}

//mod_main_per_frame: where to add code that runs every frame before main game loop
void mod_main_per_frame(void) {
    s32 index = 0;
    char textBuffer[8];
    char convertedVersionBuffer[sizeof(pracTwistVersionString) * 2];
    f32* acceleration = (f32*)0x80168DEC;
    //puppyprint_calculate_average_times();
    gLevelAccessBitfeild = 0xFF;
    gLevelClearBitfeild = 0xFF;
    D_80200B68 = 0xFF; //unlock all levels
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

    if (toggles[TOGGLE_HIDE_IGT] > 0) {
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

    if (freezeTimer > 0) {
        freezeTimer--;
    }

    

    
    // totalCount = totalCount + currentCount;
    // if (currentCount)

    

    // if (toggles[TOGGLE_SPEED] == 1) {
    //     *acceleration = 1.66f;
    // } else {
    //     *acceleration = 0.32f;
    // }

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