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