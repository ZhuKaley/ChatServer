#ifndef __EVENT_LOOP_H__
#define __EVENT_LOOP_H__

#include <map>
#include <vector>
#include <functional>
#include <sys/epoll.h>

struct event_info
{
    int fd;
    int ev_types;
    void *obj;
    void(*ev_callback)(const int/*fd*/, const int/*ev_types*/, void *obj/*class*/);

    event_info() :
        fd(-1),
        ev_types(0)
    {

    }
};

class event_loop
{
public:
    event_loop();
    ~event_loop();

    bool init();
    void deinit();

    bool run();
    bool stop();

    bool event_add(event_info* ev_info);
    bool event_mod(event_info* ev_info);
    bool event_del(const int fd);

private:
    static void* on_start(void *obj);
    static void on_stop(const int fd, const int ev_types, void *obj);
    void handle_events(const int ev_count);
    void ev_info_del(int fd);

private:
    bool m_runnable;
    int m_epfd;
    int m_pairfd[2];
    std::map<const int, event_info *> m_evs_info;
    std::vector<struct epoll_event> m_evs;
};

#endif
