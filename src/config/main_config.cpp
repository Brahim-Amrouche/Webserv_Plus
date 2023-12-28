/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_config.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 03:16:57 by bamrouch          #+#    #+#             */
/*   Updated: 2023/12/28 03:52:56 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void    configure(const char *path)
{
    (void)path;
    try
    {
        TokenizeInput *tokenizer = new TokenizeInput(path);
        list<string> *tokens = tokenizer->getTokensList();
        while(!tokens->empty())
        {
            cout << tokens->front() << endl;
            tokens->pop_front();
        }
        delete tokens;
        delete tokenizer;
    }
    catch(const TokenizeInput::TokenizeInputExceptions &e)
    {
        std::cout << e.what() << endl;
    }
    
    return ;
}