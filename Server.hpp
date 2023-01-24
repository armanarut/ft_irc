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

#include "ircserv.hpp"
#include "Channel.hpp"
#include "Client.hpp"


class Server
{
    typedef std::map<int, Client>::iterator  iterator;

public:
    Server(short port, const std::string& pass)
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
    std::string  _pass;

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
            if (r == -1 && errno == EINTR)
                break;//continue;
            else if (r == -1)
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
                    if (FD_ISSET(it->first, &rd))
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
        std::cout << "Online user: " << new_socket << std::endl;
        std::cout << "Users online: " << _client.size() << std::endl;
    }

/****************[Operators]****************/
    void    check_operators(iterator &it)
    {
        std::string line = it->second.buffer;
        std::string word;
        if (*line.rbegin() == '\n')
            line.pop_back();
        if (line.rfind(" ") == std::string::npos)
            word = line;
        else
            word = line.substr(0, line.find(" "));
    
        if (word.size())
        {
            if (word == "NICK")
                operator_NICK(it, line.substr(word.size() + 1, line.size()));
            else if (word == "PRIVMSG")
                operator_PRIVMSG(it, line.substr(word.size() + 1, line.size()));
            else if (word == "JOIN")
                operator_JOIN(it, line.substr(word.size() + 1, line.size()));
            else if (word == "PART")
                operator_PART(it, line.substr(word.size() + 1, line.size()));
            else
            {
                SEND_ERR(it->first, word, ERR_UNKNOWNCOMMAND);
            }
        }
    }

    void    operator_PART(iterator &it, const std::string& line)
    {
        std::string word;

        word = line.substr(0, line.find(" "));
        if (!word.size())
        {
            SEND_MSG(it->first, ERR_NEEDMOREPARAMS);
        }
        else if (_channel.find(word) == _channel.end())
        {
            SEND_ERR(it->first, word, ERR_NOSUCHCHANNEL);
        }
        else if (!_channel[word].isAvelabel(&it->second))
        {
            SEND_ERR(it->first, word, ERR_NOTONCHANNEL);
        }
        else
        {
            _channel[word].leave_chanel(&it->second);
            SEND_CHANEL(it->first, word, LEAVE_CHANNEL);
        }
    }

    void    operator_JOIN(iterator &it, const std::string& line)
    {
        std::string word;

        word = line.substr(0, line.find(" "));
        if (!word.size())
        {
            SEND_MSG(it->first, ERR_NEEDMOREPARAMS);
        }
        else if (word[0] != '#')
        {
            SEND_ERR(it->first, word, ERR_BADCHANMASK);
        }
        else
        {
            if (_channel.find(word) == _channel.end())
                _channel.insert(std::make_pair(word, Channel(word)));
            _channel[word].add_user(&it->second);
            SEND_CHANEL(it->first, word, JOIN_CHANNEL);
        }
    }

    void    operator_PRIVMSG(iterator &it, const std::string& line)
    {
        std::string word;
        std::string text;

        word = line.substr(0, line.find(" "));
        if (!word.size() || line.rfind(" ") == std::string::npos)
        {
            SEND_MSG(it->first, ERR_NOTEXTTOSEND);
        }
        else
        {
            text = line.substr(word.size() + 1, line.size());
            if (!text.size())
            {
                SEND_MSG(it->first, ERR_NOTEXTTOSEND);
            }
            else if (word[0] == '#')
            {
                if (_channel.find(word) == _channel.end())
                {
                    SEND_ERR(it->first, word, ERR_NOSUCHNICK);
                }
                else if (!_channel[word].isAvelabel(&it->second))
                {
                    SEND_ERR(it->first, word, ERR_CANNOTSENDTOCHAN);
                }
                else
                    _channel[word].sendMsg(&it->second, text);
            }
            else
            {
                if (_user.find(word) == _user.end())
                {
                    SEND_ERR(it->first, word, ERR_NOSUCHNICK);
                }
                else
                    it->second.sendMsg(_user[word], text);
            }
        }
    }

    void    operator_NICK(iterator &it, const std::string& line)
    {
        std::string word;

        word = line.substr(0, line.find(" "));

        if (!word.size())
        {
            SEND_MSG(it->first, ERR_NONICKNAMEGIVEN);
        }
        else if (!is_all_alpha(word))
        {
            SEND_ERR(it->first, word, ERR_ERRONEUSNICKNAME);
        }
        else if (_user.find(word) != _user.end() && _user[word] != it->first)
        {
            SEND_ERR(it->first, word, ERR_NICKNAMEINUSE);
        }
        else
        {
            _user.erase(word);
            std::cout << "setting NICK: " << word << std::endl;
            it->second.setNick(word);
            _user.insert(std::make_pair(word, it->first));
        }
    }

    Server();
    Server& operator=(const Server& other);
};