#ifndef _MOD_MAIN_H_
#define _MOD_MAIN_H_

#include "common.h"
#include "text.h"
#include "../lib/ff.h"

extern FIL fileres;
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
extern s32 isFirstZoneCopy;
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
void func_8002CB6C(Gfx*, void*, s32);
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
void loadEnemyObjectsHook(void);
void crash_screen_init(void);
void func_8004E784_Hook(contMain* arg0, s32 arg1, s32* arg2, contMain* arg3);
void Debug_ChangeRoom_Hook(void);
void Debug_ChangeRoom(void);
void debugMain(void);
void debugMain_Hook(void);
int guRandom_Hook(void);
void Porocess_Mode0_Hook(void);
void DisplayTimer(void);
void ChameleonFromDoor_Hook(playerActor* player, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_800C54F8_Hook(Vec2s*, s32* arg1);
void setTimerParametersBool(void);
void setFreezeTimerAsm(void);
void ActorTick_GhostBoss(Actor* arg0);
void ActorTick_GhostBoss_Hook(Actor* arg0);
void func_8006D688(s32);
void endStageCodeAsm(void);
void endStageCodeAsm2(void);
extern s32 isMenuActive;
extern s32 currPageNo;
extern s32 currOptionNo;
void func_80089BA0(void);
void func_8004E784(contMain* arg0, s32 arg1, s32* arg2, contMain* arg3);
void loadBossDeadEyes(s32);
extern s32 inFrameAdvance;
extern char pathTimes[];
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
extern graphicStruct gGraphicsList[2];
extern s32 gFramebufferIndex;
extern s32 D_800F066C;
extern s32 sDebugMultiplayer;
extern s8 D_800FF8DC;
extern u8 D_800FF8E0[];
extern u8 D_800FF8E4[];
extern s32 D_80174980;
extern s32 D_801749AC;
extern u64* storedCount;
extern u64* elapsedCount;
extern u32* storedIGT;
extern u64* prevDoorEntryCount;
extern u32* prevCurrentStageCountRTA;
extern u32* startingCount;

#endif // _MOD_MAIN_H_
