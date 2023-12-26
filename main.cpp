/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/25 10:12:41 by bamrouch          #+#    #+#             */
/*   Updated: 2023/12/26 16:43:21 by bamrouch         ###   ########.fr       */
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
    (void) argc;
    if (signal(SIGINT, sigIntHandler) == SIG_ERR)
    {
        cout << "Couldn't Add signal handler" << endl;
        return (1);
    }
    try
    {
        Socket *server_sock = new Socket(argv[1], argv[2]);
        cleanup_data.cleanup_server_sock = server_sock;
        LoadBalancer *load_balancer = new LoadBalancer(server_sock);
        cleanup_data.cleanup_server_sock = NULL;
        cleanup_data.cleanup_loadbalancer = load_balancer;
        load_balancer->loop();
    }
    catch (const Socket::SocketExceptions &e)
    {
        cout << e.what() << endl;
    }
    catch (const LoadBalancer::LoadBalancerExceptions &e)
    {
        cout << e.what() << endl;
    }
}