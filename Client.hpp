#pragma once

#include <string>
#include <sys/socket.h>


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

	void	setNick(std::string nick)
	{
		nick_ = nick;
	}

	std::string getUser()
	{
		return (user_);
	}

	void	setUser(std::string user)
	{
		user_ = user;
	}

	int		getFd()
	{
		return (fd_);
	}

	void	setPasswd()
	{
		passwd = true;
	}

	bool	hasPasswd()
	{
		return (passwd);
	}

	void	sendMsg(int socket_fd, const std::string &msg, const std::string &channel = std::string())
	{
		if (channel.size())
			send(socket_fd, (channel + ": ").c_str(), channel.length() + 2, 0);
		send(socket_fd, (nick_ + ": ").c_str(), nick_.length() + 2, 0);
		send(socket_fd, msg.c_str(), msg.length(), 0);
		send(socket_fd,"\r\n\r\n", 4, 0);
	}

    std::string		buffer;

private:
	std::string		nick_;
	std::string		user_;
	const int		fd_;
	bool			passwd;
};
