#include "xml_message.h"
#include "xml_util.h"
//步骤消息体
QDomElement ApplStepMsgBody::Attr2XmlElement()
{
    QDomDocument doc;
    QDomElement steps = doc.createElement("steps");
    QDomElement number = doc.createElement("number");
    QDomAttr attr = doc.createAttribute("value");
    attr.setValue(QString::number(this->_step_num,10));
    number.setAttributeNode(attr);
    steps.appendChild(number);
    for(size_t i=0;i<this->_step_vec.size();i++)
    {
        QDomElement step = doc.createElement("step");
        QDomAttr attr = doc.createAttribute("value");
        attr.setValue(QString::number(this->_step_vec[i],10));
        step.setAttributeNode(attr);

        QDomElement progress = doc.createElement("progress");
        QDomAttr attr_s = doc.createAttribute("value");
        attr_s.setValue(QString::fromStdString(this->_proc_name_vec[i]));
        progress.setAttributeNode(attr_s);

        step.appendChild(progress);
        steps.appendChild(step);
    }
    return steps;
}

QDomElement ApplStepMsgBody::XmlElement2Attr(QDomElement element)
 {
     QDomNode child =  element.firstChild();
     while(!child.isNull())
     {
         if(child.toElement().tagName() == "number")
         {
             //int _step_num 属性
             if(child.toElement().hasAttribute("value"))
             {
                 this->_step_num = child.toElement().attribute("value").toInt();
             }
         }
         if(child.toElement().tagName() == "step")
         {
             //_step_vec属性
             if(child.toElement().hasAttribute("value"))
             {
                 //child.toElement().setAttribute("value",QString::number(_step_num,10));
                  this->_step_vec.push_back(child.toElement().attribute("value").toInt());
             }
            QDomNode child_s = child.toElement().firstChild();
            if(child_s.toElement().tagName()=="progress")
            {
                //_proc_name_vec属性
                if(child_s.toElement().hasAttribute("value"))
                {
                    this->_proc_name_vec.push_back(child_s.toElement().attribute("value").toStdString());
                }
            }
         }
         child = child.nextSibling();
     }
     return element;
 }

void ApplStepMsgBody::AttrFillXmlElement(QDomElement root)
 {
     int i = 0;
     QDomNode child =  root.firstChild();
     while(!child.isNull())
     {
         if(child.toElement().tagName() == "number")
         {
             //int _step_num 属性
             if(child.toElement().hasAttribute("value"))
             {
                 child.toElement().setAttribute("value",QString::number(this->_step_num,10));
                 //_step_num = child.toElement().attribute("value").toInt();
             }
         }
         if(child.toElement().tagName() == "step")
         {
             //_step_vec属性
             int _step_int = 0;
             if(child.toElement().hasAttribute("value"))
             {
                 _step_int = child.toElement().attribute("value").toInt();
             }
            QDomNode child_s = child.toElement().firstChild();
            if(child_s.toElement().tagName()=="progress")
            {
                //_proc_name_vec属性
                if(child_s.toElement().hasAttribute("value"))
                {
                    child.toElement().setAttribute(QString::fromStdString("value"),QString::fromStdString(this->_proc_name_vec[_step_int]));
                    //_proc_name_vec[i] = child.toElement().attribute("value");
                }
            }
            i++;
         }
         child = child.nextSibling();
     }
 }

//应用消息体
QDomElement ApplMessageBody::Attr2XmlElement()
{
   QDomDocument doc;
   QDomElement application = doc.createElement("application");
   QDomElement name = doc.createElement("name");
   QDomAttr name_attr = doc.createAttribute("value");
   name_attr.setValue(QString::fromStdString(_appl_name));
   name.setAttributeNode(name_attr);

   QDomElement step = doc.createElement("step");
   QDomAttr step_attr = doc.createAttribute("value");
   step_attr.setValue(QString::number(_appl_step,10));
   step.setAttributeNode(step_attr);
   application.appendChild(name);
   application.appendChild(step);
   if(_msg_step)
   {
       QDomElement steps = _msg_step->Attr2XmlElement();
       application.appendChild(steps);
   }

   return application;
}

