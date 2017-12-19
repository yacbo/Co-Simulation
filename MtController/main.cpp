#include "mt_ctrl_wnd.h"
#include <QApplication>
#include  "comm_param_settings.h"
#include  "arch_param_settings.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();
    return a.exec();
}
