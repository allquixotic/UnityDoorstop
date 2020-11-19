/*
 * Custom implementation for common C runtime functions
 * This makes the DLL essentially freestanding on Windows without having to rely
 * on msvcrt.dll
 */
#ifndef CRT_H
#define CRT_H

#include "../util.h"
#include <windows.h>

HANDLE h_heap;

#define STR_LEN(str) (sizeof(str) / sizeof((str)[0]))

extern void *memset(void *dst, int c, size_t n);
#pragma intrinsic(memset)

extern void *memcpy(void *dst, const void *src, size_t n);
#pragma intrinsic(memcpy)

extern size_t wcslen(wchar_t const *str);
#pragma intrinsic(wcslen)

extern size_t strlen(char const *str);
#pragma intrinsic(strlen)

extern void *malloc(size_t size);

extern void *calloc(size_t num, size_t size);

extern wchar_t *wmemcpy(wchar_t *dst, const wchar_t *src, size_t n);

extern void *wmemset(wchar_t *dst, wchar_t c, size_t n);

inline void free(void *mem) { HeapFree(h_heap, 0, mem); }

inline int setenv(const char_t *name, const char_t *value, int overwrite) {
    return !SetEnvironmentVariable(name, value);
}

inline char_t *getenv(const char_t *name) {
    DWORD size = GetEnvironmentVariable(name, NULL, 0);
    if (size == 0)
        return NULL;
    char_t *buf = calloc(size + 1, sizeof(char_t));
    GetEnvironmentVariable(name, buf, size + 1);
    return buf;
}

#ifndef UNICODE
#define CommandLineToArgv CommandLineToArgvA
extern char **CommandLineToArgvA(char *cmd_line, int *argc);
#else
#define CommandLineToArgv CommandLineToArgvA
#endif

#endif