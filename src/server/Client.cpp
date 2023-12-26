/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 08:12:04 by bamrouch          #+#    #+#             */
/*   Updated: 2023/12/26 16:03:02 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

Client::ClientExceptions::ClientExceptions(const client_errors &err, Client *cln): TException("Client Error: ", err, cln)
{
    switch (err_c)
    {
        case E_CLIENT_RECEIVE:
            msg += "Client Receive Failed";
            break;
        case E_CLIENT_SEND:
            msg += "Client Send Failed";
            break;
        default :
            msg += "Unknown Error";
    }
}

Client::Client():socket(NULL)
{}

Client::Client(Socket *cl_sock): socket(cl_sock)
{
    cout << "New Client Created from socket" << endl; 
}

Client::Client(const Client &cpy_cl)
{
    this->operator=(cpy_cl);
}

Socket *Client::getSocket() const
{
    return socket;
}

SOCKET_ID Client::getSocketId() const
{
    return socket->getSockid();
}

void    Client::setSocket(Socket *new_sock)
{
    socket = new_sock;
}

void    Client::receive()
{
    cout << "Receiving" << endl;
}

void    Client::send()
{
    cout << "sending" << endl;
}

Client &Client::operator=(const Client &eq_cl)
{
    if (&eq_cl == this)
        return *this;
    socket = eq_cl.socket;
    return *this;
}

bool    Client::operator==(SOCKET_ID &sock_id)
{
    if (getSocketId() == sock_id)
        return true;
    return false;
}

Client::~Client()
{
    if (socket)
        delete socket;
}


