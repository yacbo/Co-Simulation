#include "sbssoftui.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SbsSoftUI w;
    w.show();
    return a.exec();
}
