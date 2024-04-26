#include "imagetopdfwidget.h"

#include "../defines.h"
#include "../global.h"

#include "../kernel/font.h"
#include "../kernel/debug.h"

#include "selectfileswidget.h"

#include <hpdf.h>

struct OTS_ImageToPDFWidgetPrivate {
    PX_Object *selectBar, *passwdLine, *parent;
    OTS_Vector *selectItems;
};

typedef struct OTS_ImageToPDFWidgetPrivate OTS_ITPWPrivate;

static const char *getImageType(const char *imagepath);
static void addFilesCallback(void *usrptr, void *data);
static void deleteFileCallback(PX_Object *obj, PX_Object_Event event, void *data);
static void selectFilesCallback(PX_Object *obj, PX_Object_Event event, void *data);

OTS_ImageToPDFWidget *OTS_ImageToPDFWidget_Initialize(int width, int height) {
    OTS_ImageToPDFWidget *pdfWidget = (OTS_ImageToPDFWidget *)MP_Malloc(mp, sizeof(OTS_ImageToPDFWidget));
    pdfWidget->widget = OTS_DEFUALT_WIDGET_CREATE;
    pdfWidget->pdata = (OTS_ITPWPrivate *)malloc(sizeof(OTS_ITPWPrivate));
    const char *filter = "*.png;*.jpg;*.svg;*.jpeg;*.jpe;";
    OTS_SelectFilesWidget *selectImagesWidget = OTS_SelectFilesWidget_Initialize(width, height, "Select image files", filter);
    PX_Object_WidgetHide(selectImagesWidget->widget);
    char *labelText = (char *)malloc(sizeof(char) * 1024);
    int labelWidth = 0, labelHeight = 0, inputWidth = 0, inputHeight = 0;
    strcpy(labelText, "Input Images: ");
    labelWidth = OTS_FontTextWidth(gs->textFont, labelText);
    labelHeight = OTS_FontTextHeight(gs->textFont, labelText);
    PX_Object *label = PX_Object_LabelCreate(mp, pdfWidget->widget, 20, 20, labelWidth, labelHeight+8, 
                                                        labelText, gs->textFont->fontModule, gs->textColor);
    inputWidth = 240, inputHeight = 25;
    pdfWidget->pdata->selectBar = PX_Object_SelectBarCreate(mp, pdfWidget->widget, 30+labelWidth, 20, inputWidth, inputHeight, gs->textFont->fontModule);
    PX_Object *button = PX_Object_PushButtonCreate(mp, pdfWidget->widget, inputWidth+45+labelWidth, 20, 170, 
                                                            inputHeight, "Delete this Image", gs->textFont->fontModule);
    PX_Object_PushButtonSetStyle(button, PX_OBJECT_PUSHBUTTON_STYLE_ROUNDRECT);
    PX_ObjectRegisterEvent(button, PX_OBJECT_EVENT_EXECUTE, deleteFileCallback, NULL);
    button = PX_Object_PushButtonCreate(mp, pdfWidget->widget, inputWidth+45+labelWidth+170+20, 20, 120, inputHeight, "Find Images", gs->textFont->fontModule);
    PX_Object_PushButtonSetStyle(button, PX_OBJECT_PUSHBUTTON_STYLE_ROUNDRECT); 
    PX_ObjectRegisterEvent(button, PX_OBJECT_EVENT_EXECUTE, selectFilesCallback, selectImagesWidget);
    OTS_SelectFilesWidget_RegisterAddItemEvent(selectImagesWidget, addFilesCallback, pdfWidget);
    return pdfWidget;
}

void selectFilesCallback(PX_Object *obj, PX_Object_Event event, void *data) {
    OTS_SelectFilesWidget *widget = (OTS_SelectFilesWidget *)data;
    PX_Object_WidgetShow(widget->widget);
}

void deleteFileCallback(PX_Object *obj, PX_Object_Event event, void *data) {

}

void addFilesCallback(void *usrptr, void *data) {
    OTS_Vector *vec = (OTS_Vector *)data;
    OTS_ImageToPDFWidget *widget = (OTS_ImageToPDFWidget *)usrptr;
    // OTS_DEBUG("%s.\n", OTS_Vector_AT(vec, 0));
    PX_Object_SelectBarClear(widget->pdata->selectBar);
    for (int i=0;i<OTS_Vector_Size(vec);i++) {
        char *text = (char *)OTS_Vector_AT(vec, i);
        PX_Object_SelectBarAddItem(widget->pdata->selectBar, text);
    }
}


enum OTS_ConvertToPDFState OTS_ImagesToPDF(const char **imagespath, int imageslen, const char *pdfpath) {
    HPDF_Doc pdf = HPDF_New(NULL, NULL);
    if (!pdf) {
        OTS_ERROR("Error: Cannot create PDF document.\n");  return ERROR_NOT_CREATE_DOCUMENT;
    }
    for (int i=0;i<imageslen;i++) {
        HPDF_Page page = HPDF_AddPage(pdf);
        if (!page) {
            OTS_ERROR("Error: Cannot add page to PDF document, this page index is %d.\n", i+1);
            HPDF_Free(pdf); return ERROR_NOT_ADD_PAGE;
        }
        HPDF_Image image = NULL;
        // HPDF_Image image = HPDF_LoadJpegImageFromFile(pdf, imagespath[i]);
        const char *type = getImageType(imagespath[i]);
        if (!strcmp(type, "JPEG")) {
            image = HPDF_LoadJpegImageFromFile(pdf, imagespath[i]);
        } else if (!strcmp(type, "PNG")) {
            image = HPDF_LoadPngImageFromFile(pdf, imagespath[i]);
        } else {
            OTS_ERROR("Error: not support this image type.\n"); return ERROR_IMAGETYPE_NOT_SUPPORT;
        }
        int width = HPDF_Image_GetWidth(image), height = HPDF_Image_GetHeight(image);
        HPDF_Page_DrawImage(page, image, 100, 100, width, height);
    }
    if (HPDF_SaveToFile(pdf, pdfpath)!=HPDF_OK) {
        OTS_ERROR("Error: Cannot save pdf document.\n");
        HPDF_Free(pdf); return ERROR_NOT_SAVEPDF;
    }
    HPDF_Free(pdf);
    return CONVERT_SUCCESS;
}


const char *getImageType(const char *imagepath) {
    // 打开图像文件
    FILE *file = fopen(imagepath, "rb");
    if (!file) {
        OTS_ERROR("Error: Cannot open image file '%s'\n", imagepath);
        return NULL;
    }

    // 读取文件头部分
    unsigned char header[4];
    if (fread(header, sizeof(unsigned char), 4, file) != 4) {
        OTS_ERROR("Error: Cannot read image file '%s'\n", imagepath);
        fclose(file);
        return NULL;
    }

    // 关闭图像文件
    fclose(file);

    // 判断图像类型
    if (header[0] == 0xFF && header[1] == 0xD8 && header[2] == 0xFF) {
        return "JPEG";
    } else if (header[0] == 0x89 && header[1] == 0x50 && header[2] == 0x4E && header[3] == 0x47) {
        return "PNG";
    } else if (header[0] == 0x47 && header[1] == 0x49 && header[2] == 0x46 && header[3] == 0x38) {
        return "GIF";
    } else {
        return "Unknown";
    }
}
