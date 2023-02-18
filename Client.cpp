#include "Client.hpp"

	Client::Client(int fd, std::string _hostname)
		:nickname(),
		username(),
		hostname(_hostname),
		realname(),
		registered(false),
		passwd(false),
		fd_(fd) {}

	Client::~Client() {}

	void	Client::init(const std::string& user, const std::string& real)
	{
		username = user;
		realname = real;
	}

	std::string Client::getNick()
	{
		return (nickname);
	}

	std::string Client::getUser()
	{
		return (username);
	}

	std::string Client::getHost()
	{
		return (hostname);
	}

	std::string Client::getReal()
	{
		return (realname);
	}

	void	Client::setNick(std::string nick)
	{
		nickname = nick;
	}

	void	Client::setUser(std::string user)
	{
		username = user;
	}

	int		Client::getFd()
	{
		return (fd_);
	}

	void	Client::registering()
	{
		if (passwd && !username.empty() && !realname.empty() && !nickname.empty())
		{
			registered = true;
			reply(RPL_WELCOME(nickname));
		}
	}

	void	Client::unlockPasswd()
	{
		passwd = true;
	}

	bool	Client::isRegistered()
	{
		return (registered);
	}

	bool	Client::hasPasswd()
	{
		return (passwd);
	}

	std::string	Client::getPrefix()
	{
		return (nickname + (username.empty() ? "" : "!" + username) + (hostname.empty() ? "" : "@" + hostname));
	}

	void	Client::sending(const std::string& massage)
	{
		std::string buffer = massage + "\r\n";

		if (send(fd_, buffer.c_str(), buffer.length(), 0) < 0)
			std::cout << "Error: can't send message to client." << std::endl;
	}

	void Client::reply(const std::string& reply)
	{
		sending(":" + getPrefix() + " " + reply);
	}

	void	Client::joinChannel(Channel* channel)
	{
		_channel.push_back(channel);
		channel->join(this);
	}

	void	Client::leaveChannel(Channel* channel)
	{
		for (std::vector<Channel*>::iterator it = _channel.begin(); it != _channel.end(); ++it)
		{
			if (*it == channel)
			{
				(*it)->leave_channel(this);
				_channel.erase(it);
				break;
			}
		}
	}

	void	Client::leaveChannel(int del_all)
	{
        (void)del_all;
		for (std::vector<Channel*>::iterator it = _channel.begin(); it != _channel.end(); ++it)
			(*it)->leave_channel(this);
		_channel.clear();
	}