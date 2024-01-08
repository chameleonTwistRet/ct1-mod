#ifndef _MOD_MAIN_H_
#define _MOD_MAIN_H_

#include "common.h"
#include "text.h"
#include "../lib/ff.h"

typedef struct tempStructDebug {
    char unk0[0x28];
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
} tempStructDebug;

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
void Debug_ChangeRoom(void);                               /* extern */
void Debug_ChangeView(void);                               /* extern */
void Debug_MovePlayer(void);                               /* extern */
s32 func_80080C28(f32, f32, f32, f32, f32, f32, s32, s32, s32, s32); /* extern */
void EraseToungeEatEnemy(Tongue* arg0);
void EraseToungeEatEnemy_Hook(Tongue* arg0);
s32 toggleStorage(void);
s32 IsPickup(Actor* actor);
void pickup_collide_func(s32);

extern s32 secondarySeedCallsThisFrame;
extern s32 secondarySeedCallsTotal;
extern s32 secondarySeedCallsTotalState1;
extern s32 secondarySeedCallsTotalState2;
extern s32 secondarySeedCallsTotalStateBackup;
extern s32 secondarySeedCallsTotalStateUncompressed;
extern u64 storedElapsedTimeStateUncompressed;
extern s32 freezeTimer;
extern u32 prevCount;
extern u64 displayTimeRTA;
extern u64 displayTimeIGT;
extern u64 elapsedMicroSeconds;
extern u64 storedElapsedTimeState1;
extern u64 storedElapsedTimeState2;
extern u64 storedElapsedTimeStateBackup;
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
extern s32 sDebugInt;
extern unk80174880 D_80174880[];
extern f32 D_800F070C;
extern f32 D_800F0710;
extern f32 D_800F0714;
extern s32 D_800F0B40;
extern char D_8010C7C4[];
extern char D_8010C7CC[];
extern char D_8010C7D4[];
extern char D_8010C7E0[];
extern char D_8010C7F0[];
extern char D_8010C800[];
extern char D_8010C80C[];
extern char D_8010C81C[];
extern char D_8010C824[];
extern char D_8010C838[];
extern char D_8010C850[];
extern char D_8010C860[];
extern char D_8010C870[];
extern char D_8010C888[];
extern char D_8010C89C[];
extern f32 D_8010C8F4;
extern f32 D_8010C8F8;

extern s32 D_801748A8;
extern s32 D_80174980;
extern tempStructDebug D_801768A0;
extern PlayerActor* D_80176B74;
extern s32 D_802478E0;
extern Collision gZoneCollisions[];
extern s32 sDebugCodeSeqStep;
void Rumble_StopAll(void);                                 /* extern */
void func_8007B174(void);                                  /* extern */
void func_8008C440(void);                                  /* extern */
void func_8008C464(void);                                  /* extern */
void func_8008C4E8(void);                                  /* extern */
void func_8008C610(void);                                  /* extern */
void func_8008C698(void);                                  /* extern */
void func_8008C6D4(void);                                  /* extern */
void func_800A7844(void);                                  /* extern */
void func_800A78D0(void);                                  /* extern */
extern s16 D_800FF5C4;
extern s32 D_800FF5C8;
extern s16 D_800FF5CC;
extern s16 D_800FF5D8;
extern char D_8010D530[];
extern char D_8010D540[];
extern char D_8010D544[];
extern char D_8010D548[];
extern char D_8010D54C[];
extern char D_8010D550[];
extern char D_8010D558[];
extern char D_8010D55C[];
extern char D_8010D560[];
extern char D_8010D564[];
extern OSMesgQueue D_801192B8;
extern OSMesgQueue D_801192D0;
extern void* D_801B30A0;
extern OSTimer D_801B3148;
extern OSMesgQueue D_801B35A0;
extern s32 D_802478E0;
void ActorTick_CakeBoss(Actor*);
void ActorTick_CakeBoss_Hook(Actor*);
void ActorInit_CakeBoss_Hook(Actor*);
void func_800360E4(Actor*);
void func_800360E4(Actor*);
void func_800410B4(Actor*);
s32 toggleSpeed(void);
extern void* crash_screen_copy_to_buf(void* dest, const char* src, u32 size);
void textPrint(f32 xPos, f32 yPos, f32 scale, void *text, s32 num);
int __osPiDeviceBusy(void);
s32 compress_lz4_ct_default(const u8* srcData, int srcSize, u8* bufferAddr);
void decompress_lz4_ct_default(int srcSize, int savestateCompressedSize, u8* compressBuffer);
void func_800C2A00(void);
u32 guRandomRev(void);
void func_800C0CDC(PlayerActor* arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_800CFDC8(PlayerActor*);
s32 printCustomDebugText(void);
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
void func_800C0CDC_Hook(PlayerActor* arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void loadEnemyObjectsHook(void);
void crash_screen_init(void);
void func_8004E784_Hook(contMain* arg0, s32 arg1, s32* arg2, contMain* arg3);
void Debug_ChangeRoom_Hook(void);
void Debug_ChangeRoom(void);
void debugMain(void);
void debugMain_Hook(void);
int guRandom_Hook(void);
void Porocess_Mode0_Hook(void);
void DisplayTimerNew(void);
void TrackElapsedTimeFromLastFrame(void);
void DisplayTimerWrapper(void);
void ChameleonFromDoor_Hook(PlayerActor* player, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
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
void PrintElapsedTimeRTA(void);
extern s32 inFrameAdvance;
extern char pathTimes[];
#define ALIGN4(val) (((val) + 0x3) & ~0x3)
#define ARRAY_COUNT(arr) (s32)(sizeof(arr) / sizeof(arr[0]))
#define ramStartAddr (u32)0x800EE140
#define ramEndAddr (u32)0x8036F480
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
extern CustomThread gCustomThread;
extern graphicStruct gGraphicsList[2];
extern s32 gFramebufferIndex;
extern s32 D_800F066C;
extern s32 sDebugMultiplayer;
extern s8 D_800FF8DC;
extern u8 D_800FF8E0[];
extern u8 D_800FF8E4[];
extern s32 textOrangeColor[];
extern s32 textGreenColor[];
extern s32 textPurpleColor[];
extern s32 D_80174980;
extern s32 D_801749AC;
extern u64* storedCount;
extern u64* elapsedCount;
extern u32* storedIGT;
extern u64* prevDoorEntryCount;
extern u32* prevCurrentStageCountRTA;

extern s32 zoneExitID;

typedef struct unkArg0 {
    /* 0x00 */ char unk_00[0x60];
    /* 0x60 */ s16 unk_60;
    /* 0x62 */ s16 unk_62;
    /* 0x64 */ char unk64[4];
    /* 0x68 */ s16 unk68;
} unkArg0;

void getActorHitWithSpatActor(void);
void func_80091A38_Hook(unkArg0* arg0);
void func_80091A38(unkArg0* arg0);
s32 Rand(void);
s32 Rand_Hook(void);

#endif // _MOD_MAIN_H_
