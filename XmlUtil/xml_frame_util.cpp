
#include "xml_message.h"

XmlFrameUtil::XmlFrameUtil()
{

}

uint32_t XmlFrameUtil::CheckFrameSum(const char* fr_data)
{
    if(!fr_data){
        return 0;
    }

    //长度标识
    uint32_t xml_len = FetchUint32Value(fr_data, 1);

    //计算校验和
    uint32_t check_sum = CalculateCheckSum(fr_data, 1 + 4 + xml_len);

    return check_sum;
}

const char* XmlFrameUtil::Xml2DataFrame(QDomDocument* doc, EEncodeType encode, EProtocolType protocol)
{
    //确定协议标识符
    uint8_t type = 0xff;
    switch (protocol) {
    case eProtocol_udp: type = 0x70; break;
    case eProtocol_tcp: type = 0x7f; break;
    default: break;
    }

    if(type == 0xff){
        return nullptr;
    }

    //数据格式转换
    QString src_xml_msg = doc->toString();
    string tmp_xml = src_xml_msg.toStdString();
    QString dst_xml_msg = ConvertDataFormat(tmp_xml.c_str(), tmp_xml.length(), encode);
    if(dst_xml_msg.isEmpty()){
        return nullptr;
    }

    //计算数据帧长度
    int xml_len = dst_xml_msg.size();
    int fr_len = 1 + 4 + xml_len + 4;
    char* fr_data = new char[fr_len];
    memset(fr_data, 0, fr_len);

    //设置消息长度
    _msg_len = fr_len;

    //设置协议标识
    fr_data[0] = type;

    //设置消息长度
    FillUint32Value(fr_data, xml_len, 1);
    //设置消息体
    memcpy(fr_data + 1 + 4, dst_xml_msg.toStdString().c_str(), xml_len);

    //设置校验和
    uint32_t check_sum = CalculateCheckSum(fr_data, 1 + 4 + xml_len);
    FillUint32Value(fr_data + fr_len - 4, check_sum, 0);

    return fr_data;
}

QDomDocument* XmlFrameUtil::DataFrame2Xml(const char* fr_data, EEncodeType type, uint32_t index)
{
    //校验和不一致
    if(!fr_data || !VerifyCheckSum(fr_data)){
        return nullptr;
    }

    //长度标识
    uint32_t xml_len = FetchUint32Value(fr_data, 1);

    //拷贝xml内容
    char* src_fmt_xml = new char[xml_len];
    memcpy(src_fmt_xml, fr_data + 1 + 4, xml_len);

    //编码格式转换
    QString dst_fmt_xml = ConvertDataFormat(src_fmt_xml, xml_len, type);
    delete[] src_fmt_xml;

    if(dst_fmt_xml.isEmpty()){
        return nullptr;
    }

    QString err_msg;
    int err_line, err_col;

    //字符串转换为document
    QDomDocument doc;
    bool ret = doc.setContent(dst_fmt_xml, &err_msg, &err_line, &err_col);

    return ret ? new QDomDocument(doc) : nullptr;
}

bool XmlFrameUtil::VerifyCheckSum(const char* fr_data)
{
    if(!fr_data){
        return false;
    }

    //长度标识
    uint32_t xml_len = FetchUint32Value(fr_data, 1);
    if(xml_len > 100000){
        return false;
    }

    //校验标识
    uint32_t check_sum = FetchUint32Value(fr_data, 1 + 4 + xml_len);
    //计算校验和
    uint32_t check_ret = CalculateCheckSum(fr_data, 1 + 4 + xml_len);

    _msg_len = 1 + 4 + xml_len + 4;

    return check_sum == check_ret;
}

long XmlFrameUtil::GetMsgLength()
{
    return _msg_len;
}

uint32_t XmlFrameUtil::FetchUint32Value(const char* fr_data, int offset)
{
    uint32_t value = 0;
    unsigned  char c2i[sizeof(uint32_t)];

    const char* tmp = fr_data + offset;
    memcpy(c2i, tmp, 4);

    value = *(uint32_t*)c2i;

    return value;
}

uint32_t XmlFrameUtil::CalculateCheckSum(const char* fr_data, int len)
{
    uint32_t crc_16 = qChecksum(fr_data, len);
    return crc_16;
}

void XmlFrameUtil::FillUint32Value(const char* fr_data, uint32_t value, int offset)
{
    unsigned char i2c[sizeof(uint32_t)];
    *((uint32_t*)i2c) = value;

    char* tmp = const_cast<char*>(fr_data + offset);
    memcpy(tmp, i2c, 4);
}

QString XmlFrameUtil::ConvertDataFormat(const char* data, int len, EEncodeType dst_fmt)
{
    QString ret;
    if(!data || dst_fmt == eEncode_undef){
        return ret;
    }

    QByteArray fmt_name;
    switch(dst_fmt){
    case eEncode_utf8: fmt_name = "UTF-8"; break;
    case eEncode_ascii: fmt_name = "ASCII"; break;
    default:break;
    }

    string tmp(data, len);
    QString xml_data = QString::fromStdString(tmp);
    QTextStream data_stream(&xml_data);
    QTextCodec* codec = QTextCodec::codecForName(fmt_name);
    if(!codec){
        return ret;
    }

    data_stream.setCodec(codec);
    ret = data_stream.readAll();

    return ret;
}


