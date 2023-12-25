/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/25 10:10:24 by bamrouch          #+#    #+#             */
/*   Updated: 2023/12/25 19:52:21 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <cstdlib>
#include <csignal>
#include "helpers.hpp"

#define IPV4_AND_6 AF_UNSPEC
#define TCP_SOCK SOCK_STREAM
#define LISTEN_ON_HOST_NET AI_PASSIVE
#define LISTEN_COUNT 1000

typedef int SOCKET_ID;
typedef addrinfo ADDRESS_INFO;
typedef sockaddr SOCK_ADDR_STORAGE;
typedef socklen_t SOCK_ADDR_LEN;
typedef epoll_event EPOLL_EVENT;

#define ISVALIDSOCKET(s) (s >= 0)

typedef enum SOCKET_ERR_CODES
{
    E_ADDRESS_LOOKUP,
    E_SOCKET_OPEN,
    E_SOCKET_BIND_FAILED,
    E_LISTEN_FAILED,
    E_SOCKET_ACCEPT
} socket_errors;

using std::cout;
using std::endl;

class Socket
{
    private :
        SOCKET_ID sock_id;
        SOCK_ADDR_STORAGE sock_addr;
        SOCK_ADDR_LEN   sock_addr_len;
    public :
        class SocketExceptions : TException<socket_errors, Socket>
        {
            public :
                SocketExceptions(const socket_errors &err, const Socket *cleanup);
        };
        Socket();
        Socket(const char *host, const char *port);
        void sockBind() const;
        void sockListen() const;
        Socket *sockAccept() const;
        void    fill_epoll_event(EPOLL_EVENT *e_event, uint32_t mode) const;
        SOCKET_ID getSockid() const;
        ~Socket();
};