QDomElement ApplMessageBody::XmlElement2Attr(QDomElement element)
{
    QDomElement reElement;
    QDomNode child = element.firstChild();
    while(!child.isNull())
    {
        if(child.toElement().tagName() == "name")
        {
            //string _appl_name 属性
            if(child.toElement().hasAttribute("value"))
            {
                this->_appl_name = child.toElement().attribute("value").toStdString();
            }
        }
        if(child.toElement().tagName() == "step")
        {
            //int _appl_step 属性
            if(child.toElement().hasAttribute("value"))
            {
                this->_appl_step = child.toElement().attribute("value").toInt();
            }
        }
        if(child.toElement().tagName()=="steps")
        {
            reElement = child.toElement();
            _msg_step = new ApplStepMsgBody();
            this->_msg_step->XmlElement2Attr(reElement);
        }
        child = child.nextSibling();
    }
    return reElement;
}

void ApplMessageBody::AttrFillXmlElement(QDomElement root)
{
    QDomNode child =  root.firstChild();
    while(!child.isNull())
    {
        if(child.toElement().tagName() == "name")
        {
            //string _appl_name 属性
            if(child.toElement().hasAttribute("value"))
            {
                child.toElement().setAttribute("value",QString::fromStdString(_appl_name));
            }
        }
        if(child.toElement().tagName() == "step")
        {
            //int _appl_step属性
            if(child.toElement().hasAttribute("value"))
            {
                child.toElement().setAttribute("value",QString::number(this->_appl_step,10));
            }
        }
        if(child.toElement().tagName()=="steps")
        {
            this->_msg_step->AttrFillXmlElement(child.toElement());
        }
        child = child.nextSibling();
    }
}

ApplMessageBody::ApplMessageBody()
{
    this->_msg_step = nullptr;
}

//函数调用体
FuncInvokeBody::FuncInvokeBody()
{
    _msg_type = eSubProcedure_invoke;
}

QDomElement FuncInvokeBody::Attr2XmlElement()
{
    QDomDocument doc;
    QDomElement function = doc.createElement("function");
    QDomElement name = doc.createElement("name");
    QDomAttr name_attr = doc.createAttribute("value");
    name_attr.setValue(QString::fromStdString(_name));
    name.setAttributeNode(name_attr);

    QDomElement type = doc.createElement("type");
    QDomAttr type_attr = doc.createAttribute("value");
    type_attr.setValue(QString::number(_msg_type,10));
    type.setAttributeNode(type_attr);
    if(_data.size()>0)
    {
        for(int i= 0;i<_data.size();i++)
        {
            if( eTransData_variable == _data[i]->_data_type)
            {
                VariableMsgDataBody* variable = (VariableMsgDataBody*)_data.at(i);
                QDomElement varEl =  variable->Attr2XmlElement();
                type.appendChild(varEl);
            }

            if(eTransData_table == _data.at(i)->_data_type)
            {
                TableMsgDataBody* table = (TableMsgDataBody*)_data.at(i);
                QDomElement tabEl =  table->Attr2XmlElement();
                type.appendChild(tabEl);
            }

            if(eTransData_file == _data.at(i)->_data_type)
            {
                FileMsgDataBody* file = (FileMsgDataBody*)_data.at(i);
                QDomElement fileEl =  file->Attr2XmlElement();
                type.appendChild(fileEl);
            }

            if(eTransData_net_addr == _data.at(i)->_data_type)
            {
                NetAddrMsgDataBody* netAddr = (NetAddrMsgDataBody*)_data.at(i);
                QDomElement netEl =  netAddr->Attr2XmlElement();
                type.appendChild(netEl);
            }
        }

    }
    function.appendChild(name);
    function.appendChild(type);
    return function;
}

