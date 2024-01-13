/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 06:28:14 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/13 02:15:56 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

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

ConfigParser::ConfigParserException::ConfigParserException(const config_errors &err, ConfigParser *cln):TException<config_errors, ConfigParser>("ConfigError: ", err, cln)
{
    switch (err_c)
    {
        case E_WRONG_DIRECTIVE:
            msg+="Invalid directive";
            break;
        case E_SERVER_DIRECTIVE:
            msg+="Wrong Server Directive Format server {}";
            break;
        case E_LISTEN_DIRECTIVE:
            msg+="Wrong Listen Directive Format listen [host]:[port];";
            break;
        case E_SERVER_NAME_DIRECTIVE:
            msg+="Server Name Directive Format server_name [name];";
            break;
        case E_ERROR_PAGE_DIRECTIVE:
            msg+="Error Page Directive Format error_page [code]... [path];";
            break;
        case E_LOCATION_DIRECTIVE:
            msg+="Location Directive Format location [path] {}";
            break;
        case E_ROOT_DIRECTIVE:
            msg+="Root Directive Format root [path];";
            break;
        case E_AUTOINDEX_DIRECTIVE:
            msg+="Autoindex Directive Format autoindex [on|off];";
            break;
        case E_INDEX_DIRECTIVE:
            msg+="Index Directive Format index [path];";
            break;
        case E_CLIENT_MAX_BODY_SIZE_DIRECTIVE:
            msg+="Client Max Body Size Directive Format client_max_body_size [size](K|M);";
            break;
        case E_ALLOW_METHODS_DIRECTIVE:
            msg+="Allow Methods Directive Format allow_methods [method]...;";
            break;
        case E_CGI_DIRECTIVE:
            msg+="CGI Directive Format cgi [php|py];";
            break;
        case E_UPLOAD_DIR_DIRECTIVE:
            msg+="Upload Dir Directive Format upload_dir [path];";
            break;
        case E_REDIRECTION_DIRECTIVE:
            msg+="Redirection Directive Format redirection [code] [path];";
            break;
        case E_LISTEN_DIRECTIVE_MISSING:
            msg+="No Listen Directive Given";
            break;
        default:
            msg+= "Unknown Error";
            break;
    }   
}


ConfigParser::ConfigParser(list<string> *new_tokens):tokens(new_tokens), config(NULL), depth(0), server_sockets(NULL)
{}

ConfigParser::TokenIt ConfigParser::getTokenStart()
{
    return tokens->begin();
}

void    ConfigParser::parseConfig(TokenIt &start_token)
{
    while (start_token != tokens->end())
    {
        if (*start_token == directives[SERVER])
            parseServerDirective(start_token);
        else if (*start_token == directives[LISTEN])
            parseListenDirective(start_token);
        else if (*start_token == directives[SERVER_NAME])
            parseServerNameDirective(start_token);
        else if (*start_token == directives[ERROR_PAGE])
            parseErrorPageDirective(start_token);
        else if (*start_token == directives[LOCATION])
            parseLocationDirective(start_token);
        else if (*start_token == directives[ROOT])
            parseRootDirective(start_token);
        else if (*start_token == directives[AUTOINDEX])
            parseAutoindexDirective(start_token);
        else if (*start_token == directives[INDEX])
            parseIndexDirective(start_token);
        else if (*start_token == directives[CLIENT_MAX_BODY_SIZE])
            parseClientMaxBodySizeDirective(start_token);
        else if (*start_token == directives[ALLOW_METHODS])
            parseAllowMethodsDirective(start_token);
        else if (*start_token == directives[CGI])
            parseCgiDirective(start_token);
        else if (*start_token == directives[UPLOAD_DIR])
            parseUploadDirDirective(start_token);
        else if (*start_token == directives[REDIRECTION])
            parseRedirectionDirective(start_token);
        else if (*start_token == "}")
        {
            --depth;
            std::advance(start_token, 1);
            if (start_token != tokens->end() && *start_token == ";")
                ;
            else
                std::advance(start_token, -1);
            return;
        }
        else
            throw ConfigParserException(E_WRONG_DIRECTIVE, this);
        if (start_token != tokens->end())
            ++start_token;
    }
}

