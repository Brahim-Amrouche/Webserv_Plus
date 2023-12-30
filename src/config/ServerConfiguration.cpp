/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfiguration.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 05:52:51 by bamrouch          #+#    #+#             */
/*   Updated: 2023/12/30 17:00:22 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/webserv.hpp"

ServerConfiguration::ServerConfiguration():config_values(NULL), subdirective(NULL)
{}

ServerConfiguration::ServerConfiguration(const string &value):subdirective(NULL)
{
    config_values = new deque<string>();
    config_values->push_back(value);
}

ServerConfiguration::ServerConfiguration(const map<string, ServerConfiguration> &config)
{
    subdirective = new map<string, ServerConfiguration>();
    for (ConfigConstIt it = config.begin(); it != config.end(); it++)
        subdirective->insert(std::pair<string, ServerConfiguration>(it->first, it->second));
}

ServerConfiguration::ServerConfiguration(const ServerConfiguration &cpy_config)
{
    ServerConfiguration::operator=(cpy_config);
}

ServerConfiguration &ServerConfiguration::operator=(const ServerConfiguration &eq_config)
{
    if (this != &eq_config)
    {
        config_values = eq_config.config_values;
        subdirective = eq_config.subdirective;
    }
    return *this;
}

deque<string>  *ServerConfiguration::getConfigValue()
{
    return config_values;
}

ServerConfiguration *ServerConfiguration::getSubdirective(const string &path)
{
    if (!subdirective)
        return NULL;
    ConfigIt it = subdirective->find(path);
    if (it == subdirective->end())
        return NULL;
    return &(it->second);
}

// void    ServerConfiguration::setConfigValue(string *value)
// {
//     if (config_values)
//         delete config_value;
//     config_values = value;
// }

// void    ServerConfiguration::setSubdirective(map<string, ServerConfiguration> *config)
// {
//     if (subdirective)
//         delete subdirective;
//     subdirective = config;
// }

void    ServerConfiguration::setToNull()
{
    config_values = NULL;
    subdirective = NULL;
}

void ServerConfiguration::pushConfValue(const string &value)
{
    if (!config_values)
        config_values = new deque<string>();
    config_values->push_back(value);
}

void ServerConfiguration::pushSubdirective(const string &path, const ServerConfiguration &config)
{
    if (!subdirective)
        subdirective = new map<string, ServerConfiguration>();
    if (subdirective->find(path) != subdirective->end())
        throw ServerConfiguarationException();
    subdirective->insert(std::pair<string, ServerConfiguration>(path, config));
}


ServerConfiguration::~ServerConfiguration()
{
    if (config_values)
        delete config_values;
    if (subdirective)
        delete subdirective;
}

