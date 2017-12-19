
#include "net_handler.h"

NetHandlerBase::NetHandlerBase()
{
    _rcv_handler = nullptr;
}

NetHandlerBase::~NetHandlerBase()
{

}

bool NetHandlerBase::start_service(const char* ip, uint16_t port, uint16_t dev_port, bool cli)
{
    return true;
}

bool NetHandlerBase::stop_service()
{
    return true;
}

bool NetHandlerBase::send_data(const char* data, int len, const char* ip, int port)
{
    return true;
}

bool NetHandlerBase::register_rcv_callback(RcvHandler handler)
{
    return true;
}
