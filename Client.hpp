#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sys/socket.h>

#include "Channel.hpp"

class Channel;

class Client
{
public:
	Client(const int &fd) : nick_(), user_(), fd_(fd), passwd(false) {}

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

	void joinChannel(std::map<std::string, Channel>::pointer channel_ptr)
	{
		channels.push_back(channel_ptr);
	}

	void leaveChannel(std::map<std::string, Channel>::pointer channel_ptr)
	{
		std::vector<std::map<std::string, Channel>::pointer>::iterator iter;

		iter = channels.begin();
		while(iter != channels.end())
		{
			if(*iter == channel_ptr)
			{
				channels.erase(iter, ++iter);
				return;
			}
			++iter;
		}
	}

	void sendMsg(const std::string &msg)
	{
		std::cout << msg;
		send(fd_, msg.c_str(), msg.length(), 0);
	}

	void setPasswd()
	{
		passwd = true;
	}

	bool hasPasswd()
	{
		return (passwd);
	}

private:
	std::string												nick_;
	std::string												user_;
	std::vector<std::map<std::string, Channel>::pointer>	channels;
	const int												&fd_;
	bool													passwd;

};
