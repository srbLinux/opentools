#include "vector.h"

#include "../defines.h"

OTS_Vector *OTS_Vector_Initialize(int initSize) {
    OTS_Vector *vec = (OTS_Vector *)malloc(sizeof(OTS_Vector));
    if (!vec) {
        OTS_err_info("%s: Failed to require new memory.\n", __func__);
        return NULL;
    }
    vec->capacity = initSize; vec->size = 0;
    vec->data = (void **)malloc(sizeof(void *)*vec->capacity);
    return vec;
}