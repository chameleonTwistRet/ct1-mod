#ifndef _MOD_MAIN_H_
#define _MOD_MAIN_H_

#include "common.h"
#include "text.h"

extern s32 gameModeCurrent;
extern s32 sGameModeStart;
extern OSMesgQueue D_801192E8;
extern char D_8010DB20[];
extern Addr* mod_ROM_START;
extern Addr* mod_VRAM;
extern Addr* mod_ROM_END;
extern Addr* mod_ROM_START;
void mod_main_per_frame(void);
void mod_boot_func(void);

#endif // _MOD_MAIN_H_
