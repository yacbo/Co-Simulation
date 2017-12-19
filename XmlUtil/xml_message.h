
/************************************************************************/
/*                                                                      */
/************************************************************************/

#ifndef __XML_MESSAGE_H__
#define __XML_MESSAGE_H__

#include <QtXml>
#include "xmlutil_global.h"
#include "comm_type_def.h"

//Xml消息基类
class XmlMessageBase
{
public:
	//消息类型
	long _msg_type;

public:
	//类属性转换为Xml元素
    virtual QDomElement Attr2XmlElement() = 0;

	/*Xml元素转换为类属性
	 *element:指定的xml元素
	 */
    virtual QDomElement XmlElement2Attr(QDomElement element) = 0;

	/*利用类属性填充Xml相应的元素
	 *root:xml根节点
	 */
    virtual void AttrFillXmlElement(QDomElement root) = 0;
};

//数据消息基类
class XMLUTILSHARED_EXPORT DataXmlMsgBase : public XmlMessageBase
{
public:
    ETransDataType _data_type;
public:
    virtual QDomElement Attr2XmlElement();
    virtual QDomElement XmlElement2Attr(QDomElement element);
    virtual void AttrFillXmlElement(QDomElement root);
};

typedef vector<DataXmlMsgBase*> DataXmlVec;

//步骤消息体
class XMLUTILSHARED_EXPORT ApplStepMsgBody : public XmlMessageBase
{
public:
	//步骤数目
	int _step_num;
	//步骤编号集合
	IntVec _step_vec;
	//过程名集合
	StrVec _proc_name_vec;

public:
    QDomElement Attr2XmlElement();
    QDomElement XmlElement2Attr(QDomElement element);
    void AttrFillXmlElement(QDomElement root);
};

//应用消息体
class XMLUTILSHARED_EXPORT ApplMessageBody : public XmlMessageBase
{
public:
    ApplMessageBody();

public:
	//应用步骤数目
	int _appl_step;
	//应用名称
	string _appl_name;
	//步骤消息体
    ApplStepMsgBody* _msg_step;

public:
    QDomElement Attr2XmlElement();
    QDomElement XmlElement2Attr(QDomElement element);
    void AttrFillXmlElement(QDomElement root);
};

//函数调用体
class XMLUTILSHARED_EXPORT FuncInvokeBody : public XmlMessageBase
{
public:
    FuncInvokeBody();

public:
    string _name;
    DataXmlVec _data;

public:
    QDomElement Attr2XmlElement();
    QDomElement XmlElement2Attr(QDomElement element);
    void AttrFillXmlElement(QDomElement root);
};

//过程消息体
class XMLUTILSHARED_EXPORT ProcedureMessageBody : public XmlMessageBase
{
public:
    ProcedureMessageBody();

public:
    long _proc_type;
	string _msg_name;
    DataXmlVec _data_vector;
    ApplMessageBody* _appl_msg_body;
    FuncInvokeBody* _func_invoke_body;

public:
    QDomElement Attr2XmlElement();
    QDomElement XmlElement2Attr(QDomElement element);
    void AttrFillXmlElement(QDomElement root);

public:
    long FetchPayloadData(char* ret);              //不支持字符串

private:
    bool ParseDataItem(const DataXmlMsgBase* data, QByteArray& ret);
    bool ParseDataItem(const string& val, int type, QByteArray& ret);
};

//应用消息
struct ApplMessage
{
    //会话源宿
	int _i2u;
    int _u2i;
    long _pg_rtui_type;
	//过程消息体
    ProcedureMessageBody* _proc_msg;
};

//会话消息体
class XMLUTILSHARED_EXPORT SessionMessageBody : public XmlMessageBase
{
public:
    SessionMessageBody();

public:
	//i2u编号
	long _id_i2u;
	//u2i编号
	long _id_u2i;
	//过程消息体
    ProcedureMessageBody* _procedure_msg_body;

public:
    QDomElement Attr2XmlElement();
    QDomElement XmlElement2Attr(QDomElement element);
    void AttrFillXmlElement(QDomElement root);
};

