#ifndef _COMPAT_H
#define _COMPAT_H


#ifdef __clang__
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifdef __MINGW64__
#define min(a,b)			((a)<(b)?(a):(b))
#endif


#endif