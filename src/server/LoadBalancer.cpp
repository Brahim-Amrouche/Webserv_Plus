/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LoadBalancer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/25 14:24:00 by bamrouch          #+#    #+#             */
/*   Updated: 2023/12/25 20:24:06 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

LoadBalancer::LoadBalancerExceptions::LoadBalancerExceptions(const loadbalancer_errors &err, const LoadBalancer *cln): TException("LoadBalancer Error: ", err, cln)
{
    switch (err_c)
    {
        case E_EPOLLINIT:
            msg += "Couldn't create an epoll instance";
            break;
        case E_EPOLLCTL:
            msg += "Couldn't add new event listener";
            break;
        case E_EPOLLWAIT:
            msg += "Epoll wait failed";
            break;
        case E_CREATE_CLIENT:
            msg += "Couldn't add a new client";
            break;
        case E_LOADBALANCER_OUTOFMEM:
            break;
        default:
            msg += "Unknown Error";
    }
}

LoadBalancer::LoadBalancer(Socket *sock):listener(sock), events_trigered(0) ,load(0)
{
    cout << "Initiating The Load Balancer" << endl;
    epoll_fd = epoll_create(1);
    if (epoll_fd = -1)
        throw LoadBalancer::LoadBalancerExceptions(E_EPOLLINIT, this);
    FT::memset(&(events[0]), 0, sizeof(EPOLL_EVENT));
    listener->fill_epoll_event(&(events[0]), EPOLLIN);
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listener->getSockid(), &(events[0])) == -1)
        throw LoadBalancer::LoadBalancerExceptions(E_EPOLLCTL, this);
    ++load;
    FT::memset(events, 0, sizeof(EPOLL_EVENT) * MAX_EVENTS);
    cout << "Load Balancer Initialized" << endl;
}

void LoadBalancer::loop()
{
    while (true)
    {
        FT::memset(events, 0, sizeof(EPOLL_EVENT) * load);
        events_trigered = 0;
        events_trigered = epoll_wait(epoll_fd, events, load, -1);
        if (events_trigered == -1)
            throw LoadBalancer::LoadBalancerExceptions(E_EPOLLWAIT, this);
        if (events_trigered > 0)
            handle_request();
        else
            cout << "listening..." << endl;
    }
}

void LoadBalancer::handle_request()
{
    int i = -1;
    while (++i < events_trigered)
    {
        if (events[i].data.fd == listener->getSockid())
        {
            cout << "making new connection" << endl;
            if (events[i].events & EPOLLIN)
                new_connection(i);
        }
    }
}
