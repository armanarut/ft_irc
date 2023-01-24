#pragma once

#include <iostream>
#include <string>
#include <cerrno>
#include <cstring>
#include <sstream>
#include <unistd.h>
#include <fcntl.h>
#include <map>
#include "Client.hpp"
#include "Channel.hpp"

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>

#define HELLO_MSG "Hello from server\r\n\r\n"

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
    short   _port;
    string  _pass;

    int         server_fd;
    sockaddr_in address;

    std::map<int, Client>           _client;
    std::map<std::string, int>      _user;
	std::map<std::string, Channel>	_channel;

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

        fcntl(server_fd, F_SETFL, O_NONBLOCK);
    };

/****************[Operators]****************/
    void    check_operators(iterator &it)
    {
        std::string line = it->second.buffer;
        if (*line.rbegin() == '\n')
            line.pop_back();
        std::string word = line.substr(0, line.find(" "));
        int pos = word.size() + 1;
    
        if (pos)
        {
            if (word == "NICK")
            {
                word = line.substr(pos, line.find(" ", pos) - pos);
                if (word.size())
                {
                    cout << "setting NICK: " << word << endl;
                    it->second.setNick(word);
                    _user.insert(std::make_pair(word, it->first));
                }
            }
            else if (word == "PRIVMSG")
            {
                word = line.substr(pos, line.find(" ", pos) - pos);
                pos += word.size() + 1;
                if (_user.find(word) != _user.end())
                    it->second.sendMsg(_user[word], line.substr(pos, line.size()));
                else
                    cout << "can't find: " << word << endl;
            }
            else
                send(it->first, "command not found:\n\r\n\r", strlen("command not found:\n\r\n\r"), 0);
        }
    }

/****************[make select]****************/
    void    start()
    {
        char buffer[BUF_SIZE];
        int r, nfds = server_fd;
        fd_set rd, wr, er;
        FD_ZERO(&rd);
        FD_ZERO(&wr);
        FD_ZERO(&er);
        timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 300000;

        for(;;)
        {
            if (_client.size())
            {
                for (iterator it = _client.begin(); it != _client.end(); ++it)
                    FD_SET(it->first, &rd);
                nfds = max(nfds, _client.rbegin()->first);///////////set max fd
            }

            r = select(nfds + 1, &rd, &wr, &er, &tv);
            if (r == -1 && errno == EINTR)
                break;//continue;
            else if (r == -1)
                prog_error("select");
            else if (r)
            {
                // cout << "continue..." << endl;
                for (iterator it = _client.begin(); it != _client.end(); ++it)
                {
                    int valread = 0;
                    /****************[writing]****************/
                    if (FD_ISSET(it->first, &wr))
                    {
                        // cout << "writing..." << endl;
                        check_operators(it);
                        it->second.buffer.clear();
                        FD_CLR(it->first, &wr);
                    }
                    /****************[reading]****************/
                    if (FD_ISSET(it->first, &rd))
                    {
                        // cout << "reading client: " << it->first << endl;
                        FD_CLR(it->first, &rd);
                        memset(buffer, 0, 1024);
                        while ((valread = recv(it->first, buffer, BUF_SIZE, 0)) != -1)
                        {
                            if (valread == 0)
                            {
                                cout << "Offline client: " << it->first << endl;
                                close(it->first);
                                _client.erase(it->first);
                                break ;
                            }
                            it->second.buffer.append(buffer);
                        }
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

/****************[create new client]****************/
    void    new_client()
    {
        sockaddr_in client_address;
        socklen_t   client_addrlen = sizeof(client_address);

        memset(&address, 0, client_addrlen);
        int new_socket = accept(server_fd, (struct sockaddr*)&client_address, &client_addrlen);
        if (new_socket == -1)
            return ;
        fcntl(new_socket, F_SETFL, O_NONBLOCK);
        cout << "Online client: " << new_socket << endl;
        _client.insert(make_pair(new_socket, Client(new_socket)));
    }

    Server();
    Server& operator=(const Server& other);
};