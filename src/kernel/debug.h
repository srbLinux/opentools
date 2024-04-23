#ifndef _OPENTOOLS_DEBUG_
#define _OPENTOOLS_DEBUG_


extern char *OTS_Debug_FormatTimeNow();

#define MAX_DEBUG_MSG_LEN 2048

#if __DEBUG_OTS__

#define OTS_DEBUG(str, ...) \
    do {    \
        char *__ots_debug_format__ = (char *)malloc(sizeof(char)*MAX_DEBUG_MSG_LEN); \
        sprintf(__ots_debug_format__, "%s - [INFO] - [%s:%d] - %s: %s", OTS_Debug_FormatTimeNow(), __FILE__, __LINE__, __func__, str); \
        fprintf(stdout, __ots_debug_format__, ##__VA_ARGS__); \
        free(__ots_debug_format__);     \
    } while (0)

#else 

#define OTS_DEBUG(str, ...) \
    do {    \
        char *__ots_debug_format__ = (char *)malloc(sizeof(char)*MAX_DEBUG_MSG_LEN);    \
        sprintf(__ots_debug_format__, "%s - [%s:%d] - %s: %s", OTS_Debug_FormatTimeNow(), __FILE__, __LINE__, __func__, str); \
        fprintf(logFILE, __ots_debug_format__, ##__VA_ARGS__); \
        free(__ots_debug_format__);     \
    } while(0)

#endif

#define OTS_ERROR(str, ...) \
    do { \
        char __ots_error_format__[MAX_DEBUG_MSG_LEN] = {0}; \
        sprintf(__ots_error_format__, "%s - [ERROR] - [%s:%d] - %s: %s", OTS_Debug_FormatTimeNow(), __FILE__, __LINE__, __func__, str); \
        fprintf(stdout, __ots_error_format__, ##__VA_ARGS__); \
        fprintf(logFILE, __ots_error_format__, ##__VA_ARGS__); \
    } while (0)


#endif