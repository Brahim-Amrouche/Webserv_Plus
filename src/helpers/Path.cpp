
#include "webserv.hpp"

Path::Path()
{
    value = "";
}

Path::Path(const std::string &path)
{
    value = path;
}

Path::Path(const Path &path)
{
    value = path.value;
}

Path &Path::operator=(const string &new_path)
{
    value = new_path;
    return (*this);
}

Path &Path::operator=(const Path &new_path)
{
    value = new_path.value;
    return (*this);
}

Path &Path::operator+(const string &path)
{
    value += path;
    return (*this);
}

Path &Path::operator+=(const string &path)
{
    return ((*this) + path);
}

bool Path::isFile()
{
    struct stat st;
    if (stat(value.c_str(), &st) == 0)
        return S_ISREG(st.st_mode);
    return false;
}

bool Path::isDir()
{
    struct stat st;
    if (stat(value.c_str(), &st) == 0)
        return S_ISDIR(st.st_mode);
    return false;

}

string Path::extract_subpath(const string &subpath)
{
    if (this->value == "/" || subpath == "/")
        return this->value;
    size_t found_subpath = this->value.find(subpath);
    if (found_subpath == string::npos)
        return this->value;
    return this->value.substr(subpath.size());
}

bool Path::isSubPath(Path subpath) const
{
    while (subpath.value != "")
    {
        if (subpath.value == this->value)
            return true;
        --subpath;
    }
    return false;
}

CGI_LANG Path::isCgiPath(Path req_path, Path &script)
{
    Path tmp(*this);
    string rm_query_strings = *tmp;
    size_t q_pos = 0;
    if ((q_pos = rm_query_strings.find_first_of("?")) != string::npos)
        rm_query_strings = rm_query_strings.substr(0, q_pos);
    tmp = rm_query_strings;
    while (req_path.value != "")
    {
        if (tmp.isPhpFile())
        {
            script = req_path;
            return L_PHP;
        }
        else if (tmp.isPythonFile())
        {
            script = req_path;
            return L_PYTHON;
        }
        --tmp;
        --req_path;
    }
    return L_UNKNOWN;
}

bool Path::isPythonFile()
{
    if (!this->isFile())
        return false;
    size_t pos = value.find_last_of('.');
    if (pos != string::npos && value.substr(pos) == ".py")
        return true;
    return false;
}

bool Path::isPhpFile()
{
    if (!this->isFile())
        return false;
    size_t pos = value.find_last_of('.');
    if (pos != string::npos && value.substr(pos) == ".php")
        return true;
    return false;
}


bool Path::operator==(const string &path) const
{
    return value == path;
}

bool Path::operator==(const Path &path) const
{
    return value == path.value;
}

void Path::operator--()
{
    size_t pos = value.find_last_of('/');
    if (value == "/")
        value = "";
    else
        value = value.substr(0, pos ? pos : 1);
}

string Path::operator*()
{
    return value;
}

string Path::getFileRoute()
{
    size_t pos = value.find_last_of('/');
    if (pos == string::npos)
        return value;
    return value.substr(pos + 1);
}

Path::~Path()
{}