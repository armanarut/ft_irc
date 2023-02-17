#include "Server.hpp"

Server::Server(short port, const std::string& pass)
    :_port(port),
    _pass(pass),
    _commandHandler(new CommandHandler(this))
{
    init_server();
    start();
}

Server::~Server()
{
    for (iterator it = _client.begin(); it != _client.end(); ++it)
    {
        delete it->second;
        close(it->first);
    }
    close(server_fd);
}

short   Server::getPort() const
{
    return _port;
}

std::string Server::getPass()
{
    return _pass;
}

Client* Server::getClient(const std::string& nickname)
{
    try
    {
        return _client.at(_user.at(nickname));
    }
    catch(const std::exception& e)
    {
        return NULL;
    }
}

Channel*    Server::getChannel(const std::string& name)
{
    try
    {
        return _channel.at(name);
    }
    catch(const std::exception& e)
    {
        return NULL;
    }
}

Channel*    Server::addChannel(const std::string& name, const std::string& pass)
{
    Channel* newChannel = new Channel(name, pass);

    _channel.insert(std::make_pair(name, newChannel));
    return newChannel;
}

void    Server::setUser(Client* client, const std::string& nick, int fd)
{
    std::string old_nick = client->getNick();
    if (!(old_nick ==  ""))
        for (iterator it = _client.begin(); it != _client.end(); ++it)
            if (it->second != client)
                it->second->sending(":" + old_nick + " NICK " + nick);
    _user.erase(old_nick);
    _user.erase(nick);
    _user.insert(std::make_pair(nick, fd));
}

/****************[init server]****************/
void    Server::init_server()
{
    int opt = 1;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
        prog_error("socket failed");

    if (setsockopt(server_fd, SOL_SOCKET, \
        SO_REUSEADDR, &opt, sizeof(opt)))
        prog_error("setsockopt");
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(_port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
        prog_error("bind failed");

    if (listen(server_fd, 100) < 0)
        prog_error("listen");

    fcntl(server_fd, F_SETFL, O_NONBLOCK);
}

/****************[make select]****************/
void    Server::start()
{
    int     r, nfds = server_fd;
    fd_set  rd, wr, er;
    timeval tv;

    tv.tv_sec = 0;
    tv.tv_usec = 300000;
    FD_ZERO(&rd);
    FD_ZERO(&wr);
    FD_ZERO(&er);
    for(;;)
    {
        if (_client.size())
        {
            for (iterator it = _client.begin(); it != _client.end(); ++it)
                FD_SET(it->first, &rd);
            nfds = std::max(nfds, _client.rbegin()->first);
        }

        r = select(nfds + 1, &rd, &wr, &er, &tv);
        // if (r == -1 && errno == EINTR)
        //     break;//continue;
        if (r == -1)
            prog_error("select");
        else if (r)
        {
            // cout << "continue..." << endl;
            for (iterator it = _client.begin(); it != _client.end(); ++it)
            {
                /*************[writing]*************/
                if (FD_ISSET(it->first, &wr)) ///<---------------------------------
                {
                    // cout << "writing..." << endl;
                    FD_CLR(it->first, &wr);
                    while (!(it->second->buffer).empty())
                        _commandHandler->invoke(it->second);

                    it->second->buffer.clear();
                    if ( it->second->quit) {
                        delete_user(it);
                        break ;
                    }
                }
                /*************[reading]*************/
                else if (FD_ISSET(it->first, &rd))
                {
                    // std::cout << "client quit " << it->second->quit << std::endl;
                    FD_CLR(it->first, &rd);
                    if (!get_buffer(it))
                        break ;
                    FD_SET(it->first, &wr);
                }
            }
        }
        else
        {
            // cout << "no data..." << endl;
            new_client();
        }
    }
}

bool    Server::get_buffer(iterator& it)
{
    char    buffer[BUF_SIZE];
    int     valread;

    while ((memset(buffer, 0, BUF_SIZE), \
            valread = recv(it->first, buffer, BUF_SIZE, 0)) != -1)
    {
        if (valread == 0) ////////<----------------------------------
        {
            delete_user(it);
            return valread;
        }
        it->second->buffer.append(buffer);
    }
    return valread;
}

/****************[create new client]****************/
void    Server::new_client()
{
    sockaddr_in client_address;
    socklen_t   client_addrlen = sizeof(client_address);

    memset(&address, 0, client_addrlen);
    int new_socket = accept(server_fd, (struct sockaddr*)&client_address, &client_addrlen);
    if (new_socket == -1)
        return ;
    fcntl(new_socket, F_SETFL, O_NONBLOCK);
    char hostname[NI_MAXHOST];

    getnameinfo((struct sockaddr*)&client_address, sizeof(client_address), hostname, NI_MAXHOST, NULL, 0, NI_NUMERICSERV);
    Client* n_client = new Client(new_socket, hostname);
    _client.insert(std::make_pair(new_socket, n_client));
    std::cout << "New connection: " << n_client->getPrefix() << std::endl;
    std::cout << "Users connection: " << _client.size() << std::endl;
}

void    Server::delete_user(iterator& it){

    std::cout << "User disconnected: " << it->second->getPrefix() << std::endl;

    it->second->leaveChannel(0);

    close(it->first);
    _user.erase(it->second->getNick());

    delete it->second;
    _client.erase(it);
    std::cout << "Users connection: " << _client.size() << std::endl;
}

// void    Server::checkClientFd()
// {
//      for (std::map<int, Client*>::iterator it = _client.begin(); it != _client.end(); ++it)
//     {
//         if (it->second->quit)
//         {
//             close (it->first);
//             delete_user(it->second);
//             it = _client.begin();
//         }
//     }
// }