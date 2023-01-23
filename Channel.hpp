#pragma once

#include <map>
#include "Client.hpp"

class Client;

class Channel
{
public:
    Channel(std::string& name);
    void    add_to_chanel(Client* client)
    {
        (void)client;




        if (users_.size() == 1)
            admin_ = users_[0];//set admin
    };

    void    leave_chanel(Client* client)
    {
        (void)client;





        if (users_.size() > 0)
            admin_ = users_[0];
    }

private:
    std::vector<Client*>    users_;
    Client*                 admin_;

};
