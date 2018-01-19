#include "midsoftui.h"
#include <QApplication>
#include <QTextCodec>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));
    MidSoftUI *w = new MidSoftUI();
    w->show();

    return a.exec();
}
