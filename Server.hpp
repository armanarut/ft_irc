#pragma once

class Server;

#include <unistd.h>
#include <fcntl.h>
#include <map>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/select.h>

#include "Client.hpp"
#include "Channel.hpp"
#include "CommandHandler.hpp"

// #include <iostream>
// #include <string>
// #include <cerrno>
// #include <cstring>
// #include <sstream>
// #include <sys/socket.h>

class Server
{
public:

    typedef std::map<int, Client*>::iterator  iterator;

    Server(short port, const std::string& pass);

    ~Server();

    std::string getPass();
    Client* getClient(const std::string& nickname);
    Channel* getChannel(const std::string& name);
    void    setUser(const std::string& nickname, int fd);

private:
    short       _port;
    std::string _pass;

    int         server_fd;
    sockaddr_in address;

    std::map<int, Client*>          _client;
    std::map<std::string, int>      _user;
	std::map<std::string, Channel*>	_channel;
	CommandHandler*	                _commandHandler;

/****************[init server]****************/
    void    init_server();

/****************[make select]****************/
    void    start();
    bool    get_buffer(iterator& it);

/****************[create new client]****************/
    void    new_client();

    Server();
    Server& operator=(const Server& other);
};
