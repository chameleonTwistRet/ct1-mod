// Author: Ellie (Elisiah)
// Text.c: Handle String Data for Chameleon Twist 

#include "text.h"

// const char EUCJP[26] = {
//     'Ａ', 'Ｂ', 'Ｃ', 'Ｄ', 'Ｅ', 'Ｆ', 'Ｇ', 'Ｈ', 'Ｉ', 'Ｊ', 'Ｋ', 'Ｌ', 'Ｍ', 'Ｎ', 'Ｏ', 'Ｐ', 'Ｑ',
//     'Ｒ', 'Ｓ', 'Ｔ', 'Ｕ', 'Ｖ', 'Ｗ', 'Ｘ', 'Ｙ', 'Ｚ'
// };

extern void* crash_screen_copy_to_buf(void* dest, const char* src, u32 size);

void _sprintf(void* destination, void* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    _Printf((void*)crash_screen_copy_to_buf, destination, fmt, args);
    va_end(args);
}

void _sprintfcat(void* destination, void* fmt, ...) {
    s32 existing_length;
    char* cat_destination;

    va_list args;
    va_start(args, fmt);
    existing_length = strlen((char*)destination);
    cat_destination = (char*)destination + existing_length;                 // set to the end of the existing string
    _Printf((void*)crash_screen_copy_to_buf, cat_destination, fmt, args);
    va_end(args);
}

char* strcat(char* dest, const char* src) {
    char* ptr = dest + strlen(dest);
    while (*src != '\0') {
        *ptr++ = *src++;
    }
    *ptr = '\0';
    return dest;
}

char* strncpy_custom(char* dest, const char* src, s32 n) {
    s32 i;
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    for (; i < n; i++) {
        dest[i] = '\0';
    }
    return dest;
}

s32 strlength_custom(const char* str) {
    s32 len = 0;
    while (*str++ != '\0') {
        len++;
    }
    return len;
}

void sprintf_custom(char* buffer, char c) {
    buffer[0] = c;
    buffer[1] = '\0';
}

void convertAsciiToText(void* buffer, char* source) {
    u16* buf = (u16*)buffer;
    s32 strlength = strlength_custom(source);
    s32 i;

    for (i = 0; i < strlength; i++) {
        if ( (source[i] >= '0' && source[i] <= '9') ||
            (source[i] >= 'A' && source[i] <= 'Z')) { //is 0 - 9 or A - Z
            buf[i] = source[i] + 0xA380; //0x30 = 0 in ascii, 0xA3B0 = 0 in chameleon text
        } else if ( (source[i] > '0' && source[i] <= '9') ||
            (source[i] >= 'a' && source[i] <= 'z')) { //is 0 - 9 or A - Z
            buf[i] = source[i] + 0xA360; //0x30 = 0 in ascii, 0xA3B0 = 0 in chameleon text
        } else if(source[i] == '-') {
            buf[i] = 0xA1DD; // '-' in chameleon text
        } else if (source[i] == '.') {
            buf[i] = 0xA1A5; // '.' in chameleon text
        } else if (source[i] == ':') {
            buf[i] = 0xA1A7; // ':' in chameleon text
        } else if (source[i] == ' ') {
            buf[i] = 0xA1A1; // ' ' in chameleon text
        }
    }
    buf[i] = '\0'; //terminate buffer
}