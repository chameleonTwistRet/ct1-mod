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
void func_800C0760(s32 zone);
extern int getStatusRegister(void); //returns status reg
extern int setStatusRegister(s32); //returns status reg
void updateMenuInput(void);
void updateCustomInputTracking(void);
void checkInputsForSavestates(void);
void optimized_memcpy(void* destination, const void* source, u32 num_bytes);
void setFreezeTimerC(void);
void osSetCount(u32);
extern s32 loadEnemiesBool; //used by `func_8002D644_patch.s`
extern s32 freezeTimer;
extern u64 prevCurrentStageTime;
extern volatile s32 saveOrLoadStateMode;
extern volatile s32 savestateCurrentSlot;
extern s32 savestate1Size;
extern s32 savestate2Size;
extern volatile s32 isSaveOrLoadActive;
extern s32 stateCooldown;
extern s32 gameBootTimer;
extern Collision gZoneCollisions[];
extern s32 isMenuActive;
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
void func_800C0CDC_Hook(playerActor* arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
#define ALIGN4(val) (((val) + 0x3) & ~0x3)
#define ARRAY_COUNT(arr) (s32)(sizeof(arr) / sizeof(arr[0]))
#define ramStartAddr (u32)0x800EE1C0
#define ramEndAddr (u32)0x803B5000
#define KB *(1 <<10)
#define MB *(1 <<20)
#define GB *(1U<<30)
#define ramAddrSavestateDataSlot1 (void*)0x804C0000
#define ramAddrSavestateDataSlot2 (void*)0x805D0000
#define savestateBackup (void*)0x806E0000
//#define ramAddrSavestateRecording (void*)0x806E0000

typedef struct CustomThread {
    /* 0x000 */ OSThread thread;
    /* 0x1B0 */ char stack[0xC000];
    /* 0x9B0 */ OSMesgQueue queue;
    /* 0x9C8 */ OSMesg mesg;
    /* 0x9CC */ u16* frameBuf;
    /* 0x9D0 */ u16 width;
    /* 0x9D2 */ u16 height;
} CustomThread; // size = 0x9D4


#endif // _MOD_MAIN_H_
