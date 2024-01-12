/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 16:03:03 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/12 19:43:34 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Response.hpp"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

enum CGI_LANG
{
    UNKNOWN,
    L_PHP,
    L_PYTHON,
};

class Cgi
{
    private:
        char (&buffer)[HEADERS_MAX_SIZE + 1];
        deque<string> env;
        Request &req;
        string &fs_root;
        Path  cgi_req_path;
        int   proc_id;
        int   status;
        CGI_LANG lang;
        bool    cgi_done;
    public:
        Cgi(char (&b)[HEADERS_MAX_SIZE + 1], Request &r, string &root):buffer(b) ,req(r)
            , fs_root(root), proc_id(-1), status(0) , cgi_done(false){};
        void setLang(const CGI_LANG &l)
        {
            lang = l;
        };
        void setEnv();
        void exec();
        bool isDone();
        bool init(Path &path);
        ~Cgi();
};
