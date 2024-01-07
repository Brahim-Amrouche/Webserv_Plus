/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Path.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 16:25:01 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/07 16:50:29 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include <sys/stat.h>


using std::string;

class Path
{
    private:
        string value;
    public:
        Path();
        Path(const string &path);
        void operator--();
        string operator*();
        bool isDir();
        ~Path();
};