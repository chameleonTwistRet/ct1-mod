#ifndef _MENU_H_
#define _MENU_H_

#include "common.h"

#define X_COORD_PER_LETTER 4.5
#define FUNCS_PER_PAGE 7

s32 toggleHideSavestateText(void);
void textPrint(f32 xPos, f32 yPos, f32 scale, void *text, s32 num);
void colorTextWrapper(s32* color);
extern s8 toggles[];

typedef struct menuPage {
    /* 0x00 */ s32 optionCount;
    /* 0x04 */ s32 pageIndex;
    /* 0x08 */ char* options[FUNCS_PER_PAGE];
    /* 0x28 */ s32 (*menuProc[FUNCS_PER_PAGE]) (void);
    /* 0x48 */ s8 flags[FUNCS_PER_PAGE];
    /* 0x50 */ char*** selectionText;
} menuPage;

extern menuPage page0;
extern menuPage page1;
extern menuPage page2;
#endif //_MENU_H_
