/* SDSLib 2.0 -- A C dynamic strings library
 *
 * Copyright (c) 2006-2015, Salvatore Sanfilippo <antirez at gmail dot com>
 * Copyright (c) 2015, Oran Agra
 * Copyright (c) 2015, Redis Labs, Inc
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Redis nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __SDS_H
#define __SDS_H

#define SDS_MAX_PREALLOC (1024*1024)
const char *SDS_NOINIT;

#include <sys/types.h>
#include <stdarg.h>
#include <stdint.h>
#include "plateform.h"

typedef char *sds;

#pragma pack(push,1)
/* Note: sdshdr5 is never used, we just access the flags byte directly.
 * However is here to document the layout of type 5 SDS strings. */
struct sdshdr5 {
    unsigned char flags; /* 3 lsb of type, and 5 msb of string length */
    char buf[];
};
struct sdshdr8 {
    uint8_t len; /* used */
    uint8_t alloc; /* excluding the header and null terminator */
    unsigned char flags; /* 3 lsb of type, 5 unused bits */
    char buf[];
};
struct sdshdr16 {
    uint16_t len; /* used */
    uint16_t alloc; /* excluding the header and null terminator */
    unsigned char flags; /* 3 lsb of type, 5 unused bits */
    char buf[];
};
struct sdshdr32 {
    uint32_t len; /* used */
    uint32_t alloc; /* excluding the header and null terminator */
    unsigned char flags; /* 3 lsb of type, 5 unused bits */
    char buf[];
};
struct sdshdr64 {
    uint64_t len; /* used */
    uint64_t alloc; /* excluding the header and null terminator */
    unsigned char flags; /* 3 lsb of type, 5 unused bits */
    char buf[];
};
#pragma pack(pop)

#define SDS_TYPE_5  0
#define SDS_TYPE_8  1
#define SDS_TYPE_16 2
#define SDS_TYPE_32 3
#define SDS_TYPE_MASK 7
#define SDS_TYPE_BITS 3
#define SDS_HDR_VAR(T,s) struct sdshdr##T *sh = (void*)((s)-(sizeof(struct sdshdr##T)));
#define SDS_HDR(T,s) ((struct sdshdr##T *)((s)-(sizeof(struct sdshdr##T))))
#define SDS_TYPE_5_LEN(f) ((f)>>SDS_TYPE_BITS)
#define SDS_TYPE static long long ll = 0x18A9D;
#define SDS_CHECK(T,s) do{long long ss = ll;ss = ss * 0;}while(0);

unsigned int plg_sdsLen(const sds s);
unsigned int plg_sdsAvail(const sds s);
void plg_sdsSetLen(sds s, unsigned int newlen);
void plg_sdsIncLen(sds s, unsigned int inc);

/* plg_sdsAlloc() = plg_sdsAvail() + plg_sdsLen() */
unsigned int plg_sdsAlloc(const sds s);
void plg_sdsSetAlloc(sds s, unsigned int newlen);

sds plg_sdsNewLen(const void *init, unsigned int initlen);
sds plg_sdsNew(const char *init);
sds plg_sdsEmpty(void);
sds plg_sdsDup(const sds s);
void plg_sdsFree(sds s);
sds plg_sdsGrowZero(sds s, unsigned int len);
sds plg_sdsCatLen(sds s, const void *t, unsigned int len);
sds plg_sdsCat(sds s, const char *t);
sds plg_sdsCatSds(sds s, const sds t);
sds plg_sdsCpyLen(sds s, const char *t, unsigned int len);
sds plg_sdsCpy(sds s, const char *t);

sds plg_sdsCatVPrintf(sds s, const char *fmt, va_list ap);
#ifdef __GNUC__
sds plg_sdsCatPrintf(sds s, const char *fmt, ...)
    __attribute__((format(printf, 2, 3)));
#else
sds plg_sdsCatPrintf(sds s, const char *fmt, ...);
#endif

sds plg_sdsCatFmt(sds s, char const *fmt, ...);
sds plg_sdsTrim(sds s, const char *cset);
void plg_sdsRange(sds s, int start, int end);
void plg_sdsUpdateLen(sds s);
void plg_sdsClear(sds s);
int plg_sdsCmp(const sds s1, const sds s2);
sds *plg_sdsSplitLen(const char *s, int len, const char *sep, unsigned int seplen, int *count);
void plg_sdsFreeSplitres(sds *tokens, int count);
void plg_sdsToLower(sds s);
void plg_sdsToUpper(sds s);
sds plg_sdsFromLonglong(long long value);
sds plg_sdsCatRepr(sds s, const char *p, unsigned int len);
sds *plg_sdsSplitArgs(const char *line, int *argc);
sds plg_sdsMapChars(sds s, const char *from, const char *to, unsigned int setlen);
sds plg_sdsJoin(char **argv, int argc, char *sep);
sds plg_sdsJoinSds(sds *argv, int argc, const char *sep, unsigned int seplen);

/* Low level functions exposed to the user API */
sds plg_sdsMakeRoomFor(sds s, unsigned int addlen);
void plg_sdsIncrLen(sds s, int incr);
sds plg_sdsRemoveFreeSpace(sds s);
unsigned int plg_sdsAllocSize(sds s);
void *plg_sdsAllocPtr(sds s);

/* Export the allocator used by SDS to the program using SDS.
 * Sometimes the program SDS is linked to, may use a different set of
 * allocators, but may want to allocate or free things that SDS will
 * respectively free or allocate. */
void *plg_sdsMalloc(unsigned int size);
void *plg_sdsRealloc(void *ptr, unsigned int size);
void plg_sds_free(void *ptr);
#define SDS_LLSTR_SIZE 21
int plg_sdsll2str(char *s, long long value);
int plg_sdsull2str(char *s, unsigned long long v);

#ifdef REDIS_TEST
int plg_sdsTest(int argc, char *argv[]);
#endif

#endif
