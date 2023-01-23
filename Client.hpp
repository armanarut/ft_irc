#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sys/socket.h>

#include "Channel.hpp"

#define BUF_SIZE 1024

class Channel;

class Client
{
	typedef std::map<std::string, Channel>::iterator iterator;
public:
	Client(const int fd) : nick_(), user_(), fd_(fd), passwd(false)
	{
		(void)fd_;
	}

	~Client() {}

	std::string getNick()
	{
		return (nick_);
	}

	void setNick(std::string nick)
	{
		nick_ = nick;
	}

	std::string getUser()
	{
		return (user_);
	}

	void setUser(std::string user)
	{
		user_ = user;
	}

	// void joinChannel(std::map<std::string, Channel>::pointer channel_ptr)
	// {
	// 	channels.push_back(channel_ptr);
	// }

	// void joinChannel(std::string& channel_name)
	// {
	// 	iterator it = channels.find(channel_name);

	// 	if (it == channels.end())
	// 		it = channels.insert(std::make_pair(channel_name, Channel(channel_name))).first;
	// 	it->second.add_to_chanel(this);
	// }

	// void leaveChannel(std::map<std::string, Channel>::pointer channel_ptr)
	// {
		// std::vector<std::map<std::string, Channel>::pointer>::iterator iter;

		// iter = channels.begin();
		// while(iter != channels.end())
		// {
		// 	if(*iter == channel_ptr)
		// 	{
		// 		channels.erase(iter, ++iter);
		// 		return;
		// 	}
		// 	++iter;
		// }
	// }


	void	sendMsg(int socket_fd, const std::string &msg)
	{
		send(socket_fd, (nick_ + ": ").c_str(), nick_.length() + 2, 0);
		send(socket_fd, msg.c_str(), msg.length(), 0);
		send(socket_fd,"\n\r\n\r", 4, 0);
	}

	void setPasswd()
	{
		passwd = true;
	}

	bool hasPasswd()
	{
		return (passwd);
	}

    std::string buffer;

private:
	std::string												nick_;
	std::string												user_;
	const int												fd_;
	bool													passwd;
/*
NICK <имя пользователя>
USER <имя пользователя> < > < > <реальное имя>
*/
};
