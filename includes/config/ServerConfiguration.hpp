/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfiguration.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 05:50:19 by bamrouch          #+#    #+#             */
/*   Updated: 2023/12/31 17:45:43 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <map>
#include <string>
#include <exception>
#include <deque>

using std::exception;
using std::map;
using std::string;

class ServerConfiguration
{
    private:
        deque<string> *config_values;
        map<string, ServerConfiguration> *subdirective;
    public:
        class ServerConfiguarationException : public exception
        {
            public:
                const char *what() const throw()
                {
                    return "ServerConfiguarationException";
                };
        };
        typedef map<string, ServerConfiguration>::iterator ConfigIt;
        typedef map<string, ServerConfiguration>::const_iterator ConfigConstIt;
        ServerConfiguration();
        ServerConfiguration(const string &value);
        ServerConfiguration(const map<string, ServerConfiguration> &config);
        ServerConfiguration(const ServerConfiguration &cpy_config);
        ServerConfiguration &operator=(const ServerConfiguration &eq_config);
        deque<string>       *getConfigValue();
        ServerConfiguration *getSubdirective(const string &path);
        void    setConfigValue(string *value);
        void    setSubdirective(map<string, ServerConfiguration> *config);
        void    setToNull();
        void    pushConfValue(const string &value);
        void    pushSubdirective(const    string &directive, const ServerConfiguration &config);
        void    debug_print_directives();
        ~ServerConfiguration();
};