QDomElement FuncInvokeBody::XmlElement2Attr(QDomElement element)
{
    QDomElement child = element.firstChildElement();

    while(!child.isNull())
    {
        if(child.tagName() == "name")
        {
            //string _name 属性
            if(child.hasAttribute("value"))
            {
                this->_name = child.attribute("value").toStdString();
            }
        }
        if(child.tagName() == "type")
        {
            //int _msg_type 属性
            QDomElement typeEl = child.toElement();
            if(typeEl.hasAttribute("value"))
            {
                this->_msg_type = child.attribute("value").toLong();
            }

            QDomNode child_s = typeEl.firstChild();
            while(!child_s.isNull())
            {
                QString str =child_s.toElement().firstChildElement().attribute("value");
                if(str.toStdString() =="net address")
                {
                    NetAddrMsgDataBody* data = new NetAddrMsgDataBody();
                    data->XmlElement2Attr(child_s.toElement());
                    this->_data.push_back(data);
                }
                if(str.toStdString() =="")
                {
                    TableMsgDataBody* data = new TableMsgDataBody();
                    data->XmlElement2Attr(child_s.toElement());
                    this->_data.push_back(data);
                }
                if(str.toStdString() =="file")
                {
                    FileMsgDataBody* data = new FileMsgDataBody();
                    data->XmlElement2Attr(child_s.toElement());
                    this->_data.push_back(data);
                }
                if(str.toStdString() =="variable")
                {
                    VariableMsgDataBody* data = new VariableMsgDataBody();
                    data->XmlElement2Attr(child_s.toElement());
                    this->_data.push_back(data);
                }

                child_s = child_s.nextSibling();
            }
        }
        child = child.nextSiblingElement();
    }
    return element;
}

void FuncInvokeBody::AttrFillXmlElement(QDomElement root)
{

}

//过程消息体
QDomElement ProcedureMessageBody::Attr2XmlElement()
{
    QDomDocument doc;
    QDomElement progress = doc.createElement("progress");
    QDomElement name = doc.createElement("name");
    QDomAttr name_attr = doc.createAttribute("value");
    name_attr.setValue(QString::fromStdString(_msg_name));
    QDomAttr nameType_attr = doc.createAttribute("type");
    nameType_attr.setValue(QString::number(_proc_type, 10));
    name.setAttributeNode(name_attr);  //添加name属性.
    name.setAttributeNode(nameType_attr); //添加type属性.

    QDomElement type = doc.createElement("type");
    QDomAttr type_attr = doc.createAttribute("value");
    type_attr.setValue(QString::number(_msg_type,10));
    type.setAttributeNode(type_attr);

    if(_msg_name =="invoke")
    {
        if(!_func_invoke_body)
        {
            _func_invoke_body = new FuncInvokeBody();
        }
        QDomElement functionEle = _func_invoke_body->Attr2XmlElement();
        type.appendChild(functionEle);
    }
    else
    {
        if(_data_vector.size()>0)
        {
            for(int i= 0;i<_data_vector.size();i++)
            {
                if( eTransData_variable == _data_vector[i]->_data_type)
                {
                    VariableMsgDataBody* variable = (VariableMsgDataBody*)_data_vector.at(i);
                    QDomElement varEl =  variable->Attr2XmlElement();
                    type.appendChild(varEl);
                }

                if(eTransData_table == _data_vector.at(i)->_data_type)
                {
                    TableMsgDataBody* table = (TableMsgDataBody*)_data_vector.at(i);
                    QDomElement tabEl =  table->Attr2XmlElement();
                    type.appendChild(tabEl);
                }

                if(eTransData_file == _data_vector.at(i)->_data_type)
                {
                    FileMsgDataBody* file = (FileMsgDataBody*)_data_vector.at(i);
                    QDomElement fileEl =  file->Attr2XmlElement();
                    type.appendChild(fileEl);
                }

                if(eTransData_net_addr == _data_vector.at(i)->_data_type)
                {
                    NetAddrMsgDataBody* netAddr = (NetAddrMsgDataBody*)_data_vector.at(i);
                    QDomElement netEl =  netAddr->Attr2XmlElement();
                    type.appendChild(netEl);
                }
            }
        }
    }
    progress.appendChild(name);
    progress.appendChild(type);
    if(_appl_msg_body)
    {
        QDomElement application = _appl_msg_body->Attr2XmlElement();
        progress.appendChild(application);
    }
    return progress;
}

