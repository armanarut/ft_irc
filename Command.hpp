#pragma once

class Command;

#include <string>

#include "Client.hpp"
#include "Server.hpp"

class Command
{
protected:
	Server* _server;
	bool _authRequired;

public:
    Command(Server* server, bool authRequired = true)
    :_server(server),
    _authRequired(authRequired) {};

    virtual ~Command(){};

    bool authRequired() const { return _authRequired; };

	virtual void execute(Client *client, std::vector<std::string> arguments) = 0;
};

class CommandPASS: public Command
{
public:
    CommandPASS(Server* server, bool authRequired = true):Command(server, authRequired){}
    ~CommandPASS(){}

    void    execute(Client *client, std::vector<std::string> arguments);
};

class CommandNICK: public Command
{
public:
    CommandNICK(Server* server, bool authRequired = true):Command(server, authRequired){}
    ~CommandNICK(){}

    void    execute(Client *client, std::vector<std::string> arguments);
};