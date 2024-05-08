#include "platform.h"

#if __linux__
#include <unistd.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#if __linux__
const char *OTS_System_GetCurrentPath() {
    char *path=malloc(sizeof(char)*256);
    getcwd(path, 256);
    return path;
}
#elif _WIN32
const char *OTS_System_GetCurrentPath() {

}
#endif

char **OTS_split(const char *str, const char *delimiter, int *filelen) {
    // 计算分隔符的个数，即子串的个数加1
    int numTokens = 1;
    for (const char *ptr = str; *ptr != '\0'; ptr++) {
        if (strstr(ptr, delimiter) == ptr) {
            numTokens++;
            ptr += strlen(delimiter) - 1;
        }
    }
    
    // 分配内存以保存分割后的字符串数组
    char **tokens = (char **)malloc(numTokens * sizeof(char *));
    if (tokens == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // 初始化分割后的字符串数组长度
    *filelen = numTokens;

    // 分割字符串并保存到数组中
    int tokenIndex = 0;
    const char *start = str;
    for (const char *ptr = str; *ptr != '\0'; ptr++) {
        if (strstr(ptr, delimiter) == ptr) {
            int tokenLen = ptr - start;
            tokens[tokenIndex] = (char *)malloc((tokenLen + 1) * sizeof(char));
            if (tokens[tokenIndex] == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                exit(EXIT_FAILURE);
            }
            strncpy(tokens[tokenIndex], start, tokenLen);
            tokens[tokenIndex][tokenLen] = '\0';
            tokenIndex++;
            start = ptr + strlen(delimiter);
            ptr += strlen(delimiter) - 1;
        }
    }

    // 处理最后一个子串
    int tokenLen = str + strlen(str) - start;
    tokens[tokenIndex] = (char *)malloc((tokenLen + 1) * sizeof(char));
    if (tokens[tokenIndex] == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    strncpy(tokens[tokenIndex], start, tokenLen);
    tokens[tokenIndex][tokenLen] = '\0';

    return tokens;
}