/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfiguration.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 05:50:19 by bamrouch          #+#    #+#             */
/*   Updated: 2023/12/28 07:02:58 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <map>
#include <string>


using std::map;
using std::string;


class ServerConfiguration
{
    private:
        string  *config_value;
        map<string, ServerConfiguration> *subdirective;
    public:
        typedef map<string, ServerConfiguration>::const_iterator ConfigIt;
        ServerConfiguration(const string &value);
        ServerConfiguration(const map<string, ServerConfiguration> &config);
        ServerConfiguration(const ServerConfiguration &cpy_config);
        ServerConfiguration &operator=(const ServerConfiguration &eq_config);
        ~ServerConfiguration();
};

