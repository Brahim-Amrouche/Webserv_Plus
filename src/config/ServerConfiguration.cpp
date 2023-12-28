/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfiguration.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 05:52:51 by bamrouch          #+#    #+#             */
/*   Updated: 2023/12/28 07:02:21 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/webserv.hpp"


ServerConfiguration::ServerConfiguration(const string &value):subdirective(NULL)
{
    config_value = new string(value);
}

ServerConfiguration::ServerConfiguration(const map<string, ServerConfiguration> &config)
{
    subdirective = new map<string, ServerConfiguration>();
    for (ConfigIt it = config.begin(); it != config.end(); it++)
        subdirective->insert(std::pair<string, ServerConfiguration>(it->first, it->second));
    
}


ServerConfiguration &ServerConfiguration::operator=(const ServerConfiguration &eq_config)
{
    if (this != &eq_config)
    {
        config_value = eq_config.config_value;
        subdirective = eq_config.subdirective;
    }
    return *this;
}


ServerConfiguration::~ServerConfiguration()
{
    if (config_value)
        delete config_value;
    if (subdirective)
        delete subdirective;
}

