#ifndef __UTILS_H__
#define __UTILS_H__

class utils
{
public:
    static bool create_socket_tcp(int& fd);
    static bool create_socket_udp(int& fd);
};

#endif
