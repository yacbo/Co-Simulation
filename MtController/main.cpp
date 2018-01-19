#include "mt_ctrl_wnd.h"
#include <QApplication>
#include  "comm_param_settings.h"
#include  "arch_param_settings.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow *wd = new MainWindow();
    //wd->setAttribute(Qt::WA_DeleteOnClose);
    wd->show();

    return a.exec();
}
