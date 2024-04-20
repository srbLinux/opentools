#include "./global.h"
#include "./widgets/mainwindow.h"

px_void OTS_Global_StyleChange(style_change_style_callback callback, void *data) {
    callback(data);
}

int main()
{
    OTS_MainWindow_Initialize();
    return 0;
}