#pragma once

class CommandHandler;

#include <map>
#include <vector>
#include <sstream>

#include "Command.hpp"

class CommandHandler
{
private:
    Server* _server;
    std::map<std::string, Command*>    _commands;

public:
    CommandHandler(Server* server);
    ~CommandHandler();

    void    invoke(Client* client);
};