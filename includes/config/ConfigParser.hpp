/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 17:30:56 by bamrouch          #+#    #+#             */
/*   Updated: 2023/12/28 21:28:50 by bamrouch         ###   ########.fr       */
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
    E_SERVER_DIRECTIVE,
    E_LISTEN_DIRECTIVE,
    E_SERVER_NAME_DIRECTIVE,
    E_ERROR_PAGE_DIRECTIVE,
    E_LOCATION_DIRECTIVE,
    E_ROOT_DIRECTIVE,
    E_AUTOINDEX_DIRECTIVE,
    E_INDEX_DIRECTIVE,
    E_CLIENT_MAX_BODY_SIZE_DIRECTIVE,
    E_ALLOW_METHODS_DIRECTIVE,
    E_CGI_DIRECTIVE,
    E_UPLOAD_DIR_DIRECTIVE,
    E_REDIRECTION_DIRECTIVE,
}   config_errors;

enum DIRECTIVES_INDEX
{
    SERVER,
    LISTEN,
    SERVER_NAME,
    ERROR_PAGE,
    LOCATION,
    ROOT,
    AUTOINDEX,
    INDEX,
    CLIENT_MAX_BODY_SIZE,
    ALLOW_METHODS,
    CGI,
    UPLOAD_DIR,
    REDIRECTION,
};

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

const

class ConfigParser
{
    private:
        list<string> *tokens;
        short int depth;
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
        void parseConfig(TokenIt &it, ServerConfiguration *config);
        void parseServerDirective(TokenIt &it, ServerConfiguration *config);
        void parseListenDirective(TokenIt &it, ServerConfiguration *config);
        void parseServerNameDirective(TokenIt &it, ServerConfiguration *config);
        void parseErrorPageDirective(TokenIt &it, ServerConfiguration *config);
        void parseLocationDirective(TokenIt &it, ServerConfiguration *config);
        void parseRootDirective(TokenIt &it, ServerConfiguration *config);
        void parseAutoindexDirective(TokenIt &it, ServerConfiguration *config);
        void parseIndexDirective(TokenIt &it, ServerConfiguration *config);
        void parseClientMaxBodySizeDirective(TokenIt &it, ServerConfiguration *config);
        void parseAllowMethodsDirective(TokenIt &it, ServerConfiguration *config);
        void parseCgiDirective(TokenIt &it, ServerConfiguration *config);
        void parseUploadDirDirective(TokenIt &it, ServerConfiguration *config);
        void parseRedirectionDirective(TokenIt &it, ServerConfiguration *config);
        ~ConfigParser();
};
