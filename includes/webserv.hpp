/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/25 14:12:51 by bamrouch          #+#    #+#             */
/*   Updated: 2023/12/28 02:26:18 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "./server/server.hpp"
#include "./helpers/helpers.hpp"
#include "./config/config.hpp"

#define DEFAULT_CONFIG "./config/default_config.conf"

typedef struct s_cleanup
{
    Socket       *cleanup_server_sock;
    LoadBalancer *cleanup_loadbalancer;
}   t_cleanup;

extern t_cleanup cleanup_data;
