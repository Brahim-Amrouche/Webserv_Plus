/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 17:30:56 by bamrouch          #+#    #+#             */
/*   Updated: 2023/12/31 17:41:49 by bamrouch         ###   ########.fr       */
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

extern const char *directives[13];

class ConfigParser
{
    private:
        list<string> *tokens;
        ServerConfiguration *config;
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
        TokenIt getTokenStart();
        void parseConfig(TokenIt &it);
        void parseServerDirective(TokenIt &it);
        void parseListenDirective(TokenIt &it);
        void parseServerNameDirective(TokenIt &it);
        void parseErrorPageDirective(TokenIt &it);
        void parseLocationDirective(TokenIt &it);
        void parseRootDirective(TokenIt &it);
        void parseAutoindexDirective(TokenIt &it);
        void parseIndexDirective(TokenIt &it);
        void parseClientMaxBodySizeDirective(TokenIt &it);
        void parseAllowMethodsDirective(TokenIt &it);
        void parseCgiDirective(TokenIt &it);
        void parseUploadDirDirective(TokenIt &it);
        void parseRedirectionDirective(TokenIt &it);
        void debug_print_servers();
        ~ConfigParser();
};
