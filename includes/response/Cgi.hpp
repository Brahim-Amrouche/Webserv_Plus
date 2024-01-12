/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 16:03:03 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/12 21:24:07 by bamrouch         ###   ########.fr       */
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
        int   proc_id;
        int   status;
        CGI_LANG lang;
        bool    cgi_done;
        string  cgi_output;
    public:
        Cgi(char (&b)[HEADERS_MAX_SIZE + 1], Request &r):buffer(b) ,req(r)
            , proc_id(-1), status(0) , cgi_done(false){};
        void setLang(const CGI_LANG &l)
        {
            lang = l;
        };
        void setQueryParams(Path &req_path);
        void setEnv(Path &script, Path &req_path);
        void exec(Path &script_path);
        bool isDone();
        bool init(Path &script_path, Path &req_path);
        ~Cgi();
};
