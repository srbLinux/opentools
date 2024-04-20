#include "vector.h"

#include <stdlib.h>

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

int OTS_Vector_Erase(OTS_Vector *vec, int index) {
    if (!vec) {
        OTS_err_info("%s: OTS_Vector object is null\n", __func__);
        return 0;
    }
    if (index < 0 || index > vec->size) {
        OTS_err_info("%s: index outrange.\n", __func__);
        return 0;
    }
    vec->data[index] = NULL;
}

int OTS_Vector_PushAT(OTS_Vector *vec, void *data, int index) {
    if (!vec||!data) {
        OTS_err_info("%s: OTS_Vector object or data is null.\n", __func__);
        return 0;
    }
    if (index < 0 || index > vec->size) {
        OTS_err_info("%s: index outrange.\n", __func__);
        return 0;
    }
    vec->data[index] = data;
}

int OTS_Vector_Pushback(OTS_Vector *vec, void *data) {
    if (!vec||!data) {
        OTS_err_info("%s: OTS_Vector object or data is null.\n", __func__);
        return 0;
    }
    if (vec->size >= vec->capacity) {
        vec->capacity *= 2;
        vec->data = (void **)realloc(vec->data, vec->capacity * sizeof(void *));
        if (!vec->data) {
            OTS_err_info("%s: OTS_Vector realloc memory failed.\n", __func__);
            return 0;
        }
    }
    vec->data[vec->size++] = data; return 1;
}

void *OTS_Vector_AT(OTS_Vector *vec, int index) {
    if (index < 0 || index > vec->size) {
        OTS_err_info("%s: index outrange.\n", __func__);
        return NULL;
    }
    return vec->data[index];
}

void OTS_Vector_Free(OTS_Vector *vec) {
    free(vec->data); free(vec);
}