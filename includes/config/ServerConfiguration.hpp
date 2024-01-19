
#pragma once
#include <map>
#include <string>
#include <exception>
#include <deque>
#include "Path.hpp"

using std::exception;
using std::map;
using std::string;

class ServerConfiguration
{
    private:
        deque<string> *config_values;
        map<string, ServerConfiguration> *subdirective;
    public:
        class ServerConfiguarationException : public exception
        {
            public:
                const char *what() const throw()
                {
                    return "ServerConfiguarationException";
                };
        };
        typedef map<string, ServerConfiguration>::iterator ConfigIt;
        typedef map<string, ServerConfiguration>::const_iterator ConfigConstIt;
        ServerConfiguration();
        ServerConfiguration(const string &value);
        ServerConfiguration(const map<string, ServerConfiguration> &config);
        ServerConfiguration(ServerConfiguration *cpy_config);
        ServerConfiguration(const ServerConfiguration &cpy_config);
        ServerConfiguration &operator=(const ServerConfiguration &eq_config);
        deque<string>       *getConfigValue();
        ServerConfiguration *getSubdirective(const string &path);
        void    setConfigValue(string *value);
        void    setSubdirective(map<string, ServerConfiguration> *config);
        void    setToNull();
        void    pushConfValue(const string &value);
        void    pushSubdirective(const    string &directive, const ServerConfiguration &config);
        void    normalizeLocations();
        deque<string> *operator*();
        ServerConfiguration *operator[](Path &location_path);
        ServerConfiguration *operator[](const string &dir_name);
        void    debug_print_directives();
        ~ServerConfiguration();
};

