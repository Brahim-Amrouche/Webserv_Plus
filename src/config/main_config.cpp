/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_config.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 03:16:57 by bamrouch          #+#    #+#             */
/*   Updated: 2023/12/28 06:25:14 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void    configure(const char *path)
{
    TokenizeInput *tokenizer = new TokenizeInput(path);
    ConfigParser *parser = new ConfigParser(tokenizer->getTokensList());
    
    delete tokenizer;
    return ;
}