#ifndef NET_HANDLER_H
#define NET_HANDLER_H

#include <QObject>
#include "comm_type_def.h"

class NetHandlerBase : public QObject
{
    Q_OBJECT
public:
    NetHandlerBase();
    virtual ~NetHandlerBase();

public:
    /**
  * @brief 启动网络传输功能
  * @param ip:服务器本机或远程IP
  * @param port: 服务器端口
  * @parame cli: 是否作为客户端启动，若作为客户端，参数ip指远端服务器IP
  * @return true/false: 成功/失败
  */
    virtual bool start_service(const char* ip, uint16_t port, uint16_t dev_port, bool cli = true);

    /**
  * @brief 停止网络传输功能
  * @return  true/false: 成功/失败
  */
    virtual bool stop_service();

    /**
  * @brief 发送数据
  * @param data: 数据体，按字节存储
  * @parame len: 数据长度
  * @param ip: 目的端ip
  * @return true/false: 成功/失败
  */
    virtual bool send_data(const char* data, int len, const char* ip = nullptr, int port = 0);

    /**
  * @brief 设置数据接收回调函数
  * @param handler: 回调函数指针
  * @return true/false: 成功/失败
  */
    virtual bool register_rcv_callback(RcvHandler handler);

protected:
    RcvHandler _rcv_handler;
};

#endif // NET_HANDLER_H
