#include <iostream>

#include <string.h>
#include <netinet/in.h>

#include "utils.h"
#include "listener_tcp.h"

listener_tcp::listener_tcp() :
    m_listen_fd(-1),
    m_ev_loop(NULL)
{

}

listener_tcp::~listener_tcp()
{
    deinit();
}

bool listener_tcp::init()
{
    m_ev_loop = new event_loop();
    if(!m_ev_loop || !m_ev_loop->init())
    {
        std::cout << "tcp listener init failed." << std::endl;
        return false;
    }

    return true;
}

void listener_tcp::deinit()
{
    if(m_ev_loop)
    {
        delete m_ev_loop;
        m_ev_loop = NULL;
    }
}

bool listener_tcp::start()
{
    if(!m_ev_loop)
    {
        std::cout << "event loop not exist." << std::endl;
        return false;
    }
    
    if(!utils::create_socket_tcp(m_listen_fd))
    {
    	std::cout << "create tcp socket failed." << std::endl;
        return false;
    }

	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = utils::htons(6666);
	server_addr.sin_addr.s_addr = ::htonl(INADDR_ANY);   //utils::htonl(INADDR_ANY);

	utils::set_nonblock(m_listen_fd);
	utils::set_reuseaddr(m_listen_fd);

	if(::bind(m_listen_fd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
	{
		std::cout << "bind failed." << std::endl;
        ::close(m_listen_fd);
        m_listen_fd = -1;
		return false;
	}

    if(::listen(m_listen_fd, SOMAXCONN) == -1)
    {
        std::cout << "listen failed." << std::endl;
        ::close(m_listen_fd);
        m_listen_fd = -1;
        return false;
    }

    event_info *ev_info = new event_info();
    ev_info->fd = m_listen_fd;
    ev_info->ev_types = EPOLLIN | EPOLLET;
    ev_info->obj = this;
    ev_info->ev_callback = &listener_tcp::on_accept;
    m_ev_loop->event_add(ev_info);

    std::cout << "tcp listener stating..." << std::endl;

    return m_ev_loop->run();
}

bool listener_tcp::stop()
{

}

void listener_tcp::on_accept(int fd, int ev_types, void *obj)
{
    std::cout << "on accept fd: " << fd << std::endl;
    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    socklen_t len = sizeof(client_addr);
    int client_fd = ::accept(fd, (struct sockaddr *)&client_addr, &len);
}


