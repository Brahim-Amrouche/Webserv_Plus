/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 08:12:04 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/13 23:08:56 by bamrouch         ###   ########.fr       */
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
        case E_CLIENT_CLOSED:
            msg += "Client Closed Socket";
            break;
        default :
            msg += "Unknown Error";
    }
}

Client::Client(Socket *cl_sock, ServerSocket &s_sock): client_socket(cl_sock), 
    server_socket(s_sock), req(buffer, *cl_sock, s_sock), res(buffer, req), err_code(RES_NONE)
{
    setLastActivity();
    cout << "New Client Created from socket" << endl; 
}

Client::Client(const Client &cpy_cl): client_socket(cpy_cl.client_socket), server_socket(cpy_cl.server_socket)
    , req(buffer, *cpy_cl.client_socket,  server_socket) , res(buffer, req)
    , last_activity(cpy_cl.last_activity) , err_code(cpy_cl.err_code)
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
        setLastActivity();
        req.read();
    }
    catch(const Request::RequestException &e)
    {
        switch (e.err_c)
        {
            case E_REQUEST_ERROR_400:
                err_code = RES_BAD_REQUEST;
                break;
            case E_REQUEST_ERROR_405:
                err_code = RES_METHOD_NOT_ALLOWED;
                break;
            case E_REQUEST_ERROR_500:
                err_code = RES_INTERNAL_SERVER_ERROR;
                break;
            default:
                err_code = RES_BAD_REQUEST;   
                break;
        }  
    }
}

void    Client::send()
{
    try
    {
        if (err_code != RES_NONE)
        {
            res.serveError(err_code);
            res >> *client_socket;
        }
        setLastActivity();
        if (req.getServerConfig() && req.getBodyDone())
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

void Client::setLastActivity()
{
    last_activity = std::time(NULL);
}

bool Client::checkTimeout()
{
    if (std::time(NULL) - last_activity >= MAX_TIMEOUT)
        return true;
    return false;
}

Client::~Client()
{
    if (client_socket)
        delete client_socket;
}


