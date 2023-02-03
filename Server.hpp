#pragma once

#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <map>
#include <iostream>
// #include <cerrno>
// #include <cstring>
// #include <sstream>

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>

#include "Operators.hpp"
// #include "Channel.hpp"
// #include "Client.hpp"

class Server
{
public:

    typedef std::map<int, Client>::iterator  iterator;

    Server(short port, const std::string& pass)
        :_port(port),
        _pass(pass),
        _operators(_client, _user, _channel, _pass)
    {
        init_server();
        start();
    };

    ~Server()
    {
        for (iterator it = _client.begin(); it != _client.end(); ++it)
            close(it->first);
        close(server_fd);
    };
    
private:
    short   _port;
    std::string  _pass;

    int         server_fd;
    sockaddr_in address;

    std::map<int, Client>           _client;
    std::map<std::string, int>      _user;
	std::map<std::string, Channel>	_channel;
	Operators	                    _operators;

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

/****************[make select]****************/
    void    start()
    {
        int     r, nfds = server_fd;
        fd_set  rd, wr, er;
        timeval tv;

        tv.tv_sec = 0;
        tv.tv_usec = 300000;
        (FD_ZERO(&rd), FD_ZERO(&wr), FD_ZERO(&er));
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
                    if (FD_ISSET(it->first, &wr))
                    {
                        // cout << "writing..." << endl;
                        FD_CLR(it->first, &wr);
                        check_operators(it);
                        it->second.buffer.clear();
                    }
                    /*************[reading]*************/
                    else if (FD_ISSET(it->first, &rd))
                    {
                        // cout << "reading client: " << it->first << endl;
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

    bool    get_buffer(iterator& it)
    {
        char    buffer[BUF_SIZE];
        int     valread;

        while ((memset(buffer, 0, 1024), \
                valread = recv(it->first, buffer, BUF_SIZE, 0)) != -1)
        {
            if (valread == 0)
            {
                close(it->first);
                _user.erase(it->second.getNick());
                _client.erase(it->first);
                std::cout << "Offline user: " << it->first << std::endl;
                std::cout << "Users online: " << _client.size() << std::endl;
                return valread;
            }
            it->second.buffer.append(buffer);
        }
        return valread;
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
        _client.insert(std::make_pair(new_socket, Client(new_socket)));
        std::cout << "New user: " << new_socket << std::endl;
        std::cout << "Users online: " << _client.size() << std::endl;
    }

/****************[Operators]****************/
    void    check_operators(iterator &it)
    {
        std::string line;
        if (it->second.buffer.find("\r\n") != std::string::npos)
        {
            line = it->second.buffer.substr(0, it->second.buffer.find("\r\n"));
            it->second.buffer.erase(0, line.size() + 2);
        }
        else
        {
            line = it->second.buffer.substr(0, it->second.buffer.find("\n"));
            it->second.buffer.erase(0, line.size() + 1);
        }
        std::cout << "Incoming command: " << line << std::endl;

        std::string command;
        if (line.rfind(" ") == std::string::npos)
            command = line;
        else
            command = line.substr(0, line.find(" "));

        if (line.size() == command.size())
            line.clear();
        else
            line.substr(command.size() + 1, line.size());

        _operators(command, it, line);

        // if (!word.empty())
        // {
        //     if (word == "CAP")
        //         ;// operator_CAP(it, line.substr(word.size() + 1, line.size()));
        //     else if (word == "PING")
        //         ;// operator_PING(it, line.substr(word.size() + 1, line.size()));
        //     else if (word == "PONG")
        //         ;// operator_PONG(it, line.substr(word.size() + 1, line.size()));
        //     else if (word == "PASS")
        //     {
        //         if (CHECK_PARAM(line, word))
        //         {
        //             SEND_MSG(it->first, ERR_NEEDMOREPARAMS);
        //         }
        //         else
        //             operator_PASS(it, line.substr(word.size() + 1, line.size()));
        //     }
        //     else if (word == "USER")
        //     {
        //         if (CHECK_PARAM(line, word))
        //         {
        //             SEND_MSG(it->first, ERR_NEEDMOREPARAMS);
        //         }
        //         else
        //         operator_USER(it, line.substr(word.size() + 1, line.size()));
        //     }
        //     else if (word == "NICK")
        //     {
        //         if (CHECK_PARAM(line, word))
        //         {
        //             SEND_MSG(it->first, ERR_NEEDMOREPARAMS);
        //         }
        //         else
        //         operator_NICK(it, line.substr(word.size() + 1, line.size()));
        //     }
        //     else if (!it->second.isRegistered())
        //     {
        //         SEND_MSG(it->first, NOT_REGISTERED);
        //     }
        //     else if (word == "NOTICE")
        //     {
        //         if (CHECK_PARAM(line, word))
        //         {
        //             SEND_MSG(it->first, ERR_NEEDMOREPARAMS);
        //         }
        //         else
        //         operator_PRIVMSG(it, line.substr(word.size() + 1, line.size()), "NOTICE");
        //     }
        //     else if (word == "PRIVMSG")
        //     {
        //         if (CHECK_PARAM(line, word))
        //         {
        //             SEND_MSG(it->first, ERR_NEEDMOREPARAMS);
        //         }
        //         else
        //         operator_PRIVMSG(it, line.substr(word.size() + 1, line.size()), "PRIVMSG");
        //     }
        //     else if (word == "JOIN")
        //     {
        //         if (CHECK_PARAM(line, word))
        //         {
        //             SEND_MSG(it->first, ERR_NEEDMOREPARAMS);
        //         }
        //         else
        //         operator_JOIN(it, line.substr(word.size() + 1, line.size()));
        //     }
        //     else if (word == "PART")
        //     {
        //         if (CHECK_PARAM(line, word))
        //         {
        //             SEND_MSG(it->first, ERR_NEEDMOREPARAMS);
        //         }
        //         else
        //         operator_PART(it, line.substr(word.size() + 1, line.size()));
        //     }
		// 	else if (word == "KICK")
        //     {
        //         if (CHECK_PARAM(line, word))
        //         {
        //             SEND_MSG(it->first, ERR_NEEDMOREPARAMS);
        //         }
        //         else
		// 		operator_KICK(it, line.substr(word.size() + 1, line.size()));
        //     }
        //     else if (word == "LUSERS")
        //         operator_LUSERS(it);
        //     else
        //     {
        //         std::cout  << line << ERR_UNKNOWNCOMMAND << std::endl;
        //         SEND_ERR(it->first, word, ERR_UNKNOWNCOMMAND);
        //     }
        // }
        if (it->second.buffer.size())
            check_operators(it);
    }


    Server();
    Server& operator=(const Server& other);
};
