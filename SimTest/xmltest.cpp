#include "header_test.h"
#include "xml_message.h"
#include <QDomDocument>
#include <QFile>
#include <QTextStream>
#include <iostream>
//
void Attr2Xml()
{
    QFile file("123.xml");  //写xml文件
       if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
           return ;
    QDomDocument doc;
    QTextStream out(&file);
    //QDomProcessingInstruction instruction = doc.createProcessingInstruction( "xml", "version = \'1.0\' encoding=\'UTF-8\'" );
    //doc.appendChild( instruction );
    QDomElement root = doc.createElement( "ROOT" );
    doc.appendChild(root);

    ApplStepMsgBody app;
    app._step_num = 2;
    app._step_vec.push_back(1);
    app._step_vec.push_back(2);
    app._proc_name_vec.push_back("a");
    app._proc_name_vec.push_back("b");

    QDomElement element = app.Attr2XmlElement();
    root.appendChild(element);
    doc.save(out, 4);
}

void Xml2Attr()
{
    //std::cout<<"555555555555555555"<<endl;
    RootMessageBody app;
    app._session_msg_body = new SessionMessageBody();
    app._session_msg_body->_procedure_msg_body =new ProcedureMessageBody();
    QFile file("F:\\work2\\bin\\debug\\invoke.xml");
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
       //QMessageBox::information(NULL, QString("title"), QString("open error!"));
       return;
    }
    QDomDocument document;
    QString error;
    int row = 0, column = 0;
    if(!document.setContent(&file, false, &error, &row, &column))
    {
       //QMessageBox::information(NULL, QString("title"), QString("parse file failed at line row and column") + QString::number(row, 10) + QString(",") + QString::number(column, 10));
       return;
    }
    if(document.isNull())
    {
       //QMessageBox::information(NULL, QString("title"), QString("document is null!"));
       return;
    }
    QDomElement root = document.documentElement();

    QDomElement steps = root.firstChildElement();
    if(steps.isNull())
    {
        return;
    }
    app.XmlElement2Attr(root);
//    std::cout<<app._step_num;
//    std::cout<<app._proc_name_vec[0];
//    std::cout<<app._proc_name_vec[1];
//    std::cout<<app._step_vec[0];
//    std::cout<<app._step_vec[1];
}

void Attr2Xml_app()
{
    QFile file("1234mess.xml");  //写xml文件
       if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
           return ;
    QDomDocument doc;
    QTextStream out(&file);
    //QDomProcessingInstruction instruction = doc.createProcessingInstruction( "xml", "version = \'1.0\' encoding=\'UTF-8\'" );
    //doc.appendChild( instruction );
    QDomElement root = doc.createElement( "ROOT" );
    doc.appendChild(root);
    /*app*/
    ApplMessageBody appl;
    appl._appl_name = "app";
    appl._appl_step = 2;
    appl._msg_step = new ApplStepMsgBody();
    appl._msg_step->_step_num = 2;
    appl._msg_step->_step_vec.push_back(1);
    appl._msg_step->_step_vec.push_back(2);
    appl._msg_step->_proc_name_vec.push_back("a");
    appl._msg_step->_proc_name_vec.push_back("b");

    /*progress*/
    ProcedureMessageBody progress;
    progress._appl_msg_body = &appl;
    progress._msg_name = "app  request";
    progress._proc_type = 20;
    //
    VariableMsgDataBody va;
    va._var_name ="real time";
    va._var_type = 10;
    va._var_value = "2017.235";
    //
    VariableMsgDataBody vb;
    vb._var_name ="clientAddr";
    vb._var_type = 10;
    vb._var_value = "172.0.0.1 600";
    //
    VariableMsgDataBody vc;
    vc._var_name ="type;";
    vc._var_type = 10;
    vc._var_value = "172.0.0.1 600";
    //
    VariableMsgDataBody vd;
    vd._var_name ="length";
    vd._var_type = 10;
    vd._var_value = "172.0.0.1 600";
    //
    VariableMsgDataBody ve;
    ve._var_name ="nodeId";
    ve._var_type = 10;
    ve._var_value = "value";
    //
    VariableMsgDataBody vf;
    vf._var_name ="simu time";
    vf._var_type = 10;
    vf._var_value = "2017.235";
    progress._data_vector.push_back(&va);
    progress._data_vector.push_back(&vb);
    progress._data_vector.push_back(&vc);
    /*session*/
    SessionMessageBody session;
    session._id_i2u =12;
    session._id_u2i =12;
    session._procedure_msg_body = &progress;

    /*message*/
    RootMessageBody message;
    message._session_msg_body = &session;

    QDomElement element = message.Attr2XmlElement();

    root.appendChild(element);
    doc.save(out, 4);
}
