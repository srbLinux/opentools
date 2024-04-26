#include "vector.h"

#include <stdlib.h>

#include "../defines.h"
#include "./debug.h"

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

void *OTS_Vector_Erase(OTS_Vector *vec, int index) {
    if (!vec) {
        OTS_ERROR("OTS_Vector object is null.\n");
        return NULL;
    }
    if (index < 0 || index >= vec->size) {
        OTS_ERROR("Index is out of range.\n");
        return NULL;
    }
    void *ptr=NULL;
    if (vec->data[index] != NULL) {
        ptr = vec->data[index];
        vec->data[index] = NULL;
    }
    for (int i=index;i<vec->size-1;++i) {
        vec->data[i] = vec->data[i+1];
    }
    vec->size -- ;
    return ptr;
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

int OTS_Vector_Size(OTS_Vector *vec) {
    return vec->size;
}

int OTS_Vector_Find(OTS_Vector *vec, void *data) {
    for (int i=0;i<OTS_Vector_Size(vec);i++) {
        if (OTS_Vector_AT(vec, i)==data) {
            return i;
        }
    }
    return -1;
}

void OTS_Vector_Free(OTS_Vector *vec, int deep) {
    if (deep) {
        for (int i=0;i<vec->size;i++) {
            free(vec->data[i]);
        }
    }
    free(vec->data); free(vec);
}