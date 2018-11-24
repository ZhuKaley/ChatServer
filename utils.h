#ifndef __UTILS_H__
#define __UTILS_H__

class utils
{
public:
    static bool create_socket_tcp(int& sockfd);
    static bool create_socket_udp(int& sockfd);
	static uint32_t htonl(uint32_t host_long);
	static uint16_t htons(uint16_t host_short);
	static uint32_t ntohl(uint32_t net_long);
	static uint16_t ntohs(uint16_t net_short);
	static bool is_bigend();
	static bool set_nonblock(int& sockfd);
	static bool set_reuseaddr(int& sockfd);
};

#endif
