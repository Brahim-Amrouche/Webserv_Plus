/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/25 14:12:51 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/08 17:52:07 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "./server/server.hpp"
#include "./helpers/helpers.hpp"
#include "./config/config.hpp"
#include "./request/Request.hpp"
#include "./response/Response.hpp"
#include <climits>

#define DEFAULT_CONFIG "./config/default_config.conf"
#define DEFAULT_ROOT "./content"
#define DEFAULT_TMP_FOLDER "/tmp"
#define DEFAULT_INDEX "index.html"

extern map<string, string> mimetypes;

typedef struct s_cleanup
{
    deque<ServerSocket>   *cleanup_server_sock;
    LoadBalancer *cleanup_loadbalancer;
    // map<string, string> *cleanup_mimetypes;
}   t_cleanup;

extern t_cleanup cleanup_data;
