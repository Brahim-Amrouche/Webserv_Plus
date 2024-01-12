/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 21:48:20 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/12 21:28:56 by bamrouch         ###   ########.fr       */
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
    temp_env += req.getClientSock().getIp();
    env.push_back(temp_env);
    temp_env = "SERVER_NAME=";
    temp_env += req.getServerSock().getIp();
    env.push_back(temp_env);
    temp_env = "SERVER_PORT=";
    temp_env += req.getServerSock().getPort();
    env.push_back(temp_env);
    temp_env = "SCRIPT_NAME=";
    temp_env += *script;
    env.push_back(temp_env);
    setQueryParams(req_path);
    temp_env = "PATH_INFO=";
    temp_env += *req_path;
    env.push_back(temp_env);
    temp_env = "PATH_TRANSLATED=";
    temp_env += *req + "/" + *req_path;
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
    const char **p_env = new char*[this->env.size() + 1];
    for (size_t i = 0; i < env.size(); i++)
        p_env[i] = env[i].c_str();    
    p_env[env.size()] = NULL;
    int body_file = open(req.getReqId().c_str(), O_RDONLY);
    if (body_file < 0)
        exit(1);
    string res_file_path = req.getReqId() + ".res";
    int res_file = open(res_file_path.c_str(), O_CREAT | O_WRONLY);
    if (res_file < 0)
        exit(1);
    dup2(body_file, 0);
    dup2(res_file, 1);
    string script_route = script_path.getFileRoute();
    --script_path;
    if (chdir((*script_path).c_str()) != 0)
        exit(1);
    if (lang == L_PHP)
    {
        php_execution_args[2] = script_route.c_str();
        execve(php_execution_args[0], (char * const *)php_execution_args, (char * const *)p_env);
    }
    else if (lang == L_PYTHON)
    {
        py_execution_args[1] = script_route.c_str();
        execve(py_execution_args[0], (char * const *)py_execution_args, (char * const *)p_env);
    }
    exit(1);
}



bool Cgi::init(Path &script_path, Path &req_path)
{
    
    proc_id = fork();
    switch (proc_id)
    {
        case -1:
            throw Response::ResponseException(E_FAILED_CGI_EXEC, NULL);
        case 0:
            exec();
        default:
           return isDone();
    }
}

bool Cgi::isDone()
{
    proc_id = waitpid(proc_id, &status, WNOHANG);
    if (proc_id == -1)
        throw Response::ResponseException(E_FAILED_CGI_EXEC, NULL);
    if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
        throw Response::ResponseException(E_FAILED_CGI_EXEC, NULL);
    if (proc_id != 0)
        cgi_done = true;
    return cgi_done;
}

Cgi::~Cgi()
{
    // remove(cgi_output.c_str());
}
