#ifndef _MOD_MAIN_H_
#define _MOD_MAIN_H_

#include "common.h"
#include "text.h"

extern Addr* mod_ROM_START;
extern Addr* mod_VRAM;
extern Addr* mod_ROM_END;
extern Addr* mod_ROM_START;
void mod_main_per_frame(void);
void mod_boot_func(void);

extern int getStatusRegister(void); //returns status reg
extern int setStatusRegister(s32); //returns status reg
extern s32 display_string_frames;
extern s32 loadEnemiesBool; //used by `func_8002D644_patch.s`
extern volatile s32 saveOrLoadStateMode;
extern volatile s32 savestateCurrentSlot;
extern s32 savestate1Size;
extern s32 savestate2Size;
extern volatile s32 isSaveOrLoadActive;
extern s32 stateCooldown;
extern s32 gameBootTimer;
extern s32 textCyanColor[];
extern s32 textGreenColor[];
extern s32 textGreenMatColor[];
extern s32 textWhiteColor[];
extern s32 textRedColor[];
extern s32 textOrangeColor[];
extern s32 textPurpleColor[];

void colorTextWrapper(s32* color);
void osInvalICache(void*, s32);
void osInvalDCache(void*, s32);
int __osDpDeviceBusy(void);
int __osSpDeviceBusy(void);
int __osSiDeviceBusy(void);
extern int __osDpDeviceBusy(void);
extern int getStatusRegister(void); //returns status reg
extern int setStatusRegister(s32); //returns status reg
void hookCode(s32* patchAddr, void* jumpLocation);
void patchInstruction(void* patchAddr, s32 patchInstruction);
void _sprintf(void* destination, void* fmt, ...);
char* strcat(char* dest, const char* src);
void _sprintfcat(void* destination, void* fmt, ...);
void convertAsciiToText(void* buffer, char* source);
void pageMainDisplay(s32 currPageNo, s32 currOptionNo);

#define ALIGN4(val) (((val) + 0x3) & ~0x3)
#define ARRAY_COUNT(arr) (s32)(sizeof(arr) / sizeof(arr[0]))
#define ramStartAddr (u32)0x800EE140
#define ramEndAddr (u32)0x803B5000
#define KB *(1 <<10)
#define MB *(1 <<20)
#define GB *(1U<<30)
#define ramAddrSavestateDataSlot1 (void*)0x804C0000
#define ramAddrSavestateDataSlot2 (void*)0x805D0000

#define ARRAY_COUNT(arr) (s32)(sizeof(arr) / sizeof(arr[0]))
#define ARRAY_COUNT_INDEX(arr) ARRAY_COUNT(arr) - 1

extern s32 currPageNo;
extern s32 currOptionNo;
extern OSContPad D_80175650[MAXCONTROLLERS];
extern s32 isMenuActive;

typedef struct contMainShort {
    u16 buttons;
    s8 stickx;
    s8 sticky;
} contMainShort;

extern s8 toggles[];
enum Toggles {
    NO_TOGGLE = 0,
    //page 0
    TOGGLE_HIDE_SAVESTATE_TEXT,
    //page1
    TOGGLE_RECORDING,
    TOGGLE_PLAYBACK,
    EXPORT_RECORDING,
    //page2
    TOGGLE_SAVESTATE_SLOT
};

typedef struct CustomThread {
    /* 0x000 */ OSThread thread;
    /* 0x1B0 */ char stack[0xC000];
    /* 0x9B0 */ OSMesgQueue queue;
    /* 0x9C8 */ OSMesg mesg;
    /* 0x9CC */ u16* frameBuf;
    /* 0x9D0 */ u16 width;
    /* 0x9D2 */ u16 height;
} CustomThread; // size = 0x9D4

extern CustomThread gCustomThread;

typedef struct InputRecording {
    u32 totalFrameCount;
    u32 framePlaybackIndex;
    contMainShort recordingBuffer[72000]; //40 minutes of recording possible
} InputRecording;
extern InputRecording inputRecordingBuffer;

typedef struct SaveStateSavedVariables {
/* 0x00 */ u32 playbackIndex;
/* 0x04 */ u32 recordingFrameCount;
/* 0x08 */ s32 curSavestateSlot;
/* 0x0C */ char space[0x74];
} SaveStateSavedVariables; //sizeof 0x80
extern SaveStateSavedVariables* SaveStateVars;

#endif // _MOD_MAIN_H_