QDomElement ProcedureMessageBody::XmlElement2Attr(QDomElement element)
{

    QDomElement reElement;
    QDomNode child = element.firstChild();
    while(!child.isNull())
    {
        if(child.toElement().tagName() == "name")
        {
            //string _msg_name 属性
            if(child.toElement().hasAttribute("value"))
            {
                this->_msg_name = child.toElement().attribute("value").toStdString();
            }

            //long _pro_type 属性
            if(child.toElement().hasAttribute("type"))
            {
                this->_proc_type = child.toElement().attribute("type").toLong();
            }
        }
        if(child.toElement().tagName() == "type")
        {
            //int _proc_type 属性
            QDomElement typeEl = child.toElement();
            if(typeEl.hasAttribute("value"))
            {
                this->_msg_type = child.toElement().attribute("value").toLong();
            }
            QDomNode child_s = typeEl.firstChild();
            while(!child_s.isNull())
            {
                if(child_s.toElement().tagName()=="function")//function节点
                {
                    reElement = child_s.toElement();
                    _func_invoke_body = new FuncInvokeBody();
                    this->_func_invoke_body->XmlElement2Attr(reElement);
                }
                else
                {
                    QString str = child_s.toElement().firstChildElement().attribute(QString::fromStdString("value"));
                    if(str.toStdString() =="net address")
                    {
                        NetAddrMsgDataBody* data = new NetAddrMsgDataBody();
                        data->XmlElement2Attr(child_s.toElement());
                        this->_data_vector.push_back(data);
                    }
                    if(str.toStdString() =="")
                    {
                        TableMsgDataBody* data = new TableMsgDataBody();
                        data->XmlElement2Attr(child_s.toElement());
                        this->_data_vector.push_back(data);
                    }
                    if(str.toStdString() =="file")
                    {
                        FileMsgDataBody* data = new FileMsgDataBody();
                        data->XmlElement2Attr(child_s.toElement());
                        this->_data_vector.push_back(data);
                    }
                    if(str.toStdString() =="variable")
                    {
                        VariableMsgDataBody* data = new VariableMsgDataBody();
                        data->XmlElement2Attr(child_s.toElement());
                        this->_data_vector.push_back(data);
                    }
                }
                child_s = child_s.nextSibling();
            }
        }
        if(child.toElement().tagName() == "application")
        {
            reElement = child.toElement();
            _appl_msg_body = new ApplMessageBody();
            this->_appl_msg_body->XmlElement2Attr(reElement);
        }
        child = child.nextSibling();
    }
    return reElement;
}

void ProcedureMessageBody::AttrFillXmlElement(QDomElement root)
{
    QDomNode child =  root.firstChild();
    while(!child.isNull())
    {
        if(child.toElement().tagName() == "name")
        {
            //string _msg_name 属性
            if(child.toElement().hasAttribute("value"))
            {
                child.toElement().setAttribute("value",QString::fromStdString(this->_msg_name));
            }
        }
        if(child.toElement().tagName() == "type")
        {
            //long _proc_type属性
            if(child.toElement().hasAttribute("value"))
            {
                child.toElement().setAttribute("value",QString::number(this->_proc_type,10));
            }
        }
        if(child.toElement().tagName()=="application")
        {
            this->_appl_msg_body->AttrFillXmlElement(child.toElement());
        }
        child = child.nextSibling();
    }
}

long ProcedureMessageBody::FetchPayloadData(char* ret_data)
{
    long len = 0;
    if(_data_vector.size() == 0){
        return len;
    }

    bool ret = true;
    QByteArray  byte_ret;
    for(int i=0; i<_data_vector.size(); ++i){
        ret &= ParseDataItem(_data_vector[i], byte_ret);
    }

    if(ret){
        len =  byte_ret.length();
        memcpy(ret_data, byte_ret.data(), len);
    }

    return len;
}

bool ProcedureMessageBody::ParseDataItem(const DataXmlMsgBase* data, QByteArray& data_ret)
{
    bool ret = true;
    switch (data->_data_type) {
    case eTransData_file: break;
    case eTransData_table: break;
    case eTransData_net_addr: break;
    case eTransData_variable:{
        VariableMsgDataBody* v_data = (VariableMsgDataBody*)data;
        ret = ParseDataItem(v_data->_var_value, v_data->_var_type, data_ret);
        break;
        }
    default:  break;
    }

    return ret;
}

