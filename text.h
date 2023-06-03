#ifndef _TEXT_H_
#define _TEXT_H_

#include "mod_main.h"

char* strncpy_custom(char* dest, const char* src, s32 n);
s32 strlength_custom(const char* str);
void sprintf_custom(char* buffer, char c);
void convertAsciiToText(void* buffer, char* source);

#endif
