#pragma once

#include <iostream>
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

    void    start()
    {
        int server_fd, new_socket, valread;
        struct sockaddr_in address;
        int opt = 1;
        int addrlen = sizeof(address);
        char buffer[1024] = { 0 };
        char hello[] = "Hello from server";

        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
            ft_exit("socket failed");

        if (!setsockopt(server_fd, SOL_SOCKET, \
            SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
            ft_exit("setsockopt");
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(_port);

        if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
            ft_exit("bind failed");

        if (listen(server_fd, 3) < 0)
            ft_exit("listen");

        if ((new_socket = accept(server_fd, (struct sockaddr*)&address,
                  (socklen_t*)&addrlen)) < 0)
            ft_exit("accept");

        valread = read(new_socket, buffer, 1024);
        printf("%s\n", buffer);
        send(new_socket, hello, strlen(hello), 0);
        printf("Hello message sent\n");

        //_client.insert(std::make_pair(server_fd, Client()));
    };


    Server();
    Server& operator=(const Server& other);
};