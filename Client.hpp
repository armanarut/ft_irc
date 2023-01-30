#pragma once

#include <string>
#include <sys/socket.h>


#define BUF_SIZE 1024

class Channel;

class Client
{
	typedef std::map<std::string, Channel>::iterator iterator;
public:

	Client(int fd)
		:nickname(),
		username(),
		hostname(),
		realname(),
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

	void	setPasswd()
	{
		passwd = true;
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

    std::string		buffer;

private:
	std::string		nickname;
	std::string		username;
	std::string		hostname;
	std::string		realname;
	bool			passwd;
	int				fd_;
};
