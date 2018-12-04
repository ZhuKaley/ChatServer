#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <string>
#include <iostream>

#include "event_loop.h"
#define MAX_EVENTS 20

event_loop::event_loop() :
    m_runnable(true),
    m_epfd(-1)
{

}

event_loop::~event_loop()
{
    deinit();
}

bool event_loop::init()
{
    m_epfd = ::epoll_create1(EPOLL_CLOEXEC);
    if(m_epfd == -1)
    {
        return false;
    }

    ::socketpair(AF_UNIX, SOCK_STREAM, 0, m_pairfd);

    event_info *ev_info = new event_info();
    ev_info->fd = m_pairfd[0];
    ev_info->ev_types = EPOLLIN | EPOLLET;
    ev_info->obj = this;
    ev_info->ev_callback = &event_loop::on_stop;
    
    event_add(ev_info);
    
    m_evs.resize(MAX_EVENTS);

    return true;
}

void event_loop::deinit()
{
    std::map<const int, event_info*>::iterator it;
    for(it = m_evs_info.begin(); it != m_evs_info.end(); it++)
    {
        ev_info_del(it->first);
    }
    
    if(m_epfd >= 0)
    {
        close(m_epfd);
    }

    close(m_pairfd[0]);
    close(m_pairfd[1]);
}

bool event_loop::run()
{
    if(m_epfd < 0)
    {
        return false;
    }

    while(m_runnable)
    {
        //test
        #if 0
        static int i = 0;
        i++;
        std::cout << "i: " << i << std::endl;
        
        if(i > 10)
        {
            std::cout << "stop." << std::endl;
            stop();
        }
        #endif
        //end test
        
        int ret = ::epoll_wait(m_epfd, &*m_evs.begin(), m_evs.capacity(), 1000);
        if(ret > 0)
        {
            handle_events(ret);
        }
        else
        {
            continue;
        }
        
        if(ret == m_evs.capacity())
        {
            m_evs.resize(ret * 2);
        }
    }
}

bool event_loop::stop()
{
    if(m_pairfd[1] < 0)
    {
        std::cout << "stop failed." << std::endl;
        return false;
    }

    std::string msg = "c";
    return ::send(m_pairfd[1], msg.c_str(), 1, 0) == 1;
}

bool event_loop::event_add(event_info* ev_info)
{
    if(!ev_info)
    {
        return false;
    }

    struct epoll_event event;
    event.data.fd = ev_info->fd;
    event.data.ptr = (void*)ev_info;
    event.events = ev_info->ev_types;

    m_evs_info.insert(std::pair<const int, event_info*>(ev_info->fd, ev_info));

    return ::epoll_ctl(m_epfd, EPOLL_CTL_ADD, ev_info->fd, &event) == 0;
}

bool event_loop::event_mod(event_info* ev_info)
{
    if(!ev_info)
    {
        return false;
    }

    ev_info_del(ev_info->fd);
	
    struct epoll_event event;
    event.events = ev_info->ev_types;
    event.data.fd = ev_info->fd;
    event.data.ptr = (void*)ev_info;
	
    m_evs_info.insert(std::pair<const int, event_info*>(ev_info->fd, ev_info));

    return ::epoll_ctl(m_epfd, EPOLL_CTL_MOD, ev_info->fd, &event) == 0;
}

bool event_loop::event_del(const int fd)
{
    if(m_epfd < 0 || fd < 0)
    {
        return false;
    }

    ev_info_del(fd);

    struct epoll_event ev;
    ev.data.fd = fd;
    
    return ::epoll_ctl(m_epfd, EPOLL_CTL_DEL, fd, &ev) == 0;
}

void event_loop::on_stop(const int fd, const int ev_types, void *obj)
{
    if(!obj)
    {
        return;
    }    
    
    if(fd >= 0)
    {
        char msg[1];
        ::recv(fd, msg, 1, 0);
    }
	
	event_loop *ev_loop = (event_loop *)obj;
    ev_loop->m_runnable = false;
}

void event_loop::handle_events(const int ev_count)
{    
    for(int i = 0; i < ev_count; i++)
    {
        event_info * ev = (event_info*)m_evs[i].data.ptr;
        ev->ev_callback(ev->fd, ev->ev_types, ev->obj);
    }
}

void event_loop::ev_info_del(int fd)
{
    std::map<const int, event_info *>::iterator it = m_evs_info.find(fd);
    if(it != m_evs_info.end())
    {
        delete it->second;
        it->second = NULL;
        
        m_evs_info.erase(it);
    }
}



