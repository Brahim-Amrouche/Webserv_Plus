/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LoadBalancer.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/25 14:24:24 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/13 22:07:58 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Socket.hpp"
#include "Client.hpp"
#include <deque>
#include <algorithm>

#define MAX_EVENTS SOMAXCONN

using std::deque;

typedef enum LOADBALANCER_ERR_CODES
{
    E_EPOLLINIT,
    E_EPOLLCTL,
    E_EPOLLWAIT,
    E_CREATE_CLIENT,
    E_LOADBALANCER_OUTOFMEM,
} loadbalancer_errors;

typedef deque<Client>::iterator ClientDeqIt;
typedef deque<ServerSocket>::iterator SrvSockDeqIt;

class LoadBalancer
{
    public:
        deque<ServerSocket> *listeners;
        int epoll_fd;
        EPOLL_EVENT events[MAX_EVENTS];
        int events_trigered;
        int load;
        deque<Client> clients;
    public :
        class LoadBalancerExceptions: public TException<loadbalancer_errors, LoadBalancer>
        {
            public:
                LoadBalancerExceptions(const loadbalancer_errors &err, LoadBalancer *cln);
                virtual const char *what() const throw()
                { 
                    return TException::what();
                };
                virtual ~LoadBalancerExceptions() throw() {};
        };
        LoadBalancer(deque<ServerSocket> *sockets);
        void loop();
        void handle_request();
        void add_client(int event_id, SrvSockDeqIt &server);
        ClientDeqIt find_client(SOCKET_ID &sock_id);
        SrvSockDeqIt find_server(SOCKET_ID &sock_id);
        void remove_client(ClientDeqIt &rm_cl);
        void check_timeouts();
        ~LoadBalancer();
};
