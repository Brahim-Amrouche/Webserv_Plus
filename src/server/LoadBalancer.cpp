/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LoadBalancer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/25 14:24:00 by bamrouch          #+#    #+#             */
/*   Updated: 2023/12/28 03:56:30 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "TException.hpp"

LoadBalancer::LoadBalancerExceptions::LoadBalancerExceptions(const loadbalancer_errors &err, LoadBalancer *cln): TException("LoadBalancer Error: ", err, cln)
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
        default:
            msg += "Unknown Error";
    }
}

LoadBalancer::LoadBalancer(Socket *sock):listener(sock), epoll_fd(-1), events_trigered(0) ,load(0)
{
    cout << "Initiating The Load Balancer" << endl;
    if ((epoll_fd = epoll_create(1)) < 0)
        throw LoadBalancer::LoadBalancerExceptions(E_EPOLLINIT, NULL);
    FT::memset(events, 0, sizeof(EPOLL_EVENT));
    listener->fill_epoll_event(events, EPOLLIN);
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listener->getSockid(), events) == -1)
    {
        close(epoll_fd);
        delete listener;
        throw LoadBalancer::LoadBalancerExceptions(E_EPOLLCTL, NULL);
    }
    ++load;
    FT::memset(events, 0, sizeof(EPOLL_EVENT) * MAX_EVENTS);
    cout << "Load Balancer Initialized" << endl;
}

void LoadBalancer::loop()
{
    while (true)
    {   
        FT::memset(events, 0, sizeof(EPOLL_EVENT) * load);
        events_trigered = epoll_wait(epoll_fd, events, load, -1);
        if (events_trigered < 0)
            throw LoadBalancer::LoadBalancerExceptions(E_EPOLLWAIT, this);
        else if (events_trigered > 0)
            handle_request();
        else
            cout << "listening..." << endl;
    }
}

void LoadBalancer::handle_request()
{
    int i = -1;
    ClientDeqIt cl_it = clients.end();
    int event_fd = -1;
    while (++i < events_trigered)
    {
        cl_it = clients.end();
        event_fd =  events[i].data.fd;  
        if (event_fd == listener->getSockid())
        {
            if (events[i].events & EPOLLIN)
                add_client(i);
        }
        else if ((cl_it = find_client(event_fd)) != clients.end())
        {
            try
            {
                if (events[i].events & EPOLLIN)
                    cl_it->receive();
                else if (events[i].events & EPOLLOUT)
                    cl_it->send();
            }
            catch (const Client::ClientExceptions &e)
            {
                cout << e.what() << endl;
                clients.erase(cl_it);
            }
        }
    }
}

void LoadBalancer::add_client(int event_id)
{
    Socket *new_client_sock = listener->sockAccept();
    Client tmp_client(new_client_sock);
    clients.push_back(tmp_client);
    tmp_client.setSocket(NULL);
    FT::memset(&(events[event_id]), 0, sizeof(EPOLL_EVENT));
    new_client_sock->fill_epoll_event(&(events[event_id]), EPOLLIN | EPOLLOUT);
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_client_sock->getSockid(), &(events[event_id])) == -1)
        throw LoadBalancer::LoadBalancerExceptions(E_EPOLLCTL, this);
    ++load;
    cout << "New Client Added" << endl;
}

ClientDeqIt LoadBalancer::find_client(SOCKET_ID &sock_id)
{
    for (ClientDeqIt it = clients.begin(); it != clients.end(); it++)
    {
        if (it->getSocketId() == sock_id)
            return it;
    }
    return clients.end();
}

void    LoadBalancer::remove_client(ClientDeqIt &rm_cl)
{
    SOCKET_ID rm_id = rm_cl->getSocketId();
    clients.erase(rm_cl);
    try
    {
        if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, rm_id, NULL) == -1)
            throw LoadBalancer::LoadBalancerExceptions(E_EPOLLCTL, NULL);
    }
    catch (const LoadBalancer::LoadBalancerExceptions &e)
    {
        cout << "Couldn't Remove Client From epoll_ctl" << endl;
    }
}

LoadBalancer::~LoadBalancer()
{
    if (listener)
        delete listener;
    if (epoll_fd >= 0)
        close(epoll_fd);
}
