
#pragma once
class File;
#include "Response.hpp"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

using std::ifstream;

class Cgi
{
    private:
        char (&buffer)[HEADERS_MAX_SIZE + 1];
        Request &req;
        string &root_path;
        File &file;
        ssize_t &buffer_size;
        int   proc_id;
        int   status;
        CGI_LANG lang;
        deque<string> env;
        map<string, string> headers;
        bool    cgi_done;
        string  cgi_output;
    public:
        Cgi(char (&buf)[HEADERS_MAX_SIZE + 1], Request &r, string &root, File &f, ssize_t &b):buffer(buf) ,req(r), root_path(root)
            , file(f), buffer_size(b), proc_id(-1), status(0) , cgi_done(true){};
        void setLang(const CGI_LANG &l)
        {
            lang = l;
        };
        void setServerEnv();
        void setQueryParams(Path &script, Path &req_path);
        void setEnv(Path &script, Path &req_path);
        void exec(Path &script_path);
        void validateHeaders(size_t &read_size);
        void pushHeaders();
        void parseHeaders();
        bool isDone();
        void setCgiDone(const bool &b)
        {
            cgi_done = b;
        };
        void init(Path &script_path, Path &req_path);
        ~Cgi();
};
