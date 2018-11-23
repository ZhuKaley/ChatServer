#ifndef __LISTENER_TCP_H__
#define __LISTENER_TCP_H__

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
    int m_listen_fd;

    
};

#endif
