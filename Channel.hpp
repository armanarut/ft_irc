#pragma once

#include <vector>
#include "Client.hpp"

class Channel
{
public:

    Channel(){};

    Channel(std::string& name)
    :name_(name) {}

    ~Channel() {};

    void    add_user(Client* client)
    {
        users_.push_back(client);
        if (users_.size() == 1)
            admin_ = users_[0];
    };

    void    leave_chanel(Client* client)
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

	void	sendMsg(Client* client, const std::string& msg)
	{
        for(std::vector<Client*>::iterator it = users_.begin(); it != users_.end(); ++it)
            if (*it != client)
                client->sendMsg((*it)->getFd(), msg, name_);
	}

    bool    isAvelabel(Client* client)
    {
        for(std::vector<Client*>::iterator it = users_.begin(); it != users_.end(); ++it)
            if (*it == client)
                return true;
        return false;
    }

    bool    isAdmin(Client* client)
    {
        if (!isAvelabel(client))
            return false;
        return admin_ == client;
    }

	Client* search_user_of_channel(std::string username)
	{        
		for(std::vector<Client*>::iterator it = users_.begin(); it != users_.end(); ++it)
			if ((*it)->getUser() == username)
				return (*it);
		return (NULL);
	}

private:
    std::string             name_;
    std::vector<Client*>    users_;
    Client*                 admin_;

};
