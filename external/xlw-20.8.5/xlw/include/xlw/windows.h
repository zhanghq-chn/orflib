#include <stdint.h>

typedef void VOID;
typedef __INT32_TYPE__ INT32;
typedef __WCHAR_TYPE__ WCHAR;
typedef int32_t LONG;
typedef unsigned long DWORD_PTR;
typedef unsigned short WORD;
typedef unsigned char BYTE;
typedef unsigned long DWORD;
typedef void* HANDLE;
typedef char* LPSTR;
typedef struct HWND__ { int unused; } *HWND;
typedef struct HINSTANCE__ { int unused; } *HINSTANCE;
typedef struct tagPOINT {
    LONG x;
    LONG y;
} POINT, *PPOINT;

#if defined(__clang__)
#define _cdecl __attribute__((cdecl))
#define pascal __attribute__((stdcall))
#define EXCEL_EXPORT __attribute__((visibility("default")))
#endif
