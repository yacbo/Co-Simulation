
#include <string>
#include <QFile>
#include <QtXml>
#include "header_test.h"
#include "ms_handler.h"
#include "xml_message.h"
using namespace std;

bool generate_char_data(const char*& data, int& len)
{
    const char* xml_file = "F:\\work2\\bin\\debug\\invoke.xml";
    QFile file(xml_file);

    QDomDocument doc;
    bool ret = doc.setContent(&file);
    if(!ret){
        return nullptr;
    }

   XmlFrameUtil util;
   data = util.Xml2DataFrame(&doc, eEncode_utf8, eProtocol_tcp);
   len = util.GetMsgLength();

   return true;
}

void mshandler_test()
{
    MsHandler handler;
    handler.register_device("xxx", 0, "192.168.1.103", 9999, 0);

    int len = 0;
    const char* packet = nullptr;
    generate_char_data(packet, len);

    bool ret = handler.test_send_xml("192.168.1.103", 9999, packet, len);
}