bool ProcedureMessageBody::ParseDataItem(const string& val, int type, QByteArray& data_ret)
{
    bool ret = true;

    int len = 0;
    unsigned char cv[8] = {0};

    switch (type) {
    case eData_int8:  len = sizeof(int8_t); *((int8_t*)cv) = std::stol(val); break;
    case eData_uint8: len = sizeof(uint8_t); *((uint8_t*)cv) = std::stoul(val); break;
    case eData_int16: len = sizeof(int16_t); *((int16_t*)cv) = std::stol(val); break;
    case eData_uint16: len = sizeof(uint16_t); *((uint16_t*)cv) = std::stoul(val); break;
    case eData_int32: len = sizeof(int32_t); *((int32_t*)cv) = std::stol(val); break;
    case eData_uint32: len = sizeof(uint32_t); *((uint32_t*)cv) = std::stoul(val); break;
    case eData_int64: len = sizeof(int64_t); *((int64_t*)cv) = std::stoll(val); break;
    case eData_uint64: len = sizeof(uint64_t); *((uint64_t*)cv) = std::stoull(val); break;
    case eData_float: len = sizeof(float); *((float*)cv) = std::stof(val); break;
    case eData_double: len = sizeof(double); *((double*)cv) = std::stod(val); break;
    case eData_string:  ret = true; break;
    case eData_ptr32:  ret = false; break;
    case eData_ptr64: ret = false; break;
    default: break;
    }

    if(type != eData_string){
        char data[8];
        memcpy(data, cv, 8);
        data_ret.push_back(QByteArray::fromRawData(data, len));
    }
    else{
        data_ret.push_back(QByteArray::fromRawData(val.c_str(), 20));
    }

    return ret;
}

ProcedureMessageBody::ProcedureMessageBody()
{
    this->_appl_msg_body = nullptr;
    this->_func_invoke_body = nullptr;
}

//会话消息体
QDomElement SessionMessageBody::Attr2XmlElement()
{
    QDomDocument doc;
    QDomElement session = doc.createElement("session");
    QDomElement I2U = doc.createElement("I2U");
    QDomAttr I2U_attr = doc.createAttribute("value");
    I2U_attr.setValue(QString::number(_id_i2u,10));
    I2U.setAttributeNode(I2U_attr);

    QDomElement U2I = doc.createElement("U2I");
    QDomAttr U2I_attr = doc.createAttribute("value");
    U2I_attr.setValue(QString::number(_id_u2i,10));
    U2I.setAttributeNode(U2I_attr);

    QDomElement sessType = doc.createElement("sessType");
    QDomAttr sessType_attr = doc.createAttribute("value");
    sessType_attr.setValue(QString::number(_msg_type,10));
    sessType.setAttributeNode(sessType_attr);


    session.appendChild(I2U);
    session.appendChild(U2I);
    session.appendChild(sessType);
    if(_procedure_msg_body)
    {
        QDomElement progress = _procedure_msg_body->Attr2XmlElement();
        session.appendChild(progress);
    }
    return session;
}

QDomElement SessionMessageBody::XmlElement2Attr(QDomElement element)
{
    QDomElement reElement;
    QDomNode child = element.firstChild();
    while(!child.isNull())
    {
        if(child.toElement().tagName() == "I2U")
        {
            //long _id_i2u 属性
            if(child.toElement().hasAttribute("value"))
            {
                this->_id_i2u = child.toElement().attribute("value").toLong();
            }
        }
        if(child.toElement().tagName() == "U2I")
        {
            //long _id_u2i 属性
            if(child.toElement().hasAttribute("value"))
            {
                this->_id_u2i = child.toElement().attribute("value").toLong();
            }
        }
        if(child.toElement().tagName() == "sessType")
        {
            //sessType 属性
            if(child.toElement().hasAttribute("value"))
            {
                this->_msg_type = child.toElement().attribute("value").toLong();
            }
        }
        if(child.toElement().tagName()=="progress")
        {
            reElement = child.toElement();
            _procedure_msg_body = new ProcedureMessageBody();
            this->_procedure_msg_body->XmlElement2Attr(reElement);
        }
        child = child.nextSibling();
    }
    return reElement;
}

