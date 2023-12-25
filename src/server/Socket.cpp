/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/25 12:36:29 by bamrouch          #+#    #+#             */
/*   Updated: 2023/12/25 19:52:35 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

Socket::SocketExceptions::SocketExceptions(const socket_errors &err, const Socket *cleanup):TException("Socket Error: ",err, cleanup)
{
    switch (err_c)
    {
        case E_ADDRESS_LOOKUP:
            msg += "Address Lookup Failed";
            break;
        case E_SOCKET_OPEN:
            msg += "Openning New Socket Failed";
            break;
        case E_SOCKET_BIND_FAILED:
            msg += "Binding Socket Failed";
            break;
        case E_LISTEN_FAILED:
            msg += "Listening To Socket Failed";
            break;
        case E_SOCKET_ACCEPT:
            msg += "Accepting New Socket Failed";
            break;
        default:
            msg += "Unknown error";    
    }
}

Socket::Socket():sock_id(-1), sock_addr_len(0)
{
    FT::memset(&(sock_addr), 0, sizeof(SOCK_ADDR_STORAGE));
}

Socket::Socket(const char *host, const char *port)
{
    ADDRESS_INFO hints;
    FT::memset(&hints, 0, sizeof(ADDRESS_INFO));
    hints.ai_family = IPV4_AND_6;
    hints.ai_socktype = TCP_SOCK;
    hints.ai_flags = LISTEN_ON_HOST_NET;

    ADDRESS_INFO *bind_address = NULL;
    cout << "Looking for address info..." << endl;
    int dns_lookup_res = getaddrinfo(host, port, &hints, &bind_address);
    if (dns_lookup_res > 0)
        throw Socket::SocketExceptions(E_ADDRESS_LOOKUP, NULL);
    FT::memcpy(&sock_addr, bind_address->ai_addr, sizeof(SOCK_ADDR_STORAGE));
    sock_addr_len = bind_address->ai_addrlen;
    cout << "Address info found" << endl;
    cout << "Openning new TCP socket ..." << endl;
    sock_id = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);
    freeaddrinfo(bind_address);
    if (!ISVALIDSOCKET(sock_id))
        throw Socket::SocketExceptions(E_SOCKET_OPEN, this);
    cout << "Socket successfully opened" << endl;
}

void Socket::sockBind() const
{
    cout << "Binding Socket" << endl;
    if (bind(sock_id, &sock_addr, sock_addr_len))
        throw Socket::SocketExceptions(E_SOCKET_BIND_FAILED, this);
    cout << "Binding successfull" << endl;
}

void Socket::sockListen() const
{
    cout << "Attempting to list on socket" << endl;
    if (listen(sock_id, SOMAXCONN))
        throw Socket::SocketExceptions(E_LISTEN_FAILED, this);
    cout << "Socket listening" << endl;
}

Socket *Socket::sockAccept() const
{
    cout << "Accepting a new connection" << endl;
    Socket *client_socket = new Socket();
    if (!client_socket)
        throw Socket::SocketExceptions(E_SOCKET_ACCEPT, this);
    client_socket->sock_id = accept(sock_id, 
        &(client_socket->sock_addr), &(client_socket->sock_addr_len));
    if (!ISVALIDSOCKET(client_socket->sock_id))
        throw Socket::SocketExceptions(E_SOCKET_ACCEPT, client_socket);
    cout << "Client socket accepted" << endl;
    return client_socket;   
}

void Socket::fill_epoll_event(EPOLL_EVENT *e_event, uint32_t mode) const
{
    e_event->events = mode;
    e_event->data.fd = sock_id;
}

SOCKET_ID Socket::getSockid() const
{
    return sock_id;
}

Socket::~Socket()
{
    if (ISVALIDSOCKET(sock_id))
        close(sock_id);
}
