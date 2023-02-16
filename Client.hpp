#pragma once

class Client;

#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include <sys/socket.h>

#include "Channel.hpp"
#include "ircserv.hpp"

class Client
{
public:

	Client(int fd, std::string _hostname);

	~Client();

	void	init(const std::string& user, const std::string& real);
	std::string getNick();
	std::string getUser();
	std::string getHost();
	std::string getReal();
	void	setNick(std::string nick);
	void	setUser(std::string user);
	int		getFd();
	void	registering();
	void	unlockPasswd();
	bool	isRegistered();
	bool	hasPasswd();
	std::string	getPrefix();
	void	sending(const std::string& massage);
	void reply(const std::string& reply);
	void	joinChannel(Channel* channel);
	void	leaveChannel(Channel* channel);
	void	leaveChannel(int del_all);

    std::string		buffer;
	bool			quit;
private:
	std::string		nickname;
	std::string		username;
	std::string		hostname;
	std::string		realname;
	bool			registered;
	bool			passwd;
	int				fd_;
	std::vector<Channel*>	_channel;
	
};
