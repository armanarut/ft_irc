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

class Server
{
public:

    typedef std::map<int, Client*>::iterator  iterator;

    Server(short port, const std::string& pass);

    ~Server();

    std::string getPass();
    Client*     getClient(const std::string& nickname);
    Channel*    getChannel(const std::string& name);
    Channel*    addChannel(const std::string& name, const std::string& pass);

    void    setUser(Client* client, const std::string& nick, int fd);
    void    delete_user(iterator& it);
    void    checkClientFd();
    short   getPort() const;

private:
    short       _port;
    std::string _pass;

    int         server_fd;
    sockaddr_in address;

    std::map<int, Client*>          _client;
    std::map<std::string, int>      _user;
	std::map<std::string, Channel*>	_channel;
	CommandHandler*	                _commandHandler;

    void    init_server();

    void    start();
    bool    get_buffer(iterator& it);

    void    new_client();

    Server();
    Server& operator=(const Server& other);
};
