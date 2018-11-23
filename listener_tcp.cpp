#include "utils.h"
#include "listener_tcp.h"

listener_tcp::listener_tcp() :
    m_listen_fd(-1)
{

}

listener_tcp::~listener_tcp()
{
    deinit();
}

bool listener_tcp::init()
{

}

void listener_tcp::deinit()
{

}

bool listener_tcp::start()
{
    if(!utils::create_socket_tcp(m_listen_fd))
    {
        return false;
    }

    
}

bool listener_tcp::stop()
{

}

