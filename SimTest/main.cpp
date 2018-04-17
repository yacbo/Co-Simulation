#include <QCoreApplication>
#include "header_test.h"
#include "pg_rtui_def.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //sock_tcp_test(false);
    //sock_udp_test(true);
    //Xml2Attr();

    //mshandler_test();

    //power_handler();

    return a.exec();
}
