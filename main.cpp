/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/25 10:12:41 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/01 22:23:30 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

t_cleanup cleanup_data = {NULL, NULL};

void sigIntHandler(int sigNum)
{
    cout << "Cleaning Up..." << endl;
    if (cleanup_data.cleanup_server_sock)
        delete cleanup_data.cleanup_server_sock;
    if (cleanup_data.cleanup_loadbalancer)
        delete cleanup_data.cleanup_loadbalancer;
    exit(sigNum);
}


int main(int argc, char *argv[])
{
    deque<ServerSocket> *server_sockets = NULL;
    if (argc == 1)
       server_sockets = configure(DEFAULT_CONFIG);
    else if(argc == 2)
        server_sockets = configure(argv[1]);
    else
    {
        cout << "Usage: ./webserv [config_file]" << endl;
        return (1);
    }
    if (!server_sockets)
        return (1);
    if (signal(SIGINT, sigIntHandler) == SIG_ERR)
    {
        cout << "Couldn't Add signal handler" << endl;
        return (1);
    }
    try
    {
        LoadBalancer *load_balancer = new LoadBalancer(server_sockets);
        cleanup_data.cleanup_server_sock = NULL;
        cleanup_data.cleanup_loadbalancer = load_balancer;
        load_balancer->loop();
    }
    catch (const LoadBalancer::LoadBalancerExceptions &e)
    {
        cout << e.what() << endl;
        return (1);
    }
}