#ifndef _MENU_H_
#define _MENU_H_

#include "mod_main.h"
#include "text.h"

enum Pages {
    PAGE_MAIN = 0,
    PAGE_JL = 1
};

#define X_COORD_PER_LETTER 4.5
#define FUNCS_PER_PAGE 8
#define FUNCS_ON_LAST_PAGE 3

typedef struct menuPage {
    /* 0x08 */ s32 optionCount;
    /* 0x0C */ s32 pageIndex;
    /* 0x10 */ char* options[FUNCS_PER_PAGE];
    /* 0x30 */ s32 (*menuProc[FUNCS_PER_PAGE]) (void);
    /* 0x50 */ u8 flags[FUNCS_PER_PAGE];
} menuPage;


extern void _sprintf(void* destination, void* fmt, ...);
extern void convertAsciiToText(void* buffer, char* source);

void pageMainDisplay(s32 currPageNo, s32 currOptionNo);
s32 menuDisplay(void);
s32 menuProcFunc(void);
s32 teleportToStageBoss(void);
void colorTextWrapper(s32* color);

#endif