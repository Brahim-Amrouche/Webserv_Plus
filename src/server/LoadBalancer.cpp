/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LoadBalancer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/25 14:24:00 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/14 20:29:30 by bamrouch         ###   ########.fr       */
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

LoadBalancer::LoadBalancer(deque<ServerSocket> *sockets):listeners(sockets), epoll_fd(-1), events_trigered(0) ,load(0)
{
    cout << "Initiating The Load Balancer" << endl;
    if ((epoll_fd = epoll_create(1)) < 0)
        throw LoadBalancer::LoadBalancerExceptions(E_EPOLLINIT, NULL);
    for (size_t i = 0; i < listeners->size() ; i++)
    {
        FT::memset(events, 0, sizeof(EPOLL_EVENT));
        (*listeners)[i].fill_epoll_event(events, EPOLLIN | EPOLLET);
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, (*listeners)[i].getSockid(), events) == -1)
        {
            close(epoll_fd);
            delete listeners;
            throw LoadBalancer::LoadBalancerExceptions(E_EPOLLCTL, NULL);
        }
        ++load;
    }
    FT::memset(events, 0, sizeof(EPOLL_EVENT) * MAX_EVENTS);
    cout << "Load Balancer Initialized" << endl;
}

void LoadBalancer::loop()
{
    while (true)
    {   
        FT::memset(events, 0, sizeof(EPOLL_EVENT) * load);
        events_trigered = epoll_wait(epoll_fd, events, load, LOADBALANCER_CHECK_DELAY);
        if (events_trigered < 0)
            throw LoadBalancer::LoadBalancerExceptions(E_EPOLLWAIT, this);
        else if (events_trigered > 0)
            handle_request();
        // else
        //     cout << "listening..." << endl;
        check_timeouts();
    }
}

void LoadBalancer::check_timeouts()
{
    for (ClientMapIt it = clients.begin(); it != clients.end(); it++)
        it->second.checkTimeout();
}

SrvSockDeqIt LoadBalancer::find_server(SOCKET_ID &sock_id)
{
    for (SrvSockDeqIt it = listeners->begin(); it != listeners->end(); it++)
    {
        if (it->getSockid() == sock_id)
            return it;
    }
    return listeners->end();
}

void LoadBalancer::handle_request()
{
    int i = -1;
    ClientMapIt cl_it = clients.end();
    SrvSockDeqIt srv_sock = listeners->end();
    int event_fd = -1;
    while (++i < events_trigered)
    {
        cl_it = clients.end();
        srv_sock = listeners->end();
        event_fd =  events[i].data.fd;
        if ((srv_sock = find_server(event_fd)) != listeners->end())
        {
            cout << "the fd of event done :" << event_fd << endl;
            if (events[i].events & EPOLLIN)
                add_client(i, srv_sock);
        }
        else if ((cl_it = clients.find(event_fd)) != clients.end())
        {
            try
            {
                if (events[i].events & EPOLLHUP)
                    throw Client::ClientExceptions(E_CLIENT_CLOSED, NULL);
                else if (events[i].events & EPOLLIN)
                    cl_it->second.receive();
                else if (events[i].events & EPOLLOUT)
                    cl_it->second.send();
            }
            catch (const Client::ClientExceptions &e)
            {
                cout << e.what() << endl;
                clients.erase(cl_it);
                --load;
            }
        }
    }
}

void LoadBalancer::add_client(int event_id, SrvSockDeqIt &server)
{
    Socket *new_client_sock = server->sockAccept();
    Client tmp_client(new_client_sock, *server);
    std::pair<ClientMapIt, bool> res = clients.insert(std::make_pair(new_client_sock->getSockid(), tmp_client));
    if (!res.second)
    {
        delete new_client_sock;
        return;
    }
    res.first->second.setCloseSocket(true);
    tmp_client.nullify();
    FT::memset(&(events[event_id]), 0, sizeof(EPOLL_EVENT));
    new_client_sock->fill_epoll_event(&(events[event_id]), EPOLLIN | EPOLLOUT | EPOLLHUP);
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_client_sock->getSockid(), &(events[event_id])) == -1)
        throw LoadBalancer::LoadBalancerExceptions(E_EPOLLCTL, this);
    ++load;
    cout << "New Client Added: "<< new_client_sock->getSockid()  << endl;
}

void    LoadBalancer::remove_client(ClientMapIt &rm_cl)
{
    try
    {
        if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, rm_cl->second.getSocketId(), NULL) == -1)
            throw LoadBalancer::LoadBalancerExceptions(E_EPOLLCTL, NULL);
    }
    catch (const LoadBalancer::LoadBalancerExceptions &e)
    {
        cout << "Couldn't Remove Client From epoll_ctl" << endl;
    }
    clients.erase(rm_cl);
}

LoadBalancer::~LoadBalancer()
{
    if (listeners)
        delete listeners;
    if (epoll_fd >= 0)
        close(epoll_fd);
    
}
