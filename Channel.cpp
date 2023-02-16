#include "Channel.hpp"

Channel::Channel(){}

Channel::Channel(const std::string& name, const std::string& key)
:name_(name),
k(key) {}

Channel::~Channel() {}

void    Channel::join(Client* client)
{
    client->sending(RPL_JOIN(client->getPrefix(), name_));
    this->sending(client, "join to channel", "");
    users_.push_back(client);
    this->newAdmin();
}

void    Channel::leave_channel(Client* client)
{
    client->sending(RPL_PART(client->getPrefix(), name_));
    this->sending(client, "leave the channel", "");
    for(std::vector<Client*>::iterator it = users_.begin(); it != users_.end(); ++it)
    {
        if (*it == client)
        {
            users_.erase(it);
            break ;
        }
    }
    this->newAdmin();
}

void	Channel::sending(Client* client, const std::string& message, const std::string& command)
{
    for(std::vector<Client*>::iterator it = users_.begin(); it != users_.end(); ++it)
        if (*it != client)
            (*it)->sending(RPL_MSG(client->getPrefix(), command, name_, message));
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
    return false;
}

void    Channel::newAdmin()
{
    if (users_.size() > 0 && admin_ != users_[0])
    {
        admin_ = users_[0];
        admin_->sending(RPL_MSG(admin_->getPrefix(), "", name_, "you are the new admin"));
        this->sending(admin_, "is a new admin", "");
    }
}

std::string    Channel::getKey()
{
    return k;
}

void    Channel::setKey(const std::string& key)
{
    k = key;
}

void    Channel::whoReply(Client *client)
{
    for(std::vector<Client*>::iterator it = users_.begin(); it != users_.end(); ++it)
        client->sending(RPL_WHOREPLY( name_, (*it)->getUser(), (*it)->getHost(), (*it)->getNick(), (*it)->getReal()));
}