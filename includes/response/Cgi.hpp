/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 16:03:03 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/13 00:21:03 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Response.hpp"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>



class Cgi
{
    private:
        char (&buffer)[HEADERS_MAX_SIZE + 1];
        deque<string> env;
        Request &req;
        string &root_path;
        int   proc_id;
        int   status;
        CGI_LANG lang;
        bool    cgi_done;
        string  cgi_output;
    public:
        Cgi(char (&b)[HEADERS_MAX_SIZE + 1], Request &r, string &root):buffer(b) ,req(r), root_path(root)
            , proc_id(-1), status(0) , cgi_done(true){};
        void setLang(const CGI_LANG &l)
        {
            lang = l;
        };
        void setServerEnv();
        void setQueryParams(Path &req_path);
        void setEnv(Path &script, Path &req_path);
        void exec(Path &script_path);
        bool isDone();
        void init(Path &script_path, Path &req_path);
        ~Cgi();
};
