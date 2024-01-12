/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Path.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 16:25:01 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/12 21:27:09 by bamrouch         ###   ########.fr       */
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
        Path &operator=(const string &new_path);
        Path &operator=(const Path &new_path);
        Path &operator+(const string &path);
        Path &operator+=(const string &path);
        string operator*();
        string getFileRoute();
        bool isSubPath(Path subpath) const;
        bool isPythonFile();
        bool isPhpFile();
        bool operator==(const string &path) const;
        bool operator==(const Path &path) const;
        bool isFile();
        bool isDir();
        ~Path();
};