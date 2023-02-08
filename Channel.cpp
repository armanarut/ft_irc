#include "Channel.hpp"

    Channel::Channel(){}

    Channel::Channel(std::string& name)
    :name_(name) {}

    Channel::~Channel() {}

    void    Channel::add_user(Client* client)
    {
        users_.push_back(client);
        if (users_.size() == 1)
            admin_ = users_[0];
    }

    void    Channel::leave_chanel(Client* client)
    {
        for(std::vector<Client*>::iterator it = users_.begin(); it != users_.end(); ++it)
        {
            if (*it == client)
            {
                users_.erase(it);
                break ;
            }
        }
        if (users_.size() > 0 && admin_ != users_[0])
            admin_ = users_[0];
    }

	void	Channel::sendMsg(Client* client, const std::string& msg, const std::string& command)
	{
        for(std::vector<Client*>::iterator it = users_.begin(); it != users_.end(); ++it)
            if (*it != client)
                client->sendMsg((*it)->getFd(), msg, name_, command);
	}

    bool    Channel::Channel::isAvelabel(Client* client)
    {
        for(std::vector<Client*>::iterator it = users_.begin(); it != users_.end(); ++it)
            if (*it == client)
                return true;
        return false;
    }

    bool    Channel::isAdmin(Client* client)
    {
        if (!isAvelabel(client))
            return false;
        return admin_ == client;
    }

	bool Channel::search_user(Client* client)
	{        
		for(std::vector<Client*>::iterator it = users_.begin(); it != users_.end(); ++it)
			if (*it == client)
				return true;
		return true;
	}