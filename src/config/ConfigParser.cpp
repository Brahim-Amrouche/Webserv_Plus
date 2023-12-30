/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 06:28:14 by bamrouch          #+#    #+#             */
/*   Updated: 2023/12/30 20:03:35 by bamrouch         ###   ########.fr       */
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
    std::advance(start_token, 1);
    if (start_token == tokens->end() || *start_token != "{")
        throw ConfigParserException(E_SERVER_DIRECTIVE, this);
    ++depth;
    std::advance(start_token, 1);
    ServerConfiguration temp(directives[SERVER]);
    servers.push_back(temp);
    temp.setToNull();
    config = &(servers.back());
    parseConfig(start_token, config);
}

void ConfigParser::parseListenDirective(TokenIt &start_token, ServerConfiguration *config)
{
    if (depth != 1)
        throw ConfigParserException(E_LISTEN_DIRECTIVE, this);
    std::advance(start_token, 1);
    string parsed_data[2];
    try{
        if (start_token == tokens->end())
            throw ConfigParserException(E_LISTEN_DIRECTIVE, this);    
        PH::parseHostString(*start_token, parsed_data);
        ServerConfiguration host_port;
        host_port.pushConfValue(parsed_data[0]);
        host_port.pushConfValue(parsed_data[1]);
        if (config->getSubdirective(directives[LISTEN]))
            throw ServerConfiguration::ServerConfiguarationException();
        config->pushSubdirective(directives[LISTEN], host_port);
        host_port.setToNull();
        std::advance(start_token, 1);
        if (start_token == tokens->end() || *start_token != ";")
            throw PH::PHException();
        std::advance(start_token, 1);
    }
    catch (const  exception &e)
    {
        throw ConfigParserException(E_LISTEN_DIRECTIVE, this);
    }
}

void    ConfigParser::parseServerNameDirective(TokenIt &start_token, ServerConfiguration *config)
{
    if (depth != 1)
        throw ConfigParserException(E_SERVER_NAME_DIRECTIVE, this);
    std::advance(start_token,1);
    if (start_token == tokens->end() || PH::strIsBreakToken(*start_token))
        throw ConfigParserException(E_SERVER_NAME_DIRECTIVE, this);
    try
    {
        if (config->getSubdirective(directives[SERVER_NAME]))
            throw ServerConfiguration::ServerConfiguarationException();
        config->pushSubdirective(directives[SERVER_NAME], *start_token);
    }
    catch(const exception &e)
    {
        throw ConfigParserException(E_SERVER_NAME_DIRECTIVE, this);
    }
    std::advance(start_token, 1);
    if (start_token == tokens->end() || *start_token != ";")
        throw ConfigParserException(E_SERVER_NAME_DIRECTIVE, this);
    std::advance(start_token, 1);
}

void    ConfigParser::parseLocationDirective(TokenIt &start_token, ServerConfiguration *config)
{
    if (depth != 1)
        throw ConfigParserException(E_LOCATION_DIRECTIVE, this);
    std::advance(start_token, 1);
    if (start_token == tokens->end() || PH::strIsPath(*start_token))
        throw ConfigParserException(E_LOCATION_DIRECTIVE, this);
    ServerConfiguration *root_conf = config;
    try
    {
        ServerConfiguration temp(directives[LOCATION]);
        config->pushSubdirective(*start_token, temp);
        temp.setToNull();
        config = config->getSubdirective(*start_token);
        std::advance(start_token, 1);
        if (start_token == tokens->end() || *start_token != "{")
            throw ConfigParserException(E_LOCATION_DIRECTIVE, NULL);
        ++depth;
        parseConfig(start_token, config);
        config = root_conf;
    }
    catch(const exception &e)
    {
        throw ConfigParserException(E_LOCATION_DIRECTIVE, this);
    }
}

