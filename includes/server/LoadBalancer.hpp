/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LoadBalancer.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/25 14:24:24 by bamrouch          #+#    #+#             */
/*   Updated: 2023/12/25 20:18:14 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Socket.hpp"

#define MAX_EVENTS SOMAXCONN

typedef enum LOADBALANCER_ERR_CODES
{
    E_EPOLLINIT,
    E_EPOLLCTL,
    E_EPOLLWAIT,
    E_CREATE_CLIENT,
    E_LOADBALANCER_OUTOFMEM,
} loadbalancer_errors;

class LoadBalancer
{
    public:
        Socket *listener;
        int epoll_fd;
        EPOLL_EVENT events[MAX_EVENTS];
        int events_trigered;
        int load;
    public :
        class LoadBalancerExceptions: public TException<loadbalancer_errors, LoadBalancer>
        {
            public:
                LoadBalancerExceptions(const loadbalancer_errors &err, const LoadBalancer *cln);
        };
        LoadBalancer(Socket *sock);
        void loop();
        void handle_request();
        void new_connection(int event_id);
        ~LoadBalancer();
};