typedef queue<SessionMessageBody> SessionMsgQue;

//根消息体
class XMLUTILSHARED_EXPORT RootMessageBody : public XmlMessageBase
{
public:
    RootMessageBody();

public:
	//会话消息体
    SessionMessageBody* _session_msg_body;

public:
    QDomDocument* Attr2Document();
    QDomElement Attr2XmlElement();
    QDomElement XmlElement2Attr(QDomElement element);
    void AttrFillXmlElement(QDomElement root);
};

//网络地址数据
class XMLUTILSHARED_EXPORT NetAddrMsgDataBody : public DataXmlMsgBase
{
public:
    NetAddrMsgDataBody();

public:
	//器件名称
	string _device_name;
	//器件ip
	uint32_t _device_ip;
	//器件port
	uint32_t _device_port;

public:
    QDomElement Attr2XmlElement();
    QDomElement XmlElement2Attr(QDomElement element);
    void AttrFillXmlElement(QDomElement root);
};

//文件数据
class XMLUTILSHARED_EXPORT FileMsgDataBody : public DataXmlMsgBase
{
public:
    FileMsgDataBody();

public:
	//文件名
	string _file_name;
	//文件路径
	string _file_path;

public:
    QDomElement Attr2XmlElement();
    QDomElement XmlElement2Attr(QDomElement element);
    void AttrFillXmlElement(QDomElement root);
};

//表格数据
class XMLUTILSHARED_EXPORT TableMsgDataBody : public DataXmlMsgBase
{
public:
    TableMsgDataBody();

public:
	//表格名称
	string _table_name;

public:
    QDomElement Attr2XmlElement();
    QDomElement XmlElement2Attr(QDomElement element);
    void AttrFillXmlElement(QDomElement root);
};

//变量数据
class XMLUTILSHARED_EXPORT VariableMsgDataBody : public DataXmlMsgBase
{
public:
    VariableMsgDataBody();

public:
	//变量名称
	string _var_name;
	//变量类型
	int _var_type;
	//变量值
	string _var_value;

public:
    QDomElement Attr2XmlElement();
    QDomElement XmlElement2Attr(QDomElement element);
    void AttrFillXmlElement(QDomElement root);
};

//Xml消息与数据帧转换工具//
class XMLUTILSHARED_EXPORT XmlFrameUtil
{
public:
    XmlFrameUtil();

public:
    /*计算帧校验和
     *fr_data:帧数据
     */
    uint32_t CheckFrameSum(const char* fr_data);

	/*Xml消息转换为数据帧
	 *doc:xml消息体
	 *encode:编码类型
     *protocol:协议类型
	 */
    const char* Xml2DataFrame(QDomDocument* doc, EEncodeType encode, EProtocolType protocol);

	/*帧数据转换为xml消息
	 *fr_data:帧数据
	 *type:编码类型
	 *index:转换起始字节下标
     * return: QDomDocument*, 使用完需要释放
	 */
    QDomDocument* DataFrame2Xml(const char* fr_data, EEncodeType type, uint32_t index);

    /*验证校验和是否一致
     * fr_data:帧数据
    */
    bool VerifyCheckSum(const char* fr_data);

    /* 获取msg长度 */
    long GetMsgLength();

private:
    /* 按大端模式处理，即高位在低字节 */
    uint32_t FetchUint32Value(const char* fr_data, int offset);

    /* 计算校验和 */
    uint32_t CalculateCheckSum(const char* fr_data, int offset);

    /* 填充32位整型值 */
    void FillUint32Value(const char* fr_data, uint32_t value, int offset);

    /* 转换数据格式，utf-8或ASCII */
    QString ConvertDataFormat(const char* data, int len, EEncodeType dst_fmt);

    /* 初始化CRC校验表 */
    static void InitCRC32Table();

private:
    long _msg_len;
};

#endif
