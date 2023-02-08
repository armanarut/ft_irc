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

	Client(int fd)
		:nickname(),
		username(),
		hostname(),
		realname(),
		registered(false),
		passwd(false),
		fd_(fd) {}

	~Client() {}

	void	init(const std::string& user, const std::string& host, const std::string& real)
	{
        std::cout << "operator_USER: username=" << user << " hostname=" << host << " realname=" << real << std::endl;
		username = user;
		hostname = host;
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
			registered = true;
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

	void	sendMsg(int socket_fd, const std::string &msg, const std::string &nick_or_channel, const std::string &command)
	{
		SEND_CLIENT(socket_fd, nickname, command, nick_or_channel.c_str(), msg.c_str());
		// send(socket_fd, (":" + nickname + " PRIVMSG ").c_str(), nickname.length() + 10, 0);
		// 	send(socket_fd, (nick_or_channel + " :").c_str(), nick_or_channel.length() + 2, 0);
		// send(socket_fd, msg.c_str(), msg.length(), 0);
		// send(socket_fd,"\r\n", 2, 0);
	}

	std::string	getPerfix()
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
		sending(":" + getPerfix() + " " + reply);
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
