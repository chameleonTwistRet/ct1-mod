#include "common.h"
#include "../../include/menu.h"

char* path = "TEMP.BIN";

#include "../../include/mod_main.h"

char testHex[] = {0x80, 0x81, 0x82, 0x83, 0x84, 0xFF};
void WriteFileToSDCard(void* buffer, s32 size, char* filePath);
void ReadFileFromSDCard(void* buffer, s32 size, char* filePath);

s32 ExportRecording(void) {
    WriteFileToSDCard(&inputRecordingBuffer, ALIGN4(sizeof(InputRecording)), path);
    toggles[TOGGLE_RECORDING] = 0;
}

s32 ImportRecording(void) {
    ReadFileFromSDCard(&inputRecordingBuffer, ALIGN4(sizeof(InputRecording)), path);
}

extern CustomThread gCustomThread;
void savestateMain(void);
s32 StartRecording(void) {
    toggles[TOGGLE_RECORDING] = 1;
    toggles[TOGGLE_PLAYBACK] = 0;

    isSaveOrLoadActive = 1;

    osCreateThread(&gCustomThread.thread, 255, (void*)savestateMain, NULL,
            gCustomThread.stack + sizeof(gCustomThread.stack), 255);
    osStartThread(&gCustomThread.thread);
    stateCooldown = 5;

    while (isSaveOrLoadActive != 0) {}    
}

s32 toggleHideSavestateText(void) {
    toggles[TOGGLE_HIDE_SAVESTATE_TEXT] ^= 1;
    return 1;
}