void ConfigParser::validateServerConfig()
{
    ServerConfiguration &server_conf = *config;
    if (!server_conf[directives[LISTEN]])
        throw ConfigParserException(E_LISTEN_DIRECTIVE_MISSING, this);
    if (!server_conf[directives[SERVER_NAME]])
    {
        deque<string> &server_name = *(server_conf[directives[LISTEN]]->getConfigValue());
        ServerConfiguration temp(server_name[0] + ":" + server_name[1]);
        server_conf.pushSubdirective(directives[SERVER_NAME], temp);
        temp.setToNull();
    }
    if (!server_conf[directives[ALLOW_METHODS]])
    {
        ServerConfiguration temp(directives[ALLOW_METHODS]);
        temp.pushConfValue("GET");
        temp.pushConfValue("POST");
        temp.pushConfValue("DELETE");
        server_conf.pushSubdirective(directives[ALLOW_METHODS], temp);
        temp.setToNull();
    }
    if (!server_conf[directives[ROOT]])
    {
        ServerConfiguration temp(DEFAULT_ROOT);
        server_conf.pushSubdirective(directives[ROOT], temp);
        temp.setToNull();
    }
    deque<string> &server_name = *(server_conf[directives[SERVER_NAME]]->getConfigValue());
    server_conf.pushConfValue(server_name[0]);
    server_conf.normalizeLocations();
}

void ConfigParser::parseServerDirective(TokenIt &start_token)
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
    parseConfig(start_token);
    validateServerConfig();
    config = NULL;
}

void ConfigParser::parseListenDirective(TokenIt &start_token)
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
        config->pushSubdirective(directives[LISTEN], host_port);
        host_port.setToNull();
        std::advance(start_token, 1);
        if (start_token == tokens->end() || *start_token != ";")
            throw PH::PHException();
    }
    catch (const  exception &e)
    {
        throw ConfigParserException(E_LISTEN_DIRECTIVE, this);
    }
}

void    ConfigParser::parseServerNameDirective(TokenIt &start_token)
{
    if (depth != 1)
        throw ConfigParserException(E_SERVER_NAME_DIRECTIVE, this);
    std::advance(start_token,1);
    if (start_token == tokens->end() || PH::strIsBreakToken(*start_token))
        throw ConfigParserException(E_SERVER_NAME_DIRECTIVE, this);
    try
    {
        ServerConfiguration temp(*start_token);
        config->pushSubdirective(directives[SERVER_NAME], temp);
        temp.setToNull();
    }
    catch(const exception &e)
    {
        throw ConfigParserException(E_SERVER_NAME_DIRECTIVE, this);
    }
    std::advance(start_token, 1);
    if (start_token == tokens->end() || *start_token != ";")
        throw ConfigParserException(E_SERVER_NAME_DIRECTIVE, this);
}

void    ConfigParser::parseLocationDirective(TokenIt &start_token)
{
    if (depth != 1)
        throw ConfigParserException(E_LOCATION_DIRECTIVE, this);
    std::advance(start_token, 1);
    if (start_token == tokens->end() || !PH::strIsPath(*start_token))
        throw ConfigParserException(E_LOCATION_DIRECTIVE, this);
    ServerConfiguration *root_conf = config;
    try
    {
        ServerConfiguration temp(directives[LOCATION]);
        temp.pushConfValue(*start_token);
        config->pushSubdirective(*start_token, temp);
        temp.setToNull();
        config = (*config)[*start_token];
        std::advance(start_token, 1);
        if (start_token == tokens->end() || *start_token != "{")
            throw ConfigParserException(E_LOCATION_DIRECTIVE, NULL);
        std::advance(start_token, 1);
        ++depth;
        parseConfig(start_token);
        config = root_conf;
    }
    catch(const exception &e)
    {
        throw ConfigParserException(E_LOCATION_DIRECTIVE, this);
    }
}

void    ConfigParser::parseErrorPageDirective(TokenIt &start_token)
{
    if (depth < 1)
        throw ConfigParserException(E_ERROR_PAGE_DIRECTIVE, this);
    std::advance(start_token, 1);
    ServerConfiguration *subdir = NULL;
    if ((subdir = (*config)[directives[ERROR_PAGE]]))
        ;
    else
    {
        ServerConfiguration temp(directives[ERROR_PAGE]);
        config->pushSubdirective(directives[ERROR_PAGE], temp);
        temp.setToNull();
        subdir = (*config)[directives[ERROR_PAGE]];
    }
    try
    {
        TokenIt end_token = tokens->end();
        PH::parseErrorPage(start_token, end_token, subdir->getConfigValue());
    }
    catch(const std::exception& e)
    {
        throw ConfigParserException(E_ERROR_PAGE_DIRECTIVE, this);
    }
}


void    ConfigParser::parseRootDirective(TokenIt &start_token)
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
}

void    ConfigParser::parseAutoindexDirective(TokenIt &start_token)
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
}

void    ConfigParser::parseIndexDirective(TokenIt &start_token)
{
    if (depth < 1)
        throw ConfigParserException(E_INDEX_DIRECTIVE, this);
    std::advance(start_token, 1);
    if (start_token == tokens->end() || PH::strIsBreakToken(*start_token))
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
}

void    ConfigParser::parseClientMaxBodySizeDirective(TokenIt &start_token)
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
}

