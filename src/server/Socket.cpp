
#include "webserv.hpp"

Socket::SocketExceptions::SocketExceptions(const socket_errors &err, Socket *cln):TException("Socket Error: ",err, cln)
{
    switch (err_c)
    {
        case E_ADDRESS_LOOKUP:
            msg += "Address Lookup Failed";
            break;
        case E_SOCKET_OPEN:
            msg += "Openning New Socket Failed";
            break;
        case E_SOCKET_OPT:
            msg += "Setting Socket Options Failed";
            break;
        case E_SOCKET_BIND_FAILED:
            msg += "Binding Socket Failed";
            break;
        case E_LISTEN_FAILED:
            msg += "Listening To Socket Failed";
            break;
        case E_SOCKET_ACCEPT:
            msg += "Accepting New Socket Failed";
            break;
        default:
            msg += "Unknown error";    
    }
}

Socket::Socket():sock_id(-1), sock_addr_len(0)
{
    FT::memset(&(sock_addr), 0, sizeof(SOCK_ADDR_STORAGE));
}

Socket::Socket(const char *host, const char *port)
{
    ADDRESS_INFO hints;
    FT::memset(&hints, 0, sizeof(ADDRESS_INFO));
    hints.ai_family = IPV4_AND_6;
    hints.ai_socktype = TCP_SOCK;
    hints.ai_flags = LISTEN_ON_HOST_NET;

    ADDRESS_INFO *bind_address = NULL;
    cout << "Looking for address info..." << endl;
    int dns_lookup_res = getaddrinfo(host, port, &hints, &bind_address);
    if (dns_lookup_res > 0)
        throw Socket::SocketExceptions(E_ADDRESS_LOOKUP, NULL);
    FT::memcpy(&sock_addr, bind_address->ai_addr, sizeof(SOCK_ADDR_STORAGE));
    sock_addr_len = bind_address->ai_addrlen;
    cout << "Address info found" << endl;
    cout << "Openning new TCP socket ..." << endl;
    sock_id = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);
    freeaddrinfo(bind_address);
    if (!ISVALIDSOCKET(sock_id))
        throw Socket::SocketExceptions(E_SOCKET_OPEN, NULL);
    int opt = 1;
    if (setsockopt(sock_id, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
        throw Socket::SocketExceptions(E_SOCKET_OPEN, NULL);
}

Socket::Socket(const Socket &cpy_sock)
{
    Socket::operator=(cpy_sock);
}


Socket &Socket::operator=(const Socket &eq_sock)
{
    if (this != &eq_sock)
    {
        sock_id = eq_sock.sock_id;
        sock_addr = eq_sock.sock_addr;
        sock_addr_len = eq_sock.sock_addr_len;
    }
    return *this;
}

void Socket::sockBind()
{
    if (bind(sock_id, &sock_addr, sock_addr_len))
        throw Socket::SocketExceptions(E_SOCKET_BIND_FAILED, this);
    cout << "Binding successfull" << endl;
}

void Socket::sockListen()
{
    if (listen(sock_id, SOMAXCONN))
        throw Socket::SocketExceptions(E_LISTEN_FAILED, this);
    cout << "Socket listening" << endl;
}

Socket *Socket::sockAccept()
{
    cout << "Accepting a new connection" << endl;
    Socket *client_socket = new Socket();
    if (!client_socket)
        throw Socket::SocketExceptions(E_SOCKET_ACCEPT, this);
    client_socket->sock_id = accept(sock_id, 
        &(client_socket->sock_addr), &(client_socket->sock_addr_len));
    if (!ISVALIDSOCKET(client_socket->sock_id))
        throw Socket::SocketExceptions(E_SOCKET_ACCEPT, client_socket);
    return client_socket;
}

void Socket::fill_epoll_event(EPOLL_EVENT *e_event, uint32_t mode)
{
    e_event->events = mode;
    e_event->data.fd = sock_id;
}

SOCKET_ID Socket::getSockid()
{
    return sock_id;
}

void Socket::nullify()
{
    sock_id = -1;
    sock_addr_len = 0;
    FT::memset(&sock_addr, 0, sizeof(SOCK_ADDR_STORAGE));
}

Socket::~Socket()
{
    if (ISVALIDSOCKET(sock_id))
        close(sock_id);
}


ServerSocket::ServerSocket(ServerConfiguration &first_server, const char *host, const char *port):Socket(host, port), configs(NULL)
{
    configs = new deque<ServerConfiguration>();
    configs->push_back(first_server);
}

ServerSocket::ServerSocket(const ServerSocket &cpy_srv_sock):Socket()
{
    ServerSocket::operator=(cpy_srv_sock);
}

ServerSocket &ServerSocket::operator=(const ServerSocket &eq_srv_sock)
{
    if (this != &eq_srv_sock)
    {
        Socket::operator=(eq_srv_sock);
        configs = eq_srv_sock.configs;
    }
    return (*this);
}

ServerConfiguration *ServerSocket::operator[](string &host)
{
    for (deque<ServerConfiguration>::iterator it = configs->begin(); it != configs->end(); it++)
    {
        ServerConfiguration *server_name_dir = (*it)[directives[SERVER_NAME]];
        deque<string> *server_name = **server_name_dir;
        if ((*server_name)[0] == host)
            return &(*it);
    }
    return  &(*configs->begin());
}

void ServerSocket::nullify()
{
    Socket::nullify();
    configs = NULL;
}

void ServerSocket::pushServerConfig(ServerConfiguration &new_config)
{
    configs->push_back(new_config);
}

ServerSocket::~ServerSocket()
{
    if (configs)
        delete configs;
    cout << "Server Socket Closed"  << endl;
}