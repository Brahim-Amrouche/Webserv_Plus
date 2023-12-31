/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_config.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 03:16:57 by bamrouch          #+#    #+#             */
/*   Updated: 2023/12/31 18:40:51 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void    configure(const char *path)
{
    TokenizeInput *tokenizer = NULL;
    list<string> *tokens = NULL;
    try
    {
        tokenizer = new TokenizeInput(path);
        tokens = tokenizer->getTokensList();
        // ConfigParser::TokenIt start = tokens->begin();
        // ConfigParser::TokenIt end  = tokens->end();
        // while (start != end)
        // {
        //     std::cout << *start << std::endl;
        //     start++;
        // }
    }
    catch(const TokenizeInput::TokenizeInputExceptions& e)
    {  
        cout << e.what() << endl;
        return ;
    }
    
    try 
    {
        // delete tokens;
        ConfigParser *parser = new ConfigParser(tokens);
        ConfigParser::TokenIt it = parser->getTokenStart();
        parser->parseConfig(it);
        parser->debug_print_servers();
        delete parser;
    }
    catch (const ConfigParser::ConfigParserException &e)
    {
        cout << e.what() << endl;
        delete tokenizer;
        return ;
    }
    delete tokenizer;
}