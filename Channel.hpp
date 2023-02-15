#pragma once

class Channel;

#include <vector>
#include "Client.hpp"

class Channel
{
public:

    Channel();
    Channel(const std::string& name);
    ~Channel();

    void    add_user(Client* client);
    void    leave_chanel(Client* client);
	void	sending(Client* client, const std::string& msg, const std::string& command);
    bool    isAvelabel(Client* client);
    bool    isAdmin(Client* client);
	bool    search_user(Client* client);
    void    next_client_set_admin();

private:
    std::string             name_;
    std::vector<Client*>    users_;
    Client*                 admin_;

};
