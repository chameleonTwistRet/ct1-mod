#include "common.h"
#include "os.h"
#include "xstdio.h"

OSPiHandle __Dom1SpeedParam;
OSPiHandle __Dom2SpeedParam;

OSPiHandle *__osCurrentHandle[2] = {&__Dom1SpeedParam, &__Dom2SpeedParam};

#define UNUSED      __attribute__((unused))
#define FALLTHROUGH __attribute__((fallthrough))
#define NORETURN    __attribute__((noreturn))

void* is_proutSyncPrintf(void* arg, const char* str, u32 count);

OSPiHandle* sISVHandle; // official name : is_Handle

typedef struct {
    /* 0x00 */ u32 magic; // IS64
    /* 0x04 */ u32 get;
    /* 0x08 */ u8 unk_08[0x14-0x08];
    /* 0x14 */ u32 put;
    /* 0x18 */ u8 unk_18[0x20-0x18];
    /* 0x20 */ u8 data[0x10000-0x20];
} ISVDbg;

extern OSPiHandle __CartRomHandle;

OSPiHandle* osCartRomInit(void) {
    static u32 first = TRUE;
    register u32 value;
    register s32 status;
    register u32 prevInt;
    register u32 latency;
    register u32 pageSize;
    register u32 relDuration;
    register u32 pulse;

    __osPiGetAccess();

    if (!first) {
        __osPiRelAccess();
        return &__CartRomHandle;
    }

    first = FALSE;
    __CartRomHandle.type = DEVICE_TYPE_CART;
    __CartRomHandle.baseAddress = PHYS_TO_K1(PI_DOM1_ADDR2);
    __CartRomHandle.domain = PI_DOMAIN1;
    __CartRomHandle.speed = 0;
    _bzero(&__CartRomHandle.transferInfo, sizeof(__OSTranxInfo));

    status = IO_READ(PI_STATUS_REG);
    while (status & (PI_STATUS_DMA_BUSY | PI_STATUS_IO_BUSY)) {
        status = IO_READ(PI_STATUS_REG);
    }

    latency = IO_READ(PI_BSD_DOM1_LAT_REG);
    pageSize = IO_READ(PI_BSD_DOM1_PGS_REG);
    relDuration = IO_READ(PI_BSD_DOM1_RLS_REG);
    pulse = IO_READ(PI_BSD_DOM1_PWD_REG);

    IO_WRITE(PI_BSD_DOM1_LAT_REG, 255);
    IO_WRITE(PI_BSD_DOM1_PGS_REG, 0);
    IO_WRITE(PI_BSD_DOM1_RLS_REG, 3);
    IO_WRITE(PI_BSD_DOM1_PWD_REG, 255);

    value = IO_READ(__CartRomHandle.baseAddress);
    __CartRomHandle.latency = value & 0xFF;
    __CartRomHandle.pageSize = (value >> 0x10) & 0xF;
    __CartRomHandle.relDuration = (value >> 0x14) & 0xF;
    __CartRomHandle.pulse = (value >> 8) & 0xFF;

    IO_WRITE(PI_BSD_DOM1_LAT_REG, latency);
    IO_WRITE(PI_BSD_DOM1_PGS_REG, pageSize);
    IO_WRITE(PI_BSD_DOM1_RLS_REG, relDuration);
    IO_WRITE(PI_BSD_DOM1_PWD_REG, pulse);

    prevInt = __osDisableInt();
    __CartRomHandle.next = __osPiTable;
    __osPiTable = &__CartRomHandle;
    __osRestoreInt(prevInt);
    __osPiRelAccess();

    return &__CartRomHandle;
}

