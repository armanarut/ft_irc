#include "Channel.hpp"

Channel::Channel(){}

Channel::Channel(const std::string& name, const std::string& key)
:name_(name),
k(key) {}

Channel::~Channel() {}

void    Channel::join(Client* client)
{
    users_.push_back(client);
    for(std::vector<Client*>::iterator it = users_.begin(); it != users_.end(); ++it)
        (*it)->sending(RPL_JOIN(client->getPrefix(), name_));
    this->newAdmin();
    this->replyName(client);
}

void    Channel::replyName(Client* client)
{
    for(std::vector<Client*>::iterator it = users_.begin(); it != users_.end(); ++it)
    {
        if (*it == admin_)
            client->sending(RPL_NAMREPLY(client->getNick(), name_, "@", (*it)->getNick()));
        else
            client->sending(RPL_NAMREPLY(client->getNick(), name_, "+", (*it)->getNick()));
    }
    client->sending(RPL_ENDOFNAMES(client->getNick(), name_));
}

void    Channel::part(Client* client)
{
    std::vector<Client*>::iterator del;
    for(std::vector<Client*>::iterator it = users_.begin(); it != users_.end(); ++it)
    {
        (*it)->sending(RPL_PART(client->getPrefix(), name_));
        if (*it == client)
            del = it;
    }
    users_.erase(del);
    this->newAdmin();
}

void    Channel::kick(Client* client, const std::string& reason)
{
    std::vector<Client*>::iterator del;
    for(std::vector<Client*>::iterator it = users_.begin(); it != users_.end(); ++it)
    {
        (*it)->sending(RPL_KICK(client->getPrefix(), name_, client->getNick(), reason));
        if (*it == client)
            del = it;
    }
    users_.erase(del);
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
        for(std::vector<Client*>::iterator it = users_.begin(); it != users_.end(); ++it)
            this->replyName(*it);
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
        client->sending(RPL_WHOREPLY(client->getNick(), name_, (*it)->getUser(), (*it)->getHost(), (*it)->getNick(), "H", (*it)->getReal()));
    client->sending(RPL_ENDOFWHO(client->getNick(), name_));
}