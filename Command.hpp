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

class CommandUSER: public Command
{
public:
    CommandUSER(Server* server, bool authRequired = true):Command(server, authRequired){}
    ~CommandUSER(){}

    void    execute(Client *client, std::vector<std::string> arguments);
};

class CommandPRIVMSG: public Command
{
public:
    CommandPRIVMSG(Server* server, bool authRequired = true):Command(server, authRequired){}
    ~CommandPRIVMSG(){}

    void    execute(Client *client, std::vector<std::string> arguments);
};

class CommandNOTICE: public Command
{
public:
    CommandNOTICE(Server* server, bool authRequired = true):Command(server, authRequired){}
    ~CommandNOTICE(){}

    void    execute(Client *client, std::vector<std::string> arguments);
};

class CommandCAP: public Command
{
public:
    CommandCAP(Server* server, bool authRequired = true):Command(server, authRequired){}
    ~CommandCAP(){}

    void    execute(Client *client, std::vector<std::string> arguments);
};

class CommandPING: public Command
{
public:
    CommandPING(Server* server, bool authRequired = true):Command(server, authRequired){}
    ~CommandPING(){}

    void    execute(Client *client, std::vector<std::string> arguments);
};

class CommandPONG: public Command
{
public:
    CommandPONG(Server* server, bool authRequired = true):Command(server, authRequired){}
    ~CommandPONG(){}

    void    execute(Client *client, std::vector<std::string> arguments);
};

class CommandJOIN: public Command
{
public:
    CommandJOIN(Server* server, bool authRequired = true):Command(server, authRequired){}
    ~CommandJOIN(){}

    void    execute(Client *client, std::vector<std::string> arguments);
};

class CommandPART: public Command
{
public:
    CommandPART(Server* server, bool authRequired = true):Command(server, authRequired){}
    ~CommandPART(){}

    void    execute(Client *client, std::vector<std::string> arguments);
};

class CommandKICK: public Command
{
public:
    CommandKICK(Server* server, bool authRequired = true):Command(server, authRequired){}
    ~CommandKICK(){}

    void    execute(Client *client, std::vector<std::string> arguments);
};

class CommandQUIT: public Command
{
public:
    CommandQUIT(Server* server, bool authRequired = true):Command(server, authRequired){}
    ~CommandQUIT(){}

    void    execute(Client *client, std::vector<std::string> arguments);
};

class CommandMODE: public Command
{
public:
    CommandMODE(Server* server, bool authRequired = true):Command(server, authRequired){}
    ~CommandMODE(){}

    void    execute(Client *client, std::vector<std::string> arguments);
};

class CommandWHO: public Command
{
public:
    CommandWHO(Server* server, bool authRequired = true):Command(server, authRequired){}
    ~CommandWHO(){}

    void    execute(Client *client, std::vector<std::string> arguments);
};