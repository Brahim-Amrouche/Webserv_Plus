/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_config.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 03:16:57 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/01 22:17:57 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

deque<ServerSocket>   *configure(const char *path)
{
    TokenizeInput *tokenizer = NULL;
    list<string> *tokens = NULL;
    try
    {
        tokenizer = new TokenizeInput(path);
        tokens = tokenizer->getTokensList();
        delete tokenizer;
        tokenizer = NULL;
    }
    catch(const TokenizeInput::TokenizeInputExceptions& e)
    {  
        cout << e.what() << endl;
        return NULL;
    }
    ConfigParser *parser = new ConfigParser(tokens);
    deque<ServerSocket> *server_sockets = NULL;
    try 
    {
        ConfigParser::TokenIt it = parser->getTokenStart();
        parser->parseConfig(it);
        parser->generateServerSockets();
        server_sockets = parser->getServerSockets();
        parser->debug_print_servers();
        delete parser;
        parser = NULL;
    }
    catch (const ConfigParser::ConfigParserException &e)
    {
        cout << e.what() << endl;
        return NULL;
    }
    catch (const Socket::SocketExceptions &e)
    {
        cout << e.what() << endl;
        delete parser;
        return NULL;
    }
    cleanup_data.cleanup_server_sock = server_sockets;
    return server_sockets;
}