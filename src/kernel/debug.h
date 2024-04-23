#ifndef _OPENTOOLS_DEBUG_
#define _OPENTOOLS_DEBUG_


enum OTS_TcpUploadState {
    UPLOAD_OK, UPLOAD_FAILED_NETWORK_ERROR,
};

extern char *OTS_Debug_FormatTimeNow();
extern enum OTS_TcpUploadState OTS_Debug_UploadFile(const char *upload_url, const char *filename);
extern enum OTS_TcpUploadState OTS_Debug_UploadTime(const char *upload_url, size_t begin_timestamp, size_t end_timestamp);

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