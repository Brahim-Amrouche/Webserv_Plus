/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfiguration.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 05:52:51 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/03 17:20:58 by bamrouch         ###   ########.fr       */
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

ServerConfiguration::ServerConfiguration(ServerConfiguration *cpy_config):config_values(NULL), subdirective(NULL)
{
    if (!cpy_config)
        return;
    if (cpy_config->config_values)
    {
        config_values = new deque<string>();
        for (deque<string>::iterator it = cpy_config->config_values->begin(); it != cpy_config->config_values->end(); it++)
            config_values->push_back(*it);
    }
    if (cpy_config->subdirective)
    {
        subdirective = new map<string, ServerConfiguration>();
        for (ConfigIt it = cpy_config->subdirective->begin(); it != cpy_config->subdirective->end(); it++)
        {
            ServerConfiguration temp(&it->second);
            (*subdirective)[it->first] = temp;
            temp.setToNull();
        }
    }
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
    (*subdirective)[path] = config;
}

void ServerConfiguration::normalizeLocations()
{
    if (!subdirective)
        return;
    for (ConfigIt it = subdirective->begin(); it != subdirective->end(); it++)
    {
        if (!PH::strIsPath(it->first))
            continue;
        ServerConfiguration &location = it->second;
        for (ConfigIt it = subdirective->begin(); it != subdirective->end(); it++)
        {
            if(PH::strIsPath(it->first))
                continue;
            if (location[it->first])
                continue;
            ServerConfiguration temp(&it->second);
            location.pushSubdirective(it->first, temp);
            temp.setToNull();
        }
    }
}

deque<string> *ServerConfiguration::operator*()
{
    return config_values;
}

ServerConfiguration *ServerConfiguration::operator[](Path &location_path)
{
    if (!subdirective)
        return NULL;
    while (*location_path != "")
    {
        map<string, ServerConfiguration>::iterator found = subdirective->find(*location_path);
        if (found != subdirective->end())
            return &(found->second);
        --location_path;
    }
    return this;
}

ServerConfiguration *ServerConfiguration::operator[](string dir_name)
{
    if (!subdirective)
        return NULL;
    map<string, ServerConfiguration>::iterator found = subdirective->find(dir_name);
    if (found == subdirective->end())
        return NULL;
    return &(found->second);
}

void ServerConfiguration::debug_print_directives()
{
    if (config_values)
    {
        cout << "config_values-> ";
        for (deque<string>::iterator it = config_values->begin(); it != config_values->end(); it++)
            cout << *it << " ";
        cout << std::endl;
    }
    if (subdirective)
    {
        std::cout << "subdirective => " << std::endl;
        for (ConfigIt it = subdirective->begin(); it != subdirective->end(); it++)
        {
            std::cout << it->first << "-> ";
            it->second.debug_print_directives();
        }
        std::cout << "subdirective end <=" << std::endl;
    }
}



ServerConfiguration::~ServerConfiguration()
{
    if (config_values)
        delete config_values;
    if (subdirective)
        delete subdirective;
}

