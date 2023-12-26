/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 08:12:13 by bamrouch          #+#    #+#             */
/*   Updated: 2023/12/26 16:02:17 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Socket.hpp"

typedef enum CLIENT_ERR_CODES
{
    E_CLIENT_RECEIVE,
    E_CLIENT_SEND,
}   client_errors;

class Client
{
    private :
        Socket *socket;
    public :
        class ClientExceptions: TException<client_errors, Client>
        {
            public:
                ClientExceptions(const client_errors &err, Client *cln);
                const char *what() const throw();
        };
        Client();
        Client(Socket *cl_sock);
        Client(const Client &cpy_cl);
        Socket    *getSocket() const;
        SOCKET_ID getSocketId() const;
        void      setSocket(Socket *new_sock);
        void      receive();
        void      send();
        Client    &operator=(const Client &eq_cl);
        bool      operator==(SOCKET_ID &sock_id);
        ~Client();
};