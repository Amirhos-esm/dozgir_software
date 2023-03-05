#ifndef DEBUG_DEBUG_H_
#define DEBUG_DEBUG_H_

#include "stdio.h"
#include "mainCodes.h"

#ifdef __cplusplus
extern "C"{
#endif

#define DBG_ENABLE

#if defined(DBG_ENABLE)
#define log(...) {swap(1);printf( __VA_ARGS__);};
#define logln(...) {swap(1);printf( __VA_ARGS__);printf("\n");}
#else

#define log(...)
#define logln(...)

#endif


#define DBG 1

void swap(uint8_t);


#ifdef __cplusplus
}
#endif

#endif
