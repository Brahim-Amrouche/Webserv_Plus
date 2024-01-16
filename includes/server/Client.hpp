/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 08:12:13 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/16 15:55:51 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Socket.hpp"
#include "Request.hpp"
#include "Response.hpp"

typedef enum CLIENT_ERR_CODES
{
    E_CLIENT_RECEIVE,
    E_CLIENT_SEND,
    E_CLIENT_CLOSED,
}   client_errors;

class Client
{
    private :
        char   buffer[HEADERS_MAX_SIZE + 1];
        Socket *client_socket;
        ServerSocket &server_socket;
        Request req;
        Response res;
        std::time_t last_activity;
        response_code err_code;
        bool        err_set;
        bool        close_socket;
    public :
        class ClientExceptions: public TException<client_errors, Client>
        {
            public:
                ClientExceptions(const client_errors &err, Client *cln);
                const char *what() const throw() 
                {
                    return TException::what();
                };
                ~ClientExceptions() throw()
                {};
        };
        // Client();
        Client(Socket *cl_sock, ServerSocket &server_sock);
        Client(const Client &cpy_cl);
        Socket    *getSocket() const;
        SOCKET_ID getSocketId() const;
        void      receive();
        void      send();
        void      nullify();
        void      setLastActivity();
        void      checkTimeout();
        void      setCloseSocket(const bool &c)
        {
            close_socket = c;
        };
        
        Client    &operator=(const Client &eq_cl);
        bool      operator==(SOCKET_ID &sock_id);
        ~Client();
};