void    ConfigParser::parseAllowMethodsDirective(TokenIt &start_token)
{
    if (depth < 1)
        throw ConfigParserException(E_ALLOW_METHODS_DIRECTIVE, this);
    ServerConfiguration temp(directives[ALLOW_METHODS]);
    deque<string> *methods_list = temp.getConfigValue();
    std::advance(start_token, 1);
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
}

void    ConfigParser::parseCgiDirective(TokenIt &start_token)
{
    if (depth < 1)
        throw ConfigParserException(E_CGI_DIRECTIVE, this);
    std::advance(start_token, 1);
    if (start_token == tokens->end() || (*start_token != "php" && *start_token != "py"))
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
}

void    ConfigParser::parseUploadDirDirective(TokenIt &start_token)
{
    if (depth < 1)
        throw ConfigParserException(E_UPLOAD_DIR_DIRECTIVE, this);
    std::advance(start_token, 1);
    if (start_token == tokens->end() || !PH::strIsPath(*start_token))
        throw ConfigParserException(E_UPLOAD_DIR_DIRECTIVE, this);
    try
    {
        string upload_path;
        deque<string> &conf = (*config->getConfigValue());
        if (conf[0] == directives[LOCATION])
            upload_path = conf[1];
        upload_path += *start_token;
        ServerConfiguration temp(upload_path);
        config->pushSubdirective(directives[UPLOAD_DIR], temp);
        temp.setToNull();
    }
    catch(const std::exception& e)
    {
        throw ConfigParserException(E_UPLOAD_DIR_DIRECTIVE, this);
    }
    std::advance(start_token, 1);
    if (start_token == tokens->end() || *start_token != ";")
        throw ConfigParserException(E_UPLOAD_DIR_DIRECTIVE, this);
}

void    ConfigParser::parseRedirectionDirective(TokenIt &start_token)
{
    if (depth < 1)
        throw ConfigParserException(E_REDIRECTION_DIRECTIVE, this);
    std::advance(start_token, 1);
    if (start_token == tokens->end() || PH::getHttpCodeType(*start_token) == UNVALID_CODE || PH::getHttpCodeType(*start_token) != REDIRECTION_CODE)
        throw ConfigParserException(E_REDIRECTION_DIRECTIVE, this);
    try{
        ServerConfiguration temp(*start_token);
        config->pushSubdirective(directives[REDIRECTION], temp);
        deque<string> *redirection_list = temp.getConfigValue();
        temp.setToNull();
        std::advance(start_token, 1);
        if (start_token == tokens->end() || PH::strIsBreakToken(*start_token))
            throw PH::PHException();
        redirection_list->push_back(*start_token);
    }
    catch (const exception &e)
    {
        throw ConfigParserException(E_REDIRECTION_DIRECTIVE, this);
    }
    std::advance(start_token, 1);
    if (start_token == tokens->end() || *start_token != ";")
        throw ConfigParserException(E_REDIRECTION_DIRECTIVE, this);
}

void ConfigParser::generateServerSockets()
{
    server_sockets = new deque<ServerSocket>();
    for (size_t i= 0; i < servers.size() ; i++)
    {
        if (std::find(similar_servers.begin(), similar_servers.end(), i) !=  similar_servers.end())
            continue;
        ServerConfiguration *curr_dir = servers[i][directives[LISTEN]];
        deque<string> *listen_val = **curr_dir;
        string host_val = (*listen_val)[0], port_val= (*listen_val)[1];
        ServerSocket *new_server = new ServerSocket(servers[i], host_val.c_str(), port_val.c_str());
        servers[i].setToNull();
        new_server->sockBind();
        new_server->sockListen();
        server_sockets->push_back(*new_server);
        for (size_t j = i + 1; j < servers.size(); j++)
        {
            ServerConfiguration *tmp_dir = servers[j][directives[LISTEN]];
            deque<string> *tmp_listen_val = **tmp_dir;
            string tmp_host = (*tmp_listen_val)[0], tmp_port = (*tmp_listen_val)[1];
            if(tmp_host == host_val && tmp_port == port_val)
            {
                new_server->pushServerConfig(servers[j]);
                servers[j].setToNull();
                similar_servers.push_back(j);
            }
        }
        new_server->nullify();
        delete new_server;
    } 
}

deque<ServerSocket> *ConfigParser::getServerSockets()
{
    deque<ServerSocket> *temp = server_sockets;
    server_sockets = NULL;
    return temp;   
}

void ConfigParser::debug_print_servers()
{
    deque<ServerConfiguration>::iterator it = servers.begin();
    deque<ServerConfiguration>::iterator end = servers.end();
    while (it != end)
    {
        it->debug_print_directives();
        ++it;
    }   
}

ConfigParser::~ConfigParser()
{
    if (tokens)
        delete tokens;
    if (server_sockets)
        delete server_sockets;
}