s32 __osEPiRawReadIo(OSPiHandle* handle, u32 devAddr, u32* data) {
    s32 status;
    OSPiHandle* curHandle;

    status = IO_READ(PI_STATUS_REG);
    while (status & (PI_STATUS_DMA_BUSY | PI_STATUS_IO_BUSY)) {
        status = IO_READ(PI_STATUS_REG);
    }

    if (__osCurrentHandle[handle->domain]->type != handle->type) {
        curHandle = __osCurrentHandle[handle->domain];

        if (handle->domain == 0) {
            if (curHandle->latency != handle->latency) {
                IO_WRITE(PI_BSD_DOM1_LAT_REG, handle->latency);
            }

            if (curHandle->pageSize != handle->pageSize) {
                IO_WRITE(PI_BSD_DOM1_PGS_REG, handle->pageSize);
            }

            if (curHandle->relDuration != handle->relDuration) {
                IO_WRITE(PI_BSD_DOM1_RLS_REG, handle->relDuration);
            }

            if (curHandle->pulse != handle->pulse) {
                IO_WRITE(PI_BSD_DOM1_PWD_REG, handle->pulse);
            }
        } else {
            if (curHandle->latency != handle->latency) {
                IO_WRITE(PI_BSD_DOM2_LAT_REG, handle->latency);
            }

            if (curHandle->pageSize != handle->pageSize) {
                IO_WRITE(PI_BSD_DOM2_PGS_REG, handle->pageSize);
            }

            if (curHandle->relDuration != handle->relDuration) {
                IO_WRITE(PI_BSD_DOM2_RLS_REG, handle->relDuration);
            }

            if (curHandle->pulse != handle->pulse) {
                IO_WRITE(PI_BSD_DOM2_PWD_REG, handle->pulse);
            }
        }

        curHandle->type = handle->type;
        curHandle->latency = handle->latency;
        curHandle->pageSize = handle->pageSize;
        curHandle->relDuration = handle->relDuration;
        curHandle->pulse = handle->pulse;
    }

    *data = IO_READ(handle->baseAddress | devAddr);
    return 0;
}

s32 osEPiReadIo(OSPiHandle* handle, u32 devAddr, u32* data) {
    register s32 ret;

    __osPiGetAccess();
    ret = __osEPiRawReadIo(handle, devAddr, data);
    __osPiRelAccess();

    return ret;
}

s32 osEPiWriteIo(OSPiHandle* handle, u32 devAddr, u32 data) {
    register s32 ret;

    __osPiGetAccess();
    ret = __osEPiRawWriteIo(handle, devAddr, data);
    __osPiRelAccess();

    return ret;
}

#define gISVDbgPrnAdrs ((ISVDbg*)0xB3FF0000)
#define ASCII_TO_U32(a, b, c, d) ((u32)((a << 24) | (b << 16) | (c << 8) | (d << 0)))

void isPrintfInit(void) {
    sISVHandle = osCartRomInit();
    osEPiWriteIo(sISVHandle, (u32)&gISVDbgPrnAdrs->put, 0);
    osEPiWriteIo(sISVHandle, (u32)&gISVDbgPrnAdrs->get, 0);
    osEPiWriteIo(sISVHandle, (u32)&gISVDbgPrnAdrs->magic, ASCII_TO_U32('I', 'S', '6', '4'));
}

void osSyncPrintfUnused(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    _Printf(is_proutSyncPrintf, NULL, fmt, args);

    va_end(args);
}

void osSyncPrintfCopy(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    _Printf(is_proutSyncPrintf, NULL, fmt, args);

    va_end(args);
}

// assumption
void rmonPrintf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    _Printf(is_proutSyncPrintf, NULL, fmt, args);

    va_end(args);
}

void* is_proutSyncPrintf(void* arg, const char* str, u32 count) {
    u32 data;
    s32 pos;
    s32 start;
    s32 end;

    osEPiReadIo(sISVHandle, (u32)&gISVDbgPrnAdrs->magic, &data);
    if (data != ASCII_TO_U32('I', 'S', '6', '4')) {
        return (void*)1;
    }
    osEPiReadIo(sISVHandle, (u32)&gISVDbgPrnAdrs->get, &data);
    pos = data;
    osEPiReadIo(sISVHandle, (u32)&gISVDbgPrnAdrs->put, &data);
    start = data;
    end = start + count;
    if (end >= 0xFFE0) {
        end -= 0xFFE0;
        if (pos < end || start < pos) {
            return (void*)1;
        }
    } else {
        if (start < pos && pos < end) {
            return (void*)1;
        }
    }
    while (count) {
        u32 addr = (u32)&gISVDbgPrnAdrs->data + (start & 0xFFFFFFC);
        s32 shift = ((3 - (start & 3)) * 8);

        if (*str) {
            osEPiReadIo(sISVHandle, addr, &data);
            osEPiWriteIo(sISVHandle, addr, (*str << shift) | (data & ~(0xFF << shift)));

            start++;
            if (start >= 0xFFE0) {
                start -= 0xFFE0;
            }
        }
        count--;
        str++;
    }
    osEPiWriteIo(sISVHandle, (u32)&gISVDbgPrnAdrs->put, start);

    return (void*)1;
}

void func_80002384(const char* exp, const char* file, u32 line) {
    osSyncPrintfCopy("File:%s Line:%d  %s \n", file, line, exp);
    while (1) {
        ;
    }
}
