#include <iostream>

#include "event_loop.h"
#include "listener_tcp.h"

int main()
{
    //std::cout << "hello chatserver." << std::endl;
    listener_tcp listener;
    if(!listener.init())
    {
        std::cout << "listener init failed." << std::endl;
        return -1;
    }

    if(!listener.start())
    {
        std::cout << "listener start failed." << std::endl;
        return -1;
    }
    
    #if 0
    event_loop ev_loop;
    if(!ev_loop.init())
    {
        return -1;
    }

    ev_loop.run();
    #endif
    return 0;
}
