#pragma once

class Client;

#include <string>
#include <cstring>
#include <map>
#include <iostream>
#include <sys/socket.h>

#include "Channel.hpp"
#include "ircserv.hpp"

class Client
{
	typedef std::map<std::string, Channel>::iterator iterator;
public:

	Client(int fd, std::string _hostname)
		:nickname(),
		username(),
		hostname(_hostname),
		realname(),
		registered(false),
		passwd(false),
		fd_(fd) {}

	~Client() {}

	void	init(const std::string& user, const std::string& real)
	{
		username = user;
		realname = real;
	}

	std::string getNick()
	{
		return (nickname);
	}

	void	setNick(std::string nick)
	{
		nickname = nick;
	}

	std::string getUser()
	{
		return (username);
	}

	void	setUser(std::string user)
	{
		username = user;
	}

	int		getFd()
	{
		return (fd_);
	}

	void	registering()
	{
		if (passwd && !username.empty() && !realname.empty() && !nickname.empty())
		{
			registered = true;
			reply(RPL_WELCOME(nickname));
		}
	}

	void	unlockPasswd()
	{
		passwd = true;
	}

	bool	isRegistered()
	{
		return (registered);
	}

	bool	hasPasswd()
	{
		return (passwd);
	}

	std::string	getPrefix()
	{
		return (nickname + (username.empty() ? "" : "!" + username) + (hostname.empty() ? "" : "@" + hostname));
	}

	void	sending(const std::string& massage)
	{
		std::string buffer = massage + "\r\n";

		if (send(fd_, buffer.c_str(), buffer.length(), 0) < 0)
			throw std::runtime_error("Error: can't send message to client.");
	}

	void reply(const std::string& reply)
	{
		sending(":" + getPrefix() + " " + reply);
	}

    std::string		buffer;

private:
	std::string		nickname;
	std::string		username;
	std::string		hostname;
	std::string		realname;
	bool			registered;
	bool			passwd;
	int				fd_;
};
