/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 17:30:56 by bamrouch          #+#    #+#             */
/*   Updated: 2023/12/28 06:51:39 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "TokenizeInput.hpp"
#include "ServerConfiguration.hpp"
#include <map>

using std::map;
using std::deque;

typedef enum CONFIG_PARSER_ERR_CODES
{
    E_WRONG_DIRECTIVE,
    
}   config_errors;

const char *directives[] =
{
    "server",
    "listen",
    "server_name",
    "error_page",
    "location",
    "root",
    "autoindex",
    "index",
    "client_max_body_size",
    "allow_methods",
    "cgi",
    "upload_dir",
    "redirection",
};

class ConfigParser
{
    private:
        list<string> *tokens;
        int          subdirective_count;
        deque<ServerConfiguration> servers;
    public:
        typedef list<string>::iterator TokenIt;
        class ConfigParserException: public TException<config_errors, ConfigParser>
        {
            public:
                ConfigParserException(const config_errors &err, ConfigParser *cln);
                virtual const char *what() const throw()
                {
                    return TException::what();
                };
                virtual ~ConfigParserException() throw()
                {};
        };
        ConfigParser(list<string> *new_tokens);
        void parseConfig();
        ~ConfigParser();
};