void SessionMessageBody::AttrFillXmlElement(QDomElement root)
{
    QDomNode child =  root.firstChild();
    while(!child.isNull())
    {
        if(child.toElement().tagName() == "I2U")
        {
            //long _id_i2u 属性
            if(child.toElement().hasAttribute("value"))
            {
                child.toElement().setAttribute("value",QString::number(this->_id_i2u,10));
            }
        }
        if(child.toElement().tagName() == "U2I")
        {
            //long _id_u2i属性
            if(child.toElement().hasAttribute("value"))
            {
                child.toElement().setAttribute("value",QString::number(this->_id_u2i,10));
            }
        }
        if(child.toElement().tagName()=="application")
        {
            this->_procedure_msg_body->AttrFillXmlElement(child.toElement());
        }
        child = child.nextSibling();
    }
}
SessionMessageBody::SessionMessageBody()
{
    this->_procedure_msg_body =nullptr;
    this->_msg_type = 0;
}

//根消息体
QDomElement RootMessageBody::Attr2XmlElement()
{
    QDomDocument doc;
    QDomElement message = doc.createElement("message");
    if(_session_msg_body)
    {
        QDomElement session = _session_msg_body->Attr2XmlElement();
        message.appendChild(session);
    }
    return message;
}

QDomElement RootMessageBody::XmlElement2Attr(QDomElement element)
{
    QDomElement reElement;
    QDomNode child = element.firstChild();
    while(!child.isNull())
    {
        if(child.toElement().tagName()=="session")
        {
           this->_session_msg_body->XmlElement2Attr(child.toElement());
           reElement = child.toElement();
        }
        child = child.nextSibling();
    }
    return reElement;
}

void RootMessageBody::AttrFillXmlElement(QDomElement root)
{
    QDomNode child = root.firstChild();
    while(!child.isNull())
    {
        if(child.toElement().tagName()=="session")
        {
           this->_session_msg_body->AttrFillXmlElement(child.toElement());
        }
        child = child.nextSibling();
    }
}

QDomDocument* RootMessageBody::Attr2Document()
{
    QDomDocument* doc = new QDomDocument();
    QDomElement message = doc->createElement( "message" );
    if(_session_msg_body)
    {
        QDomElement session = _session_msg_body->Attr2XmlElement();
        message.appendChild(session);
    }
    doc->appendChild(message);
    return doc;
}

RootMessageBody::RootMessageBody()
{
    this->_session_msg_body = nullptr;
}

//数据消息基类
QDomElement DataXmlMsgBase::Attr2XmlElement()
{
    QDomElement message;
    return message;
}

QDomElement DataXmlMsgBase::XmlElement2Attr(QDomElement element)
{
    return element;
}

void DataXmlMsgBase::AttrFillXmlElement(QDomElement root){}


//网络地址数据
QDomElement NetAddrMsgDataBody::Attr2XmlElement()
{
    QDomDocument doc;
    QDomElement data = doc.createElement("data");

    QDomElement type = doc.createElement("type");
    QDomAttr type_attr = doc.createAttribute("value");
    type_attr.setValue("net address");
    type.setAttributeNode(type_attr);
    /*address 节点*/
    QDomElement address = doc.createElement("address");
    QDomElement name = doc.createElement("name");
    QDomElement ip = doc.createElement("ip");
    QDomElement port = doc.createElement("port");

    QDomAttr name_attr = doc.createAttribute("value");
    name_attr.setValue(QString::fromStdString(_device_name));
    name.setAttributeNode(name_attr);
    QDomAttr ip_attr = doc.createAttribute("value");
    ip_attr.setValue(QString::number(_device_ip,10));
    ip.setAttributeNode(ip_attr);

    QDomAttr port_attr = doc.createAttribute("value");
    port_attr.setValue(QString::number(_device_port,10));
    port.setAttributeNode(port_attr);

    address.appendChild(name);
    address.appendChild(ip);
    address.appendChild(port);
    /*address 节点*/

    data.appendChild(type);
    data.appendChild(address);

    return data;
}

