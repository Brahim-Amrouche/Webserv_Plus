
#include "webserv.hpp"

void Cgi::setQueryParams(Path &script, Path &req_path)
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
    env.push_back(query_params);
    string path_info = (*req_path).substr(0, slash_pos + 1) + last_path.substr(0, qmark_pos);
    req_path = path_info;
    slash_pos = (*script).find_last_of('/');
    last_path = (*script).substr(slash_pos + 1);
    qmark_pos = last_path.find('?');
    path_info = (*script).substr(0, slash_pos + 1) + last_path.substr(0,qmark_pos);
    script = path_info;
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
    setQueryParams(script, req_path);
    temp_env = "SCRIPT_NAME=";
    temp_env += *script;
    env.push_back(temp_env);
    temp_env = "PATH_INFO=";
    temp_env += *req_path;
    env.push_back(temp_env);
    temp_env = "PATH_TRANSLATED=";
    temp_env += root_path + *req_path;
    env.push_back(temp_env);
    temp_env = "REDIRECT_STATUS=CGI";
    env.push_back(temp_env);
    temp_env = "AUTH_TYPE=";
    env.push_back(temp_env);
    ostringstream oss;
    temp_env = "CONTENT_LENGTH=";
    oss << req.getBodySize();
    temp_env += oss.str();
    env.push_back(temp_env);
    temp_env = "CONTENT_TYPE=";
    temp_env += req[CONTENT_TYPE];
    env.push_back(temp_env);
    temp_env = "HTTP_COOKIE=";
    temp_env += req[COOKIE];
    env.push_back(temp_env);
}

const char *php_execution_args[3] = {PHP_CGI_PATH, NULL, NULL};
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
    int res_file = open(cgi_output.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (res_file < 0)
        exit(1);
    if (dup2(body_file, STDIN_FILENO) == -1  || dup2(res_file, STDOUT_FILENO) == -1 || dup2(res_file, STDERR_FILENO) == -1)
        exit(1);
    close(body_file);
    close(res_file);
    string script_name = script_path.getFileRoute();
    --script_path;
    script_path = string(".") + (*script_path);
    if (chdir(root_path.c_str()) != 0  || chdir((*script_path).c_str()) != 0)
        exit(1);
    if (lang == L_PHP)
    {
        php_execution_args[1] = script_name.c_str();
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
    cgi_done = false;
    setEnv(script_path, req_path);
    cgi_output = req.getReqId() + ".res";
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

void Cgi::validateHeaders(size_t &read_size)
{
    ifstream ifs(cgi_output.c_str(), std::ios::binary | std::ios::ate);
    ssize_t end_pos = ifs.tellg();
    file.getFileSize() = end_pos;
    if (headers.find("Content-Length") == headers.end())
    {
        stringstream ss;
        ss << (end_pos - read_size);
        headers.insert(std::pair<string, string>("Content-Length", ss.str()));
    }
    ifs.close();
    if (headers.find("Content-Type") == headers.end() && headers.find("Content-type") == headers.end())
        headers.insert(std::pair<string, string>("Content-Type", "application/octet-stream"));
    if (headers.find("Status") == headers.end())
        headers.insert(std::pair<string, string>("Status", "200 OK"));
    else
    {
        string status = headers["Status"];
        size_t space_pos = status.find_first_of(' ');
        string code = status.substr(0, space_pos);
        response_code status_code = RES_NONE;
        if (!(status_code = PH::getHttpCode(code)))
            throw Response::ResponseException(E_FAILED_CGI_EXEC, NULL);
        string status_handle = status.substr(space_pos + 1);
        if (!status_handle.size())
            status_handle = response_handle[status_code];
        else if (status_handle !=  response_handle[status_code])
            status_handle = response_handle[status_code];
        headers["Status"] = code + " " + status_handle;
    }
}

void Cgi::pushHeaders()
{
    string status_line("HTTP/1.1 ");
    status_line += headers["Status"] + "\r\n";
    RESH::pushHeaders(buffer, status_line, buffer_size);
    for (map<string, string>::iterator it = headers.begin(); it != headers.end(); it++)
    {
        string header = it->first + ": " + it->second + "\r\n";
        RESH::pushHeaders(buffer, header, buffer_size);
    }
    string date = RESH::getDateHeader();
    RESH::pushHeaders(buffer, date, buffer_size);
    string server = "Server: webserv/1.0\r\n";
    RESH::pushHeaders(buffer, server, buffer_size);
    string close_connection = RESH::getCloseConnectionHeader();
    RESH::pushHeaders(buffer, close_connection, buffer_size);
    RESH::pushHeaders(buffer, "\r\n", buffer_size);
}

void Cgi::parseHeaders()
{
    ifstream res_file (cgi_output.c_str(), std::ios::in | std::ios::binary);
    if (!res_file.is_open())
        throw Response::ResponseException(E_FAILED_CGI_EXEC, NULL);
    res_file.read(buffer, HEADERS_MAX_SIZE);
    string keyval;
    size_t read_size = 0;
    size_t read_count = res_file.gcount();
    for (size_t i = 0; i < read_count; i+= 2)
    {
        size_t start = i;
        while (i < read_count && !(buffer[i] == '\r' 
            && i + 1 < read_count && buffer[i + 1] == '\n'))
            i++;
        if (i == read_count)
            throw Response::ResponseException(E_FAILED_CGI_EXEC, NULL);
        if (i == start)
        {
            file.setFilePath(cgi_output);
            file.setReadSize(i + 2);
            read_size = i + 2;
            break;
        }
        keyval = string(buffer + start, i - start);
        size_t Pos = keyval.find(": ");
        if (Pos == string::npos)
            throw Response::ResponseException(E_FAILED_CGI_EXEC, NULL);
        headers.insert(std::pair<string, string>(keyval.substr(0, Pos), keyval.substr(Pos + 2)));
    }
    res_file.close();
    validateHeaders(read_size);
    pushHeaders();
}

bool Cgi::isDone()
{
    if (cgi_done)
        return true;
    int pid;
    pid = waitpid(proc_id, &status, WNOHANG);
    if (pid == -1)
        throw Response::ResponseException(E_FAILED_CGI_EXEC, NULL);
    if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
        throw Response::ResponseException(E_FAILED_CGI_EXEC, NULL);
    if (pid > 0)
    {
        proc_id = 0;
        parseHeaders();
        cgi_done = true;
    }
    return cgi_done;
}

Cgi::~Cgi()
{
    if (proc_id > 0)
    {
        kill(proc_id, SIGKILL);
        waitpid(proc_id, &status, WUNTRACED);
    }
    remove(cgi_output.c_str());
}
