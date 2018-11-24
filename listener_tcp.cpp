#include <string.h>
#include <netinet/in.h>

#include "utils.h"
#include "listener_tcp.h"

listener_tcp::listener_tcp() :
    m_listen_fd(-1)
{

}

listener_tcp::~listener_tcp()
{
    deinit();
}

bool listener_tcp::init()
{

}

void listener_tcp::deinit()
{

}

bool listener_tcp::start()
{
    if(!utils::create_socket_tcp(m_listen_fd))
    {
    	std::cout << "create tcp socket failed." << std::endl;
        return false;
    }

	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = utils::htons(6666);
	server_addr.sin_addr.s_addr = utils::htonl(INADDR_ANY);

	utils::set_nonblock(m_listen_fd);
	utils::set_reuseaddr(m_listen_fd);

	if(::bind(m_listen_fd, &server_addr, sizeof(server_addr)) == -1)
	{
		std::cout << "bind failed." << std::endl;
		return false;
	}

	
    
}

bool listener_tcp::stop()
{

}

