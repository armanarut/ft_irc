#pragma once

#include <iostream>
#include <string>
#include <cerrno>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <map>
#include "Client.hpp"

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>

#define HELLO_MSG "Hello from server\r\n\r\n"
#define BUF_SIZE 1024

// typedef struct timeval {
//     time_t tv_sec;
//     long tv_usec;
// };

using namespace std;

class Server
{
    typedef std::map<int, Client>::iterator  iterator;

public:
    Server(short port, const string& pass)
        :_port(port),
        _pass(pass)
    {
        init_server();
        start();

        // get_client();
    };

    Server(const Server& other)
        :_port(other._port),
        _pass(other._pass)
    {};

    ~Server()
    {
        for (iterator it = _client.begin(); it != _client.end(); ++it)
            close(it->first);
        close(server_fd);
    };
    

private:
    short _port;
    string _pass;
    map<int, Client> _client;

    int server_fd;
    sockaddr_in address;

/****************[init server]****************/
    void    init_server()
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

        if (listen(server_fd, 10) < 0)
            prog_error("listen");
    };

/****************[make select]****************/
    void    start()
    {
        timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 10000;
        for(;;)
        {
            int r, nfds = server_fd;
            fd_set rd, wr, er;
            FD_ZERO(&rd);
            FD_ZERO(&wr);
            FD_ZERO(&er);
            if (_client.size())
            {
                for (iterator it = _client.begin(); it != _client.end(); ++it)
                {
                    if (fcntl(it->first, F_SETFL, O_NONBLOCK) == -1/* && errno == EBADF*/)
                    {
                        close(it->first);
                        _client.erase(it->first);
                    }
                    FD_SET(it->first, &rd);
                }
                nfds = max(nfds, _client.rbegin()->first);///////////set max fd
            }

            r = select(nfds + 1, &rd, &wr, &er, &tv);
            if (r == -1 && errno == EINTR)
                break;//continue;
            if (r == -1)
                prog_error("select");
            
            new_client();

                /****************[reading]****************/
            for (iterator it = _client.begin(); it != _client.end(); ++it)
            {
                int valread;
                string text;
                char buffer[BUF_SIZE];

                memset(buffer, 0, 1024);
                while ((valread = recv(it->first, buffer, BUF_SIZE, 0)))
                {
                    // if (valread < 0)
                        //////////handle here///////////
                    text.append(buffer);
                    memset(buffer, 0, sizeof(buffer));
                cout << text << endl;
                }

                
                send(it->first, HELLO_MSG, strlen(HELLO_MSG), 0);
                for (iterator its = _client.begin(); its != _client.end(); ++its)
                    send(its->first, text.c_str(), text.length(), 0);
                cout << "client_size=" << _client.size() << endl;
                text.clear();
            }
        }
    }

/****************[create new client]****************/
    void    new_client()
    {
        sockaddr_in client_address;
        socklen_t   client_addrlen = sizeof(client_address);

        memset(&address, 0, client_addrlen);
        int new_socket = accept(server_fd, (struct sockaddr*)&client_address, &client_addrlen);
        if (new_socket == -1)
            prog_error("accept");
        cout << "adding new client: " << new_socket << endl;
        _client.insert(make_pair(new_socket, Client(server_fd)));
    }

    Server();
    Server& operator=(const Server& other);
};