#pragma once

class Channel;

#include <vector>
#include "Client.hpp"

class Channel
{
public:

    Channel();
    Channel(const std::string& name, const std::string& key);
    ~Channel();

    void    join(Client* client);
    void    leave_channel(Client* client);
	void	sending(Client* client, const std::string& msg, const std::string& command);
    bool    isAvelabel(Client* client);
    bool    isAdmin(Client* client);
	bool    search_user(Client* client);
    std::string    getKey();
    void    setKey(const std::string& key);
    void    whoReply(Client *client);

private:
    std::string             name_;
    std::vector<Client*>    users_;
    Client*                 admin_;

    std::string             k;

    void    newAdmin();
};
