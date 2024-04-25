#ifndef _OPENTOOLS_IMAGETOPDFWIDGET_H_
#define _OPENTOOLS_IMAGETOPDFWIDGET_H_

#include <PainterEngine.h>

#include "../defines.h"

struct OTS_ImageToPDFWidgetPrivate;

typedef struct __OTS_ImageToPDFWidget {
    PX_Object *widget;
    struct OTS_ImageToPDFWidgetPrivate *pdata;
} OTS_ImageToPDFWidget;

enum OTS_ConvertToPDFState {
    CONVERT_SUCCESS, ERROR_NOT_SAVEPDF,
    ERROR_IMAGETYPE_NOT_SUPPORT, ERROR_NOT_CREATE_DOCUMENT, ERROR_NOT_ADD_PAGE,
};

extern OTS_ImageToPDFWidget *OTS_ImageToPDFWidget_Initialize(int width, int height);

/**
 * @brief   This is opentools to the outside world to provide the c language API, 
 *          used to convert multiple images into pdf to store down.
 * @param   imagespath: input images path.
 * @param   imageslen: the number of input images.
 * @param   pdfpath: path to the pdf you want to save.
 * @version 1.0.0
*/
extern OTS_EXPORT enum OTS_ConvertToPDFState OTS_ImagesToPDF(const char **imagespath, int imageslen, const char *pdfpath);

#endif