
#pragma once
#include <string>
#include <sys/stat.h>

using std::string;

enum CGI_LANG
{
    L_UNKNOWN,
    L_PHP,
    L_PYTHON,
};

class Path
{
    private:
        string value;
    public:
        Path();
        Path(const string &path);
        Path(const Path &path);
        void operator--();
        Path &operator=(const string &new_path);
        Path &operator=(const Path &new_path);
        Path &operator+(const string &path);
        Path &operator+=(const string &path);
        CGI_LANG isCgiPath(Path req_path, Path &script);
        string operator*();
        string getFileRoute();
        string extract_subpath(const string &sub_path);
        bool isSubPath(Path subpath) const;
        bool isPythonFile();
        bool isPhpFile();
        bool operator==(const string &path) const;
        bool operator==(const Path &path) const;
        bool isFile();
        bool isDir();
        ~Path();
};