#ifndef __EVENT_LOOP_H__
#define __EVENT_LOOP_H__

class event_loop
{
public:
    event_loop();
    ~event_loop();

    bool init();
    void deinit();

    bool run();
    bool stop();

    bool event_add(const int fd, const uint32_t ev_types);
    bool event_mod(const int fd, const uint32_t ev_types);
    bool event_del(const int fd);

private:
    int m_epfd;
    int m_pipefd[2];


};

#endif
