#include "common.h"
#include "../include/mod_main.h"
#include "../lib/cart.h"
#include "../lib/ff.h"
#include "../lib/ffconf.h"

FATFS FatFs;
FIL sdsavefile;

void loadEnemyObjectsHook(void);
void crash_screen_init(void);

s32 status = FR_OK;
s32 sdCardWrite = -1;
FRESULT fileres = -1;

const char* fresult_strings[] = {
    "Succeeded",
    "A hard error occurred in the low level disk I/O layer",
    "Assertion failed",
    "The physical drive cannot work",
    "Could not find the file",
    "Could not find the path",
    "The path name format is invalid",
    "Access denied due to prohibited access or directory full",
    "Access denied due to prohibited access",
    "The file/directory object is invalid",
    "The physical drive is write protected",
    "The logical drive number is invalid",
    "The volume has no work area",
    "There is no valid FAT volume",
    "The f_mkfs() aborted due to any problem",
    "Could not get a grant to access the volume within defined period",
    "The operation is rejected according to the file sharing policy",
    "LFN working buffer could not be allocated",
    "Number of open files > FF_FS_LOCK",
    "Given parameter is invalid"
};

char testString[] = "Test String";

FRESULT initFatFs(void)
{
	//Mount SD Card
	return f_mount(&FatFs, "", 0);
}

// s8 sdsaveinit(void) {
//     return fileres = f_open(&sdsavefile, path, FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
// }
s32 val = -1;
void ReadFileFromSDCard(void* buffer, s32 size, char* filePath) {
    UINT filebytesread;

    fileres = f_open(&sdsavefile, filePath, FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
    status = f_read(&sdsavefile, buffer, ALIGN4(size), &filebytesread);
    if (status != FR_OK){
        osSyncPrintf("f_write failed\n");
        osSyncPrintf("f_write = %d\n",status);
        return;
    }
    f_close(&sdsavefile);
}

void WriteFileToSDCard(void* buffer, s32 size, char* filePath) {
    UINT filebytesread;

    fileres = f_open(&sdsavefile, filePath, FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
    status = f_write(&sdsavefile, buffer, ALIGN4(size), &filebytesread);
    if (status != FR_OK){
        osSyncPrintf("f_write failed\n");
        osSyncPrintf("f_write = %d\n",status);
        return;
    }
    f_close(&sdsavefile);
}

typedef struct CustomThread2 {
    /* 0x000 */ OSThread thread;
    /* 0x1B0 */ char stack[0x800];
    /* 0x9B0 */ OSMesgQueue queue;
    /* 0x9C8 */ OSMesg mesg;
    /* 0x9CC */ u16* frameBuf;
    /* 0x9D0 */ u16 width;
    /* 0x9D2 */ u16 height;
} CustomThread2; // size = 0x9D4
CustomThread2 gCustomThread2;

void InitializeSDCard(void) {
    cart_init();
    initFatFs();
}