#include "debug.h"

#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "../defines.h"

char *OTS_Debug_FormatTimeNow() {
    char *strtime = (char *)malloc(sizeof(char)*32);
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(strtime, 32, "[%Y-%m-%d %H:%M:%S]", timeinfo);
    return strtime;
}

// 辅助函数，用于格式化输出调试消息
void OTS_Debug_Print(const char *format, ...) {
    va_list args; // 定义可变参数列表
    va_start(args, format); // 初始化可变参数列表
    // 输出源文件名、行号和函数名
    fprintf(stdout, "%s - [%s:%d] %s : ", OTS_Debug_FormatTimeNow(), __FILE__, __LINE__, __func__);
    // 输出格式化后的消息
    vfprintf(stdout, format, args);
    va_end(args); // 结束可变参数列表
}
