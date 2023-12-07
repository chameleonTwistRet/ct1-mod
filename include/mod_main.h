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

extern s32 loadEnemiesBool; //used by `func_8002D644_patch.s`
extern volatile s32 saveOrLoadStateMode;
extern volatile s32 savestateCurrentSlot;
extern s32 savestate1Size;
extern s32 savestate2Size;
extern volatile s32 isSaveOrLoadActive;
extern s32 stateCooldown;
extern s32 gameBootTimer;
extern u8 gLevelClearBitfeild;
extern u8 D_80200B68;
extern s32 textCyanColor[];
extern s32 textGreenColor[];
extern s32 textGreenMatColor[];
extern s32 textWhiteColor[];
extern s32 textRedColor[];

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
extern s32 D_801749AC;
void AddCarrot(s32);
void func_80078294(f32, f32, s32, f32, s32);
s32 isntNegative(s32);
//static SaveFile gSaveFile;
extern SaveFile gSaveFile; //should be static in matching code, but we dont want to declare this again
s32 SaveData_ReadFile(SaveFile*);
void func_8002CE54(void);
void func_800546F0(void);
void func_8008800C(s32);
s32 func_8008BE14(void);
void func_8008FD68(void);
void func_8008FE00(void);
void func_8008FEA8(s32, u8);
void func_800B4574(u8*, s16*);
void func_800C1458(s32);
void func_800C1510(u8, u8);
void func_800C2820(u8, PlayerActor*, SaveFile*);

char* ParseIntToBase10(s32, s32*);                         /* extern */
extern char D_8010D8E8[];
extern char D_8010D8F8[];
extern char D_8010D8FC[];
extern s32 currentStageCrowns;
extern s32 D_80247904;
extern s16 D_800FFEBC;
extern s16 D_80200B38;
extern s32 D_80236978;
extern char D_8010DB1C[];
extern s32 D_80174980;

#define ALIGN4(val) (((val) + 0x3) & ~0x3)
#define ARRAY_COUNT(arr) (s32)(sizeof(arr) / sizeof(arr[0]))
#define ramStartAddr (u32)0x800EE1C0
#define ramEndAddr (u32)0x803B5000
#define KB *(1 <<10)
#define MB *(1 <<20)
#define GB *(1U<<30)
#define ramAddrSavestateDataSlot1 (void*)0x804C0000
#define ramAddrSavestateDataSlot2 (void*)0x805D0000

#endif // _MOD_MAIN_H_
