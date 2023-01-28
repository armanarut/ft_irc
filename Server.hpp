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
            line = it->second.buffer.substr(0, it->second.buffer.find("\r\n"));
        else
            line = it->second.buffer.substr(0, it->second.buffer.find("\n"));
        it->second.buffer.erase(0, line.size() + 2);
        std::cout << "Incoming command: " << line << std::endl;
        std::string word;
        if (line.rfind(" ") == std::string::npos)
            word = line;
        else
            word = line.substr(0, line.find(" "));
    
        if (word.size())
        {
            if (word == "USER")
                operator_USER(it, line.substr(word.size() + 1, line.size()));
            else if (word == "NICK")
                operator_NICK(it, line.substr(word.size() + 1, line.size()));
            else if (word == "PRIVMSG")
                operator_PRIVMSG(it, line.substr(word.size() + 1, line.size()));
            else if (word == "JOIN")
                operator_JOIN(it, line.substr(word.size() + 1, line.size()));
            else if (word == "PART")
                operator_PART(it, line.substr(word.size() + 1, line.size()));
			else if (word == "KICK")
				operator_KICK(it, line.substr(word.size() + 1, line.size()));
            else if (word == "LUSERS")
                operator_LUSERS(it);
            else
            {
                std::cout  << line << ERR_UNKNOWNCOMMAND << std::endl;
                SEND_ERR(it->first, word, ERR_UNKNOWNCOMMAND);
            }
        }
        if (it->second.buffer.size())
            check_operators(it);
    }

	void	operator_KICK(iterator &it, const std::string& line)
	{
		std::string channel_name, user_name, msg_text;

        int pos = 0;

        channel_name = line.substr(pos, line.find(" ", pos) - pos);
        pos += channel_name.size() + 1;
        user_name = line.substr(pos, line.find(" ", pos) - pos);
        pos += user_name.size(); // + 1;
        msg_text = line.substr(pos, line.size());

		if (!channel_name.size() || !user_name.size())
		{
			SEND_MSG(it->first, ERR_NEEDMOREPARAMS);
		}
		else if (_channel.find(channel_name) == _channel.end())
		{
			SEND_ERR(it->first, channel_name, ERR_NOSUCHCHANNEL);
		}
		else
		{
			if (!(_channel[channel_name].isAdmin(&it->second)))
			{
				SEND_MSG(it->first, "DU ADMIN CHES");
				return;
			}
			Client*  search_out = _channel[channel_name].search_user_of_channel(user_name);
			if (search_out)
			{
//				SEND_MSG(search_out, "mesig text");
				_channel[channel_name].leave_chanel(search_out);
			}
		}
	}

    void    operator_LUSERS(iterator &it)
    {
        std::string msg = (it->second.getNick() + " " + std::to_string(_user.size()));
        SEND_ERR(it->first, msg, RPL_LUSEROP);
    }

    void    operator_USER(iterator &it, const std::string& line)
    {
        int pos = 0;
        std::string word;
        std::string user;
        std::string host;

        user = line.substr(pos, line.find(" ", pos) - pos);
        pos += user.size() + 1;
        word = line.substr(pos, line.find(" ", pos) - pos);
        pos += word.size() + 1;
        word = line.substr(pos, line.find(" ", pos) - pos);
        if (word[0] != ':')
        {
            pos += word.size() + 1;
            host = word;
        }
        it->second.init(user, host, line.substr(pos + 1, line.size()));
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
                    it->second.sendMsg(_user[word], text, word);
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
