#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <map>
#include "Client.hpp"

#include <netinet/in.h>
#include <sys/socket.h>

class Server
{
public:
    Server(short port, const std::string& pass)
        :_port(port),
        _pass(pass)
    {
        start();
        new_client();
        get_client();
    };

    Server(const Server& other)
        :_port(other._port),
        _pass(other._pass)
    {};

    ~Server(){};
    

private:
    short _port;
    std::string _pass;
    std::map<int, Client> _client;

    int server_fd;
    struct sockaddr_in address;
    socklen_t addrlen;

/****************[init server]****************/
    void    start()
    {
        int opt = 1;

        addrlen = sizeof(address);

        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
            ft_exit("socket failed");

        if (!setsockopt(server_fd, SOL_SOCKET, \
            SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
            ft_exit("setsockopt");
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(_port);

        if (bind(server_fd, (struct sockaddr*)&address, addrlen) < 0)
            ft_exit("bind failed");

        if (listen(server_fd, 3) < 0)
            ft_exit("listen");
    };

/****************[create new client]****************/
    void    new_client()
    {
        int new_socket;

        if ((new_socket = accept(server_fd, (struct sockaddr*)&address,
                  &addrlen)) < 0)
            ft_exit("accept");
            
        _client.insert(std::make_pair(new_socket, Client()));
    }

    void    get_client()
    {
        for (std::map<int, Client>::iterator it = _client.begin(); it != _client.end(); ++it)
        {
            int valread;
            char buffer[1024];
            std::memset(buffer, 0, 1024);
            char hello[] = "Hello from server\r\n";
            while ((valread = recv(it->first, buffer, 1024, 0)))
            {
                std::cout << buffer << std::endl;
                std::memset(buffer, 0, 1024);
                send(it->first, hello, strlen(hello), 0);
                std::cout << "Hello message sent\n";
            }
            
        }
    }

    Server();
    Server& operator=(const Server& other);
};