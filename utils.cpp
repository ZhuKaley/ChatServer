#include <sys/types.h>
#include <sys/socket.h>

#include "utils.h"

bool utils::create_socket_tcp(int& fd)
{
    fd = ::socket(AF_INET, SOCK_STREAM, 0);
    return fd != -1;
}

bool utils::create_socket_udp(int& fd)
{
    fd = ::socket(AF_INET, SOCK_DGRAM, 0);
    return fd != -1;
}