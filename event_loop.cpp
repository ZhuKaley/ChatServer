#include <fcntl.h>
#include <unistd.h>
#include <sys/epoll.h>

#include <string>

#include "event_loop.h"

event_loop::event_loop() :
    m_epfd(-1)
{

}

event_loop::~event_loop()
{

}

bool event_loop::init()
{
    m_epfd = ::epoll_create1(EPOLL_CLOEXEC);
    if(m_epfd == -1)
    {
        return false;
    }

    if(pipe2(m_pipefd, O_NONBLOCK | O_CLOEXEC) == -1)
    {
        close(m_epfd);
        return false;
    }

    event_add(m_pipefd[0], EPOLLIN | EPOLLET);

    return true;
}

void event_loop::deinit()
{
    if(m_epfd >= 0)
    {
        close(m_epfd);
    }

    close(m_pipefd[0]);
    close(m_pipefd[1]);
}

bool event_loop::run()
{
    if(m_epfd < 0)
    {
        return false;
    }

    
}

bool event_loop::stop()
{
    if(m_pipefd[1] < 0)
    {
        return false;
    }

    std::string msg = "c";
    return write(m_pipefd[0], msg.c_str(), 1) ==  1;
}

bool event_loop::event_add(const int fd, const uint32_t ev_types)
{
    if(m_epfd < 0 || fd < 0)
    {
        return false;
    }

    struct epoll_event ev;
    ev.events = ev_types;
    ev.data.fd = fd;

    return ::epoll_ctl(m_epfd, EPOLL_CTL_ADD, fd, &ev) == 0;
}

bool event_loop::event_mod(const int fd, const uint32_t ev_types)
{
    if(m_epfd < 0 || fd < 0)
    {
        return false;
    }

    struct epoll_event ev;
    ev.events = ev_types;
    ev.data.fd = fd;

    return ::epoll_ctl(m_epfd, EPOLL_CTL_MOD, fd, &ev) == 0;
}

bool event_loop::event_del(const int fd)
{
    if(m_epfd < 0 || fd < 0)
    {
        return false;
    }

    struct epoll_event ev;
    ev.data.fd = fd;

    return ::epoll_ctl(m_epfd, EPOLL_CTL_DEL, fd, &ev) == 0;
}

