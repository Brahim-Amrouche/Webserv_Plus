/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 08:12:04 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/10 15:25:42 by bamrouch         ###   ########.fr       */
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

Client::Client(Socket *cl_sock, ServerSocket &s_sock): client_socket(cl_sock), server_socket(s_sock), req(buffer, *cl_sock, s_sock), res(buffer, req)
{
    cout << "New Client Created from socket" << endl; 
}

Client::Client(const Client &cpy_cl): client_socket(cpy_cl.client_socket), server_socket(cpy_cl.server_socket)
    , req(buffer, *cpy_cl.client_socket,  server_socket) , res(buffer, req)
{}

Socket *Client::getSocket() const
{
    return client_socket;
}

SOCKET_ID Client::getSocketId() const
{
    return client_socket->getSockid();
}

void    Client::receive()
{
    try
    {
        req.read();
    }
    catch(const Request::RequestException &e)
    {
        cout << e.what() << endl;
        throw ClientExceptions(E_CLIENT_RECEIVE, NULL);
    }
}

void    Client::send()
{
    try
    {
        if (req.getServerConfig())
            res >> *client_socket;
    }
    catch(const Response::ResponseException &e)
    {
        cout << e.what() << endl;
        throw ClientExceptions(E_CLIENT_SEND, NULL);
    }
}

Client &Client::operator=(const Client &eq_cl)
{
    if (&eq_cl != this)
    {
        client_socket = eq_cl.client_socket;
        server_socket = eq_cl.server_socket;
        // req = eq_cl.req;
    }
    return *this;
}

bool    Client::operator==(SOCKET_ID &sock_id)
{
    if (getSocketId() == sock_id)
        return true;
    return false;
}

void Client::nullify()
{
    client_socket = NULL;
}

Client::~Client()
{
    if (client_socket)
        delete client_socket;
}


