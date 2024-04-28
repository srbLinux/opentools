#include "./global.h"
#include "./widgets/mainwindow.h"
#include "./widgets/messagebox.h"

px_void OTS_Global_StyleChange(style_change_style_callback callback, void *data) {
    callback(data);
}

FILE *logFILE=NULL;

int main()
{
#if __DEBUG_OTS__
    logFILE = stdout;
#else 
    logFILE = fopen("./run.log", "a+");
#endif
    OTS_MainWindow_Initialize();
    OTS_MessageBox_Warning("API", root, gs->textFont);
    return 0;
}