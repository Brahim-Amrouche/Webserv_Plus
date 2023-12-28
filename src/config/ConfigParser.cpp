/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 06:28:14 by bamrouch          #+#    #+#             */
/*   Updated: 2023/12/28 07:05:07 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

ConfigParser::ConfigParser(list<string> *new_tokens):tokens(new_tokens)
{}

void    ConfigParser::parseConfig()
{
    TokenIt itbeg = tokens->begin(), itend = tokens->end();
    while (itbeg != itend )
    {
        if (*itbeg == "server" && servers.size() != 0)
            servers.push_back(ServerConfiguration("server"));
        else if (*itbeg == "location")
            ;
        else
            throw std::runtime_error("invalid directive: " + *itbeg);
    }
}

ConfigParser::~ConfigParser()
{
    delete tokens;
}