void    ConfigParser::parseErrorPageDirective(TokenIt &start_token, ServerConfiguration *config)
{
    if (depth < 1)
        throw ConfigParserException(E_ERROR_PAGE_DIRECTIVE, this);
    std::advance(start_token, 1);
    if (start_token == tokens->end() || PH::getHttpCodeType(*start_token) == UNVALID_CODE 
        || (PH::getHttpCodeType(*start_token) != CLIENT_ERROR_CODE && PH::getHttpCodeType(*start_token) != SERVER_ERROR_CODE))
        throw ConfigParserException(E_ERROR_PAGE_DIRECTIVE, this);
    ServerConfiguration *subdir = NULL;
    if (subdir = config->getSubdirective(directives[ERROR_PAGE]))
        throw ConfigParserException(E_ERROR_PAGE_DIRECTIVE, this);
    else
    {
        ServerConfiguration temp(directives[ERROR_PAGE]);
        config->pushSubdirective(directives[ERROR_PAGE], temp);
        temp.setToNull();
        subdir = config->getSubdirective(directives[ERROR_PAGE]);
    }
    subdir->pushConfValue(*start_token);
    std::advance(start_token, 1);
    if (start_token == tokens->end() || !PH::strIsPath(*start_token))
        throw ConfigParserException(E_ERROR_PAGE_DIRECTIVE, this);
    subdir->pushConfValue(*start_token);
    std::advance(start_token, 1);
    if (start_token == tokens->end() || *start_token != ";")
        throw ConfigParserException(E_ERROR_PAGE_DIRECTIVE, this);
    std::advance(start_token, 1);
}


void    ConfigParser::parseRootDirective(TokenIt &start_token, ServerConfiguration *config)
{
    if (depth < 1)
        throw ConfigParserException(E_ROOT_DIRECTIVE, this);
    std::advance(start_token, 1);
    if (start_token == tokens->end() || !PH::strIsPath(*start_token))
        throw ConfigParserException(E_ROOT_DIRECTIVE, this);
    try 
    {
        ServerConfiguration temp(*start_token);
        config->pushSubdirective(directives[ROOT], temp);
        temp.setToNull();
    }
    catch(const std::exception &e)
    {
        throw ConfigParserException(E_ROOT_DIRECTIVE, this);
    }
    std::advance(start_token, 1);
    if (start_token == tokens->end() || *start_token != ";")
        throw ConfigParserException(E_ROOT_DIRECTIVE, this);
    std::advance(start_token, 1);
}

void    ConfigParser::parseAutoindexDirective(TokenIt &start_token, ServerConfiguration *config)
{
    if (depth < 1)
        throw ConfigParserException(E_AUTOINDEX_DIRECTIVE, this);
    std::advance(start_token, 1);
    if (start_token == tokens->end() || (*start_token != "on" && *start_token != "off"))
        throw ConfigParserException(E_AUTOINDEX_DIRECTIVE, this);
    try
    {
        ServerConfiguration temp(*start_token);
        config->pushSubdirective(directives[AUTOINDEX], temp);
        temp.setToNull();
    }
    catch (const std::exception &e)
    {
        throw ConfigParserException(E_AUTOINDEX_DIRECTIVE, this);
    }
    std::advance(start_token, 1);
    if (start_token == tokens->end() || *start_token != ";")
        throw ConfigParserException(E_AUTOINDEX_DIRECTIVE, this);
    std::advance(start_token, 1);
}

void    ConfigParser::parseIndexDirective(TokenIt &start_token, ServerConfiguration *config)
{
    if (depth < 1)
        throw ConfigParserException(E_INDEX_DIRECTIVE, this);
    std::advance(start_token, 1);
    if (start_token == tokens->end() || !PH::strIsPath(*start_token))
        throw ConfigParserException(E_INDEX_DIRECTIVE, this);
    try
    {
        ServerConfiguration temp(*start_token);
        config->pushSubdirective(directives[INDEX], temp);
        temp.setToNull();
    }
    catch (const std::exception &e)
    {
        throw ConfigParserException(E_INDEX_DIRECTIVE, this);
    }
    std::advance(start_token, 1);
    if (start_token == tokens->end() || *start_token != ";")
        throw ConfigParserException(E_INDEX_DIRECTIVE, this);
    std::advance(start_token, 1);
}

void    ConfigParser::parseClientMaxBodySizeDirective(TokenIt &start_token, ServerConfiguration *config)
{
    if (depth < 1)
        throw ConfigParserException(E_CLIENT_MAX_BODY_SIZE_DIRECTIVE, this);
    std::advance(start_token, 1);
    if (start_token == tokens->end())
        throw ConfigParserException(E_CLIENT_MAX_BODY_SIZE_DIRECTIVE, this);
    try
    {
        string body_size = PH::parseBodySize(*start_token);
        ServerConfiguration temp(body_size);
        config->pushSubdirective(directives[CLIENT_MAX_BODY_SIZE], temp);
        temp.setToNull();
    }
    catch(const std::exception& e)
    {
        throw ConfigParserException(E_CLIENT_MAX_BODY_SIZE_DIRECTIVE, this);
    }
    std::advance(start_token, 1);
    if (start_token == tokens->end() || *start_token != ";")
        throw ConfigParserException(E_CLIENT_MAX_BODY_SIZE_DIRECTIVE, this);
    std::advance(start_token, 1);
}

