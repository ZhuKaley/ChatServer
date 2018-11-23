#ifndef __SERVER_H__
#define __SERVER_H__

class server
{
public:
    server();
    ~server();

    bool init();
    void deinit();

    bool run();
    bool stop();

};


#endif
