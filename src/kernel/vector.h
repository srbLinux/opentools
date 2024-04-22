#ifndef _OPENTOOLS_VECTOR_H_
#define _OPENTOOLS_VECTOR_H_

/**
 * 
 * @struct  OTS_Vector
 * @version since 1.0.0
 * @brief   This is a dynamic array that does not copy data. The elements in the vector merely point to the actual data, 
 *          rather than containing the data itself. The main purpose of this approach is to allow access to the original
 *          data directly. This approach is suitable for scenarios where frequent access and updating of data are required,
 *          especially in cases of large data volumes, as it reduces memory overhead and improves program performance.
 * 
*/
typedef struct __OTS_Vector {
    void **data;
    int size, capacity;
} OTS_Vector;

extern OTS_Vector *OTS_Vector_Initialize(int initSize);
extern void *OTS_Vector_AT(OTS_Vector *vec, int index);
extern int OTS_Vector_Erase(OTS_Vector *vec, int index);
extern int OTS_Vector_Pushback(OTS_Vector *vec, void *data);
extern int OTS_Vector_PushAT(OTS_Vector *vec, void *data, int index);
extern int OTS_Vector_Size(OTS_Vector *vec);
extern void OTS_Vector_Free(OTS_Vector *vec);

#endif