QDomElement NetAddrMsgDataBody::XmlElement2Attr(QDomElement element)
{
    QDomNode child = element.firstChild();
    while(!child.isNull())
    {
        if(child.toElement().tagName()=="address")
        {
            QDomNode child_s = child.toElement().firstChild();
            while(!child_s.isNull())
            {
                if(child_s.toElement().tagName()=="name")
                {
                    if(child_s.toElement().hasAttribute("value"))
                    {
                        this->_device_name = child_s.toElement().attribute("value").toStdString();
                    }
                }
                if(child_s.toElement().tagName()=="ip")
                {
                    if(child_s.toElement().hasAttribute("value"))
                    {
                        this->_device_ip = child_s.toElement().attribute("value").toLongLong();
                    }
                }
                if(child_s.toElement().tagName()=="port")
                {
                    if(child_s.toElement().hasAttribute("value"))
                    {
                        this->_device_port = child_s.toElement().attribute("value").toInt();
                    }
                }
                child_s = child_s.nextSibling();
            }
        }
        child = child.nextSibling();
    }
    return element;
}

void NetAddrMsgDataBody::AttrFillXmlElement(QDomElement root){}
NetAddrMsgDataBody::NetAddrMsgDataBody()
{
    this->_data_type =eTransData_net_addr;
}

//文件数据
QDomElement FileMsgDataBody::Attr2XmlElement()
{
    QDomDocument doc;
    QDomElement data = doc.createElement("data");

    QDomElement type = doc.createElement("type");
    QDomAttr type_attr = doc.createAttribute("value");
    type_attr.setValue("file");
    type.setAttributeNode(type_attr);
    /*file 节点*/
    QDomElement file = doc.createElement("file");
    QDomElement name = doc.createElement("name");
    QDomElement path = doc.createElement("path");

    QDomAttr name_attr = doc.createAttribute("value");
    name_attr.setValue(QString::fromStdString(_file_name));
    name.setAttributeNode(name_attr);

    QDomAttr path_attr = doc.createAttribute("value");
    path_attr.setValue(QString::fromStdString(_file_path));
    path.setAttributeNode(path_attr);

    file.appendChild(name);
    file.appendChild(path);
    /*file 节点*/

    data.appendChild(type);
    data.appendChild(file);

    return data;
}

QDomElement FileMsgDataBody::XmlElement2Attr(QDomElement element)
{
    QDomNode child = element.firstChild();
    while(!child.isNull())
    {
        if(child.toElement().tagName()=="file")
        {
            QDomNode child_s = child.toElement().firstChild();
            while(!child_s.isNull())
            {
                if(child_s.toElement().tagName()=="name")
                {
                    if(child_s.toElement().hasAttribute("value"))
                    {
                        this->_file_name = child_s.toElement().attribute("value").toStdString();
                    }
                }
                if(child_s.toElement().tagName()=="path")
                {
                    if(child_s.toElement().hasAttribute("value"))
                    {
                        this->_file_path = child_s.toElement().attribute("value").toInt();
                    }
                }
                child_s = child_s.nextSibling();
            }
        }
        child = child.nextSibling();
    }
    return element;
}

void FileMsgDataBody::AttrFillXmlElement(QDomElement root)
{
    QDomNode child = root.firstChild();
    while(!child.isNull())
    {
        if(child.toElement().tagName()=="file")
        {
            QDomNode child_s = child.toElement().firstChild();
            while(!child_s.isNull())
            {
                if(child_s.toElement().tagName()=="name")
                {
                    if(child_s.toElement().hasAttribute("value"))
                    {
                        child_s.toElement().setAttribute("value",QString::fromStdString(this->_file_name));
                        //this->_file_name = child_s.toElement().attribute("value").toStdString();
                    }
                }
                if(child_s.toElement().tagName()=="path")
                {
                    if(child_s.toElement().hasAttribute("value"))
                    {
                        child_s.toElement().setAttribute("value",QString::fromStdString(this->_file_path));
                        //this->_file_path = child_s.toElement().attribute("value").toInt();
                    }
                }
                child_s = child_s.nextSibling();
            }
        }
        child = child.nextSibling();
    }
}
FileMsgDataBody::FileMsgDataBody()
{
    this->_data_type = eTransData_file;
}

//表格数据
QDomElement TableMsgDataBody::Attr2XmlElement()
{
    QDomDocument doc;
    QDomElement data = doc.createElement("data");

    QDomElement type = doc.createElement("type");
    QDomAttr type_attr = doc.createAttribute("value");
    type_attr.setValue("table");
    type.setAttributeNode(type_attr);
    /*table 节点*/
    QDomElement table = doc.createElement("table");
    QDomElement name = doc.createElement("name");

    QDomAttr name_attr = doc.createAttribute("value");
    name_attr.setValue(QString::fromStdString(this->_table_name));
    name.setAttributeNode(name_attr);

    table.appendChild(name);
    /*table 节点*/
    data.appendChild(type);
    data.appendChild(table);

    return data;
}

