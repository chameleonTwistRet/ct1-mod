#include "common.h"
#include "../include/menu.h"
#include "../include/mod_main.h"

#define BOMBLAND_STAGE_ID 2
#define BOMBLAND_SNAKE_BOSS_ID 0xB

u32 prevCount = 0;
u64 prevDoorEntryTime = 0;
u64 totalElapsedCounts = 0;
u64 elapsedMicroSeconds = 0;
u64 displayTimeRTA = 0;
u64 displayTimeIGT = 0;

u64 storedElapsedTimeState1 = 0;
u64 storedElapsedTimeState2 = 0;
u64 storedElapsedTimeStateBackup = 0;

s32 secondarySeedCallsTotalState1 = 0;
s32 secondarySeedCallsTotalState2 = 0;
s32 secondarySeedCallsTotalStateBackup = 0;

u64 snakeTime = 0x00000000010C2300;
s32 zoneExitID = 0;
u64 BLZoneExitTimesTAS[] = {
    0x00000000001F3B48, //first room exit
    0x0000000000653180, //second room exit
    0x0000000000497480, //third room exit
    0x000000000065B500, //pillar 1 exit
    0x000000000063A700, //lava 1 exit
    0x00000000002BA980, //spin room exit
    0x00000000004F9E80, //room before cannon 1
    0x000000000062A000, //cannon 1 exit
    0x0000000000A98300, //pillar 2 exit
    0x0000000000621C80, //lava 2 exit
    0x000000000030CC80, //bomb jump exit
    0x000000000052B380, //door clip 1 exit
    0x00000000005D7D00, //cannon 2
    0x0000000000621C80, //boss door exit
    0x00000000010C2300, //snake death
};

void TrackElapsedTimeFromLastFrame(void) {
    //first, update timers
    u32 curCount = osGetCount();
    u64 countTemp = curCount;

    //count register overflowed, calculate accordingly
    if (curCount < prevCount) {
        countTemp += 0x100000000;
    }

    totalElapsedCounts += (countTemp - prevCount);
    prevCount = curCount;
}

void PrintElapsedTimeInRoomRTA(u64 timeDiffInRoom) {
    char timeString[32];
    char convertedBuffer[sizeof(timeString) * 2];
    s32 minutes, seconds, milliseconds;

    if (freezeTimer != 0) {
        colorTextWrapper(textGreenMatColor);
        timeDiffInRoom = OS_CYCLES_TO_USEC(timeDiffInRoom);
        milliseconds = (timeDiffInRoom / 1000) % 1000;
        seconds = (timeDiffInRoom / 1000000) % 60;
        minutes = (timeDiffInRoom / 1000000) / 60;

        _sprintf(timeString, "%02d\'%02d\"%03d", minutes, seconds, milliseconds);
        convertAsciiToText(&convertedBuffer, (char*)&timeString);
        PrintText(220.0f, 194.0f, 0.0f, 0.5f, 0.0f, 0.0f, convertedBuffer, 1);
    }
}

void PrintTimeComparisonToTAS(u64 timeDiffInRoom) {
    char timeString[32]; // Assuming a maximum of 99:59.999
    char convertedBuffer[64];
    s32 minutes, seconds, milliseconds;

    if (freezeTimer == 0) {
        return;
    }

    timeDiffInRoom = OS_CYCLES_TO_USEC(timeDiffInRoom);

    if (gCurrentStage == BOMBLAND_SNAKE_BOSS_ID) {
        if (timeDiffInRoom < snakeTime) {
            timeDiffInRoom = snakeTime - timeDiffInRoom;
            colorTextWrapper(textGreenMatColor);
            milliseconds = (timeDiffInRoom / 1000) % 1000;
            seconds = (timeDiffInRoom / 1000000) % 60;
            minutes = (timeDiffInRoom / 1000000) / 60;
            _sprintf(timeString, "-%02d\'%02d\"%03d", minutes, seconds, milliseconds);
        } else {
            timeDiffInRoom = timeDiffInRoom - snakeTime;
            colorTextWrapper(textOrangeColor);
            milliseconds = (timeDiffInRoom / 1000) % 1000;
            seconds = (timeDiffInRoom / 1000000) % 60;
            minutes = (timeDiffInRoom / 1000000) / 60;
            _sprintf(timeString, "+%02d\'%02d\"%03d", minutes, seconds, milliseconds);
        }
        convertAsciiToText(&convertedBuffer, (char*)&timeString);
        PrintText(220.0f, 194.0f, 0.0f, 0.5f, 0.0f, 0.0f, convertedBuffer, 1);
        return;
    }

    //if gCurrentStage is 2 (BL)
    if (freezeTimer != 0)  {
        if (timeDiffInRoom < BLZoneExitTimesTAS[zoneExitID]) {
            timeDiffInRoom = BLZoneExitTimesTAS[zoneExitID] - timeDiffInRoom;
            colorTextWrapper(textGreenMatColor);
            milliseconds = (timeDiffInRoom / 1000) % 1000;
            seconds = (timeDiffInRoom / 1000000) % 60;
            minutes = (timeDiffInRoom / 1000000) / 60;
            _sprintf(timeString, "-%02d\'%02d\"%03d", minutes, seconds, milliseconds);
        } else {
            timeDiffInRoom = timeDiffInRoom - BLZoneExitTimesTAS[zoneExitID];
            colorTextWrapper(textOrangeColor);
            milliseconds = (timeDiffInRoom / 1000) % 1000;
            seconds = (timeDiffInRoom / 1000000) % 60;
            minutes = (timeDiffInRoom / 1000000) / 60;
            _sprintf(timeString, "+%02d\'%02d\"%03d", minutes, seconds, milliseconds);
        }


        
        convertAsciiToText(&convertedBuffer, (char*)&timeString);
        PrintText(220.0f, 194.0f, 0.0f, 0.5f, 0.0f, 0.0f, convertedBuffer, 1);
    }
}

void PrintElapsedTimeRTA(void) {
    s32 minutes, seconds, milliseconds;
    char timeString[32]; // Assuming a maximum of 99:59.999
    char convertedBuffer[64];
    u64 displayTime;
    u64 timeDiffInRoom;

    if (freezeTimer == 0) {
        displayTime = totalElapsedCounts;
    } else {
        displayTime = displayTimeRTA;
    }

    timeDiffInRoom = displayTime - prevDoorEntryTime;

    if (toggles[TOGGLE_DISPLAY_INDIVIDUAL_ROOM_TIME]) {
        PrintElapsedTimeInRoomRTA(timeDiffInRoom);
    } else if (toggles[TOGGLE_TAS_COMPARISON]) {
        PrintTimeComparisonToTAS(timeDiffInRoom);
    }

    if (freezeTimer != 0) {
        colorTextWrapper(textCyanColor);
    } else {
        colorTextWrapper(textPurpleColor);
    }

    displayTime = OS_CYCLES_TO_USEC(displayTime);
    milliseconds = (displayTime / 1000) % 1000;
    seconds = (displayTime / 1000000) % 60;
    minutes = (displayTime / 1000000) / 60;

    _sprintf(timeString, "%02d\'%02d\"%03d", minutes, seconds, milliseconds);
    convertAsciiToText(&convertedBuffer, (char*)&timeString);
    PrintText(220.0f, 208.0f, 0.0f, 0.5f, 0.0f, 0.0f, convertedBuffer, 1);
}