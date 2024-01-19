
#pragma once
#include "Response.hpp"
#include <fstream>

using std::ifstream;

class File
{
    private:
        ssize_t &buffer_size;
        ssize_t read_size;
        ssize_t file_size;
        ifstream file;
        string path;
        bool file_done;
    public :
        File(ssize_t &buf_size);
        void setFilePath(const string &p);
        void setFileDone(const bool &done);
        void setReadSize(const ssize_t &size);
        ssize_t &getFileSize();
        bool operator*();
        void operator>>(char (&buf)[HEADERS_MAX_SIZE + 1]);
        ~File();
};