QDomElement TableMsgDataBody::XmlElement2Attr(QDomElement element){ return element;}

void TableMsgDataBody::AttrFillXmlElement(QDomElement root){}
TableMsgDataBody::TableMsgDataBody()
{
    this->_data_type = eTransData_table;
}

//变量数据
QDomElement VariableMsgDataBody::Attr2XmlElement()
{
    QDomDocument doc;
    QDomElement data = doc.createElement("data");

    QDomElement type = doc.createElement("type");
    QDomAttr type_attr = doc.createAttribute("value");
    type_attr.setValue("variable");
    type.setAttributeNode(type_attr);
    /*variable 节点*/
    QDomElement address = doc.createElement("variable");
    QDomElement name = doc.createElement("name");
    QDomElement type_s = doc.createElement("type");
    QDomElement value = doc.createElement("value");

    QDomAttr name_attr = doc.createAttribute("value");
    name_attr.setValue(QString::fromStdString(this->_var_name));
    name.setAttributeNode(name_attr);

    QDomAttr type_s_attr = doc.createAttribute("value");
    type_s_attr.setValue(QString::number(this->_var_type,10));
    type_s.setAttributeNode(type_s_attr);

    QDomAttr value_attr = doc.createAttribute("value");
    value_attr.setValue(QString::fromStdString(this->_var_value));
    value.setAttributeNode(value_attr);

    address.appendChild(name);
    address.appendChild(type_s);
    address.appendChild(value);
    /*variable 节点*/

    data.appendChild(type);
    data.appendChild(address);

    return data;
}

QDomElement VariableMsgDataBody::XmlElement2Attr(QDomElement element)
{
    QDomNode child = element.firstChild();
    while(!child.isNull())
    {
        if(child.toElement().tagName()=="variable")
        {
            QDomElement child_s = child.firstChildElement();

            while(!child_s.isNull())
            {
                if(child_s.tagName()=="name")
                {
                    if(child_s.hasAttribute("value"))
                    {
                        this->_var_name = child_s.attribute("value").toStdString();
                    }
                }
                if(child_s.tagName()=="type")
                {
                    if(child_s.hasAttribute("value"))
                    {
                        this->_var_type = child_s.attribute("value").toInt();
                    }
                }
                if(child_s.tagName()=="value")
                {
                    if(child_s.hasAttribute("value"))
                    {
                        this->_var_value = child_s.attribute("value").toStdString();
                    }
                }
                child_s = child_s.nextSiblingElement();
            }

        }
        child = child.nextSibling();
    }
    return element;
}

void VariableMsgDataBody::AttrFillXmlElement(QDomElement root)
{
    QDomNode child = root.firstChild();
    while(!child.isNull())
    {
        if(child.toElement().tagName()=="variable")
        {
            QDomNode child_s = child.toElement().firstChild();
            while(!child_s.isNull())
            {
                if(child_s.toElement().tagName()=="name")
                {
                    if(child_s.toElement().hasAttribute("value"))
                    {
                        child_s.toElement().setAttribute("value",QString::fromStdString(this->_var_name));
                        //this->_var_name = child_s.toElement().attribute("value").toStdString();
                    }
                }
                if(child_s.toElement().tagName()=="type")
                {
                    if(child_s.toElement().hasAttribute("value"))
                    {
                        child_s.toElement().setAttribute("value",QString::number(this->_var_type,10));
                        //this->_var_type = child_s.toElement().attribute("value").toInt();
                    }
                }
                if(child_s.toElement().tagName()=="value")
                {
                    if(child_s.toElement().hasAttribute("value"))
                    {
                        child_s.toElement().setAttribute("value",QString::fromStdString(this->_var_value));
                        //this->_var_value = child_s.toElement().attribute("value").toStdString();
                    }
                }
                child_s = child_s.nextSibling();
            }
        }
        child = child.nextSibling();
    }
}

VariableMsgDataBody::VariableMsgDataBody()
{
    this->_data_type = eTransData_variable;
}
