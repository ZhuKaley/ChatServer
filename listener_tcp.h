#ifndef __LISTENER_TCP_H__
#define __LISTENER_TCP_H__

#include "event_loop.h"

class listener_tcp
{
public:
    listener_tcp();
    ~listener_tcp();

    bool init();
    void deinit();

    bool start();
    bool stop();

private:
    static void on_accept(int fd, int ev_types, void *obj);

private:
    int m_listen_fd;
    event_loop *m_ev_loop;

    
};

#endif
