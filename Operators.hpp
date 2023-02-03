#pragma once

#include <map>
#include <string>
#include <sys/socket.h>
#include "Channel.hpp"

// class Client;
// class Channel;

class Operators
{
    typedef std::map<int, Client>::iterator  iterator;

public:

    Operators(std::map<int, Client>& client, std::map<std::string, int>& user, std::map<std::string, Channel>& channel, std::string& pass)
        :_client(client),
        _user(user),
        _channel(channel),
        _pass(pass)
        {
            init_operators();
        };

    void operator()(const std::string& command, iterator& it, const std::string& line)
    {
        if (command == "PASS")
            operator_PASS(it, line);
    }

    void    init_operators()
    {
        // this->add_operator("PASS", &this->operator_PASS);
        // this->add_operator("KICK", this->operator_KICK);
        // this->add_operator("LUSERS", this->operator_LUSERS);
        // this->add_operator("CAP", this->operator_CAP);
        // this->add_operator("PING", this->operator_PING);
        // this->add_operator("PONG", this->operator_PONG);
        // this->add_operator("USER", this->operator_USER);
        // this->add_operator("PART", this->operator_PART);
        // this->add_operator("JOIN", this->operator_JOIN);
        // this->add_operator("PRIVMSG", this->operator_PRIVMSG);
        // this->add_operator("NOTICE", this->operator_NOTICE);
        // this->add_operator("NICK", this->operator_NICK);
    }

    void    add_operator(std::string command,   void (*f) (iterator&, const std::string&))
    {
        _operator.insert(std::make_pair(command, f));
    }

    void	operator_PASS(iterator &it, const std::string& pass);
	void	operator_KICK(iterator &it, const std::string& line);
    void    operator_LUSERS(iterator &it, const std::string& line);
    void    operator_CAP(iterator &it, const std::string& line);
    void    operator_PING(iterator &it, const std::string& line);
    void    operator_PONG(iterator &it, const std::string& line);
    void    operator_USER(iterator &it, const std::string& line);
    void    operator_PART(iterator &it, const std::string& line);
    void    operator_JOIN(iterator &it, const std::string& line);
    void    operator_PRIVMSG(iterator &it, const std::string& line);
    void    operator_NOTICE(iterator &it, const std::string& line);
    void    operator_NICK(iterator &it, const std::string& line);

private:

    std::map<std::string, void(*)(iterator&, const std::string&)>    _operator;
    std::map<int, Client>&           _client;
    std::map<std::string, int>&      _user;
	std::map<std::string, Channel>&	_channel;
    std::string&  _pass;

};