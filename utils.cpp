#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "utils.h"

bool utils::create_socket_tcp(int& sockfd)
{
    sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
    return sockfd >= 0;
}

bool utils::create_socket_udp(int& sockfd)
{
    sockfd = ::socket(AF_INET, SOCK_DGRAM, 0);
    return sockfd >= 0;
}

uint32_t utils::htonl(uint32_t host_long)
{
	if(is_bigend())
	{
		return host_long;
	}

	return ::htonl(host_long);
}

uint16_t utils::htons(uint16_t host_short)
{
	if(is_bigend())
	{
		return host_short;
	}

	return ::htons(host_short);
}

uint32_t utils::ntohl(uint32_t net_long)
{
	if(is_bigend())
	{
		return net_long;
	}

	return ::ntohl(net_long);
}

uint16_t utils::ntohs(uint16_t net_short)
{
	if(is_bigend())
	{
		return net_short;
	}

	return ::ntohs(net_short);
}
#include <iostream>
bool utils::is_bigend()
{
	union
	{
		short a;
		char c;
	} un;

	un.a = 0x0102;

	return un.c == 0x01;	//从低位到高位，先存高位，为大端
}

bool utils::set_nonblock(int& sockfd)
{
	int flags = ::fcntl(sockfd, F_GETFD);
	if(flags == -1)
	{
		return false;
	}
	
	flags |= O_NONBLOCK;
	return ::fcntl(sockfd, F_SETFD, flags) != -1;
}

bool utils::set_reuseaddr(int& sockfd)
{
	int on = 1;
	return ::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == 0;
}