void    ConfigParser::parseAllowMethodsDirective(TokenIt &start_token, ServerConfiguration *config)
{
    if (depth < 1)
        throw ConfigParserException(E_ALLOW_METHODS_DIRECTIVE, this);
    ServerConfiguration temp(directives[ALLOW_METHODS]);
    deque<string> *methods_list = temp.getConfigValue();
    std::advance(start_token, 1);
    if (start_token == tokens->end())
        throw ConfigParserException(E_ALLOW_METHODS_DIRECTIVE, this);
    try
    {
        TokenIt end_token = tokens->end();
        PH::strIsAllowedMethod(start_token, end_token, methods_list);
        config->pushSubdirective(directives[ALLOW_METHODS], temp);
        temp.setToNull();
    }
    catch  (const exception &e)
    {
        throw ConfigParserException(E_ALLOW_METHODS_DIRECTIVE, this);
    }
    if (start_token == tokens->end() || *start_token != ";")
        throw ConfigParserException(E_ALLOW_METHODS_DIRECTIVE, this);
    std::advance(start_token, 1);
}

void    ConfigParser::parseCgiDirective(TokenIt &start_token, ServerConfiguration *config)
{
    if (depth < 1)
        throw ConfigParserException(E_CGI_DIRECTIVE, this);
    std::advance(start_token, 1);
    if (start_token == tokens->end() || *start_token != "php" && *start_token != "py")
        throw ConfigParserException(E_CGI_DIRECTIVE, this);
    try
    {
        ServerConfiguration temp(*start_token);
        config->pushSubdirective(directives[CGI], temp);
        temp.setToNull();
    }
    catch(const std::exception& e)
    {
        throw ConfigParserException(E_CGI_DIRECTIVE, this);
    }
    std::advance(start_token, 1);
    if (start_token == tokens->end() || *start_token != ";")
        throw ConfigParserException(E_CGI_DIRECTIVE, this);
    std::advance(start_token, 1);
}

void    ConfigParser::parseUploadDirDirective(TokenIt &start_token, ServerConfiguration *config)
{
    if (depth < 1)
        throw ConfigParserException(E_UPLOAD_DIR_DIRECTIVE, this);
    std::advance(start_token, 1);
    if (start_token == tokens->end() || !PH::strIsPath(*start_token))
        throw ConfigParserException(E_UPLOAD_DIR_DIRECTIVE, this);
    try
    {
        ServerConfiguration temp(*start_token);
        config->pushSubdirective(directives[UPLOAD_DIR], temp);
        temp.setToNull();
    }
    catch(const std::exception& e)
    {
        throw ConfigParserException(E_UPLOAD_DIR_DIRECTIVE, this);
    }
    if (start_token == tokens->end())
        throw ConfigParserException(E_UPLOAD_DIR_DIRECTIVE, this);
    ++start_token;
}

void    ConfigParser::parseRedirectionDirective(TokenIt &start_token, ServerConfiguration *config)
{
    if (depth < 1)
        throw ConfigParserException(E_REDIRECTION_DIRECTIVE, this);
    std::advance(start_token, 1);
    if (start_token == tokens->end() || PH::getHttpCodeType(*start_token) == UNVALID_CODE 
        || PH::getHttpCodeType(*start_token) != REDIRECTION_CODE)
        throw ConfigParserException(E_REDIRECTION_DIRECTIVE, this);
    ServerConfiguration *subdir = NULL;
    if (subdir = config->getSubdirective(directives[REDIRECTION]))
        throw ConfigParserException(E_ERROR_PAGE_DIRECTIVE, this);
    else
    {
        ServerConfiguration temp(directives[REDIRECTION]);
        config->pushSubdirective(directives[REDIRECTION], temp);
        temp.setToNull();
        subdir = config->getSubdirective(directives[REDIRECTION]);
    }
    subdir->pushConfValue(*start_token);
    std::advance(start_token, 1);
    if (start_token == tokens->end() || !PH::strIsPath(*start_token))
        throw ConfigParserException(E_ERROR_PAGE_DIRECTIVE, this);
    subdir->pushConfValue(*start_token);
    if (start_token == tokens->end() || *start_token != ";")
        throw ConfigParserException(E_REDIRECTION_DIRECTIVE, this);
    std::advance(start_token, 1);
}

ConfigParser::~ConfigParser()
{
    delete tokens;
}
