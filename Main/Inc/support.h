#ifndef __SUPPORT_H
#define __SUPPORT_H

#ifdef __cplusplus
extern "C" {
#endif


typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int s32;
typedef short s16;

typedef enum {
    CFALSE = 0,
    CTRUE,
} CBOOL;

typedef enum {
    FAIL = 0,
    OK,
    BUSY,
    ERR,
} CSTATUS;

/*
 * support
 */
#define CUNUSED(x)                          ((void)x)
#define CNOP                                asm volatile("nop")
#define ERR_INT16                           -32768
#define ERR_INT32                           -2147483648
#define INC_UINT16(x)                       if (x < 0xFFFF) { x++; }
#define INC_UINT32(x)                       if (x < 0xFFFFFFFF) { x++; }
#define FLOAT_MUL                           100.0f
#define MIN_TO_MSEC(x)                      (60000 * x)
#define SEC_TO_MSEC(x)                      (1000 * x)
#define isEQUA_RET_VOID(x, y)               if (x == y) { return; }
#define isNOEQ_RET_VOID(x, y)               if (x != y) { return; }
#define isHIEQ_RET_VOID(x, y)               if (x >= y) { return; }
#define isLOEQ_RET_VOID(x, y)               if (x <= y) { return; }
#define isEQUA_RET_USER(x, y, r)            if (x == y) { return r; }
#define isNOEQ_RET_USER(x, y, r)            if (x != y) { return r; }
#define isHIEQ_RET_USER(x, y, r)            if (x >= y) { return r; }
#define isLOEQ_RET_USER(x, y, r)            if (x <= y) { return r; }
#define isUI16_MAX_CLEAR(x)                 if (x >= 0xFFFF) { x = 0; }
#define isUI32_MAX_CLEAR(x)                 if (x >= 0xFFFFFFFF) { x = 0; }
#define isNONU_FREE(x)                      if (x != NULL) { free(x); x = NULL; }
#define SAVE_VAR(x, y, v)                   { x = CTRUE; y = v; }

void combineBytes(u8 *byte, u32 *combined);
void splitBytes(u32 combined, u8 *byte);

#ifdef __cplusplus
}
#endif

#endif