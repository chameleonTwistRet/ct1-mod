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
