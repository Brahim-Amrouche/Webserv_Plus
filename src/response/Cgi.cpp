/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 21:48:20 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/13 02:45:42 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void Cgi::setQueryParams(Path &req_path)
{
    size_t slash_pos = (*req_path).find_last_of('/');
    if (slash_pos == string::npos)
        return;
    string last_path = (*req_path).substr(slash_pos + 1);
    size_t qmark_pos = last_path.find('?');
    if (qmark_pos == string::npos)
        return;
    string query_params = "QUERY_STRING=";
    query_params += last_path.substr(qmark_pos + 1);
    string path_info = (*req_path).substr(0, slash_pos + 1) + last_path.substr(0, qmark_pos);
    req_path = path_info;
}

void Cgi::setServerEnv()
{
    string temp_env("SERVER_NAME=");
    ServerConfiguration *server_conf = req.getServerConfig();
    deque<string> *server_name = **(*server_conf)[directives[SERVER_NAME]];
    temp_env += (*server_name)[0];
    env.push_back(temp_env);
    temp_env = "SERVER_PORT=";
    deque<string> *server_port = **(*server_conf)[directives[LISTEN]];
    temp_env += (*server_port)[1];
    env.push_back(temp_env);
}

void Cgi::setEnv(Path &script, Path &req_path)
{
    // defaulted env
    env.push_back("SERVER_PROTOCOL=HTTP/1.1");
    env.push_back("GATEWAY_INTERFACE=CGI/1.1");
    env.push_back("SERVER_SOFTWARE=webserv/1.0");
    string temp_env;
    //must give
    temp_env = "REQUEST_METHOD=";
    temp_env += RESH::getMethodString(req.getReqMethod());
    env.push_back(temp_env);
    temp_env = "REMOTE_ADDR=";
    env.push_back(temp_env);
    setServerEnv();
    temp_env = "SCRIPT_NAME=";
    temp_env += *script;
    env.push_back(temp_env);
    setQueryParams(req_path);
    temp_env = "PATH_INFO=";
    temp_env += *req_path;
    env.push_back(temp_env);
    temp_env = "PATH_TRANSLATED=";
    temp_env += root_path + *req_path;
    env.push_back(temp_env);
    if (req.getBodySize() > 0)
    {
        ostringstream oss;
        temp_env = "CONTENT_LENGTH=";
        oss << req.getBodySize();
        temp_env += oss.str();
        env.push_back(temp_env);
    }
    if (req[CONTENT_TYPE] != "")
    {
        temp_env = "CONTENT_TYPE=";
        temp_env += req[CONTENT_TYPE];
        env.push_back(temp_env);
    }
}

const char *php_execution_args[4] = {PHP_CGI_PATH, "-f" , NULL, NULL};
const char *py_execution_args[3] = {PYTHON_CGI_PATH, NULL , NULL};

void Cgi::exec(Path &script_path)
{
    const char **p_env = new const char*[this->env.size() + 1];
    for (size_t i = 0; i < env.size(); i++)
        p_env[i] = env[i].c_str();    
    p_env[env.size()] = NULL;
    int body_file = open(req.getReqId().c_str(), O_RDONLY);
    if (body_file < 0)
        exit(1);
    string res_file_path = req.getReqId() + ".res";
    int res_file = open(res_file_path.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (res_file < 0)
        exit(1);
    if (dup2(body_file, 0) == -1  || dup2(res_file, 1) == -1 )
        exit(1);
    string script_name = script_path.getFileRoute();
    --script_path;
    script_path = string(".") + (*script_path);
    if (chdir(root_path.c_str()) != 0  || chdir((*script_path).c_str()) != 0)
        exit(1);
    if (lang == L_PHP)
    {
        php_execution_args[2] = script_name.c_str();
        execve(php_execution_args[0], (char * const *)php_execution_args, (char * const *)p_env);
    }
    else if (lang == L_PYTHON)
    {
        py_execution_args[1] = script_name.c_str();
        execve(py_execution_args[0], (char * const *)py_execution_args, (char * const *)p_env);
    }
    exit(1);
}

void Cgi::init(Path &script_path, Path &req_path)
{
    (void) buffer;
    cgi_done = false;
    setEnv(script_path, req_path);
    proc_id = fork();
    switch (proc_id)
    {
        case -1:
            throw Response::ResponseException(E_FAILED_CGI_EXEC, NULL);
        case 0:
            exec(script_path);
        default:
           return ;
    }
}

bool Cgi::isDone()
{
    if (cgi_done)
        return true;
    proc_id = waitpid(proc_id, &status, WNOHANG);
    if (proc_id == -1)
        throw Response::ResponseException(E_FAILED_CGI_EXEC, NULL);
    if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
        throw Response::ResponseException(E_FAILED_CGI_EXEC, NULL);
    if (proc_id != 0)
    {
        proc_id = 0;
        cgi_done = true;
    }
    return cgi_done;
}

Cgi::~Cgi()
{
    if (proc_id > 0)
        kill(proc_id, SIGKILL);
    string res_file(req.getReqId());
    res_file +=".res";
    remove(res_file.c_str());
    // remove(cgi_output.c_str());
}
