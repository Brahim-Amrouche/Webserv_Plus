/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 06:28:14 by bamrouch          #+#    #+#             */
/*   Updated: 2023/12/28 21:42:40 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

ConfigParser::ConfigParser(list<string> *new_tokens):tokens(new_tokens), depth(0)
{}

void    ConfigParser::parseConfig(TokenIt &start_token, ServerConfiguration *config)
{
    while (start_token != tokens->end())
    {
        if (*start_token == directives[SERVER])
            parseServerDirective(start_token, config);
        else if (*start_token == directives[LISTEN])
            parseListenDirective(start_token, config);
        else if (*start_token == directives[SERVER_NAME])
            parseServerNameDirective(start_token, config);
        else if (*start_token == directives[ERROR_PAGE])
            parseErrorPageDirective(start_token, config);
        else if (*start_token == directives[LOCATION])
            parseLocationDirective(start_token, config);
        else if (*start_token == directives[ROOT])
            parseRootDirective(start_token, config);
        else if (*start_token == directives[AUTOINDEX])
            parseAutoindexDirective(start_token, config);
        else if (*start_token == directives[INDEX])
            parseIndexDirective(start_token, config);
        else if (*start_token == directives[CLIENT_MAX_BODY_SIZE])
            parseClientMaxBodySizeDirective(start_token, config);
        else if (*start_token == directives[ALLOW_METHODS])
            parseAllowMethodsDirective(start_token, config);
        else if (*start_token == directives[CGI])
            parseCgiDirective(start_token, config);
        else if (*start_token == directives[UPLOAD_DIR])
            parseUploadDirDirective(start_token, config);
        else if (*start_token == directives[REDIRECTION])
            parseRedirectionDirective(start_token, config);
        else if (*start_token == "}")
        {
            --depth;
            ++start_token;
            if (start_token != tokens->end() && *start_token == ";")
                ++start_token;
            break;
        }
        else
            throw ConfigParserException(E_WRONG_DIRECTIVE, this);
        ++start_token;
    }
}

void ConfigParser::parseServerDirective(TokenIt &start_token, ServerConfiguration *config)
{
    if (depth != 0)
        throw ConfigParserException(E_SERVER_DIRECTIVE, this);
    ++start_token;
    if (start_token == tokens->end() || *start_token != "{")
        throw ConfigParserException(E_SERVER_DIRECTIVE, this);
    ++depth;
    ++start_token;
    servers.push_back(ServerConfiguration(directives[SERVER]));
    config = &(servers.back());
    parseConfig(start_token, config);
}

void ConfigParser::parseListenDirective(TokenIt &start_token, ServerConfiguration *config)
{
    if (depth != 1)
        throw ConfigParserException(E_LISTEN_DIRECTIVE, this);
    ++start_token;
    if (start_token == tokens->end())
        throw ConfigParserException(E_LISTEN_DIRECTIVE, this);
    ++start_token;
}

void    ConfigParser::parseServerNameDirective(TokenIt &start_token, ServerConfiguration *config)
{
    if (depth != 1)
        throw ConfigParserException(E_SERVER_NAME_DIRECTIVE, this);
    ++start_token;
    if (start_token == tokens->end())
        throw ConfigParserException(E_SERVER_NAME_DIRECTIVE, this);
    ++start_token;
}

void    ConfigParser::parseLocationDirective(TokenIt &start_token, ServerConfiguration *config)
{
    if (depth != 1)
        throw ConfigParserException(E_LOCATION_DIRECTIVE, this);
    ++start_token;
    if (start_token == tokens->end())
        throw ConfigParserException(E_LOCATION_DIRECTIVE, this);
    ++start_token;
}

void    ConfigParser::parseErrorPageDirective(TokenIt &start_token, ServerConfiguration *config)
{
    if (depth < 1)
        throw ConfigParserException(E_ERROR_PAGE_DIRECTIVE, this);
    ++start_token;
    if (start_token == tokens->end())
        throw ConfigParserException(E_ERROR_PAGE_DIRECTIVE, this);
    ++start_token;
}


void    ConfigParser::parseRootDirective(TokenIt &start_token, ServerConfiguration *config)
{
    if (depth < 1)
        throw ConfigParserException(E_ROOT_DIRECTIVE, this);
    ++start_token;
    if (start_token == tokens->end())
        throw ConfigParserException(E_ROOT_DIRECTIVE, this);
    ++start_token;
}

void    ConfigParser::parseAutoindexDirective(TokenIt &start_token, ServerConfiguration *config)
{
    if (depth < 1)
        throw ConfigParserException(E_AUTOINDEX_DIRECTIVE, this);
    ++start_token;
    if (start_token == tokens->end())
        throw ConfigParserException(E_AUTOINDEX_DIRECTIVE, this);
    ++start_token;
}

void    ConfigParser::parseIndexDirective(TokenIt &start_token, ServerConfiguration *config)
{
    if (depth < 1)
        throw ConfigParserException(E_INDEX_DIRECTIVE, this);
    ++start_token;
    if (start_token == tokens->end())
        throw ConfigParserException(E_INDEX_DIRECTIVE, this);
    ++start_token;
}

void    ConfigParser::parseClientMaxBodySizeDirective(TokenIt &start_token, ServerConfiguration *config)
{
    if (depth < 1)
        throw ConfigParserException(E_CLIENT_MAX_BODY_SIZE_DIRECTIVE, this);
    ++start_token;
    if (start_token == tokens->end())
        throw ConfigParserException(E_CLIENT_MAX_BODY_SIZE_DIRECTIVE, this);
    ++start_token;
}

void    ConfigParser::parseAllowMethodsDirective(TokenIt &start_token, ServerConfiguration *config)
{
    if (depth < 1)
        throw ConfigParserException(E_ALLOW_METHODS_DIRECTIVE, this);
    ++start_token;
    if (start_token == tokens->end())
        throw ConfigParserException(E_ALLOW_METHODS_DIRECTIVE, this);
    ++start_token;
}

void    ConfigParser::parseCgiDirective(TokenIt &start_token, ServerConfiguration *config)
{
    if (depth < 1)
        throw ConfigParserException(E_CGI_DIRECTIVE, this);
    ++start_token;
    if (start_token == tokens->end())
        throw ConfigParserException(E_CGI_DIRECTIVE, this);
    ++start_token;
}

void    ConfigParser::parseUploadDirDirective(TokenIt &start_token, ServerConfiguration *config)
{
    if (depth < 1)
        throw ConfigParserException(E_UPLOAD_DIR_DIRECTIVE, this);
    ++start_token;
    if (start_token == tokens->end())
        throw ConfigParserException(E_UPLOAD_DIR_DIRECTIVE, this);
    ++start_token;
}

void    ConfigParser::parseRedirectionDirective(TokenIt &start_token, ServerConfiguration *config)
{
    if (depth < 1)
        throw ConfigParserException(E_REDIRECTION_DIRECTIVE, this);
    ++start_token;
    if (start_token == tokens->end())
        throw ConfigParserException(E_REDIRECTION_DIRECTIVE, this);
    ++start_token;
}


ConfigParser::~ConfigParser()
{
    delete tokens;
}