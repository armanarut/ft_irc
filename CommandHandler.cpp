#include "CommandHandler.hpp"

    CommandHandler::CommandHandler(Server* server):_server(server)
    {
    _commands["PASS"] = new CommandPASS(_server, false);
    _commands["NICK"] = new CommandNICK(_server, false);
    _commands["USER"] = new CommandUSER(_server, false);
	_commands["QUIT"] = new CommandQUIT(_server, false);

    _commands["CAP"] = new CommandCAP(_server);
    _commands["PING"] = new CommandPING(_server);
    _commands["PONG"] = new CommandPONG(_server);
    _commands["JOIN"] = new CommandJOIN(_server);
    _commands["PART"] = new CommandPART(_server);
    _commands["KICK"] = new CommandKICK(_server);
    _commands["MODE"] = new CommandMODE(_server);
    _commands["WHO"] = new CommandWHO(_server);
    _commands["INVITE"] = new CommandINVITE(_server);

    _commands["PRIVMSG"] = new CommandPRIVMSG(_server);
    _commands["NOTICE"] = new CommandNOTICE(_server);
    }

    CommandHandler::~CommandHandler(){}

    void    CommandHandler::invoke(Client* client)
    {
        std::string line;
        if (client->buffer.find("\r\n") != std::string::npos)
        {
            line = client->buffer.substr(0, client->buffer.find("\r\n"));
            client->buffer.erase(0, line.size() + 2);
        }
        else
        {
            line = client->buffer.substr(0, client->buffer.find("\n"));
            client->buffer.erase(0, line.size() + 1);
        }
        std::string name = line.substr(0, line.find(" "));

		try
		{
			Command *command = _commands.at(name);

            std::vector<std::string> arguments;

            std::string buf;
            std::stringstream ss(line.substr(name.length(), line.length()));
                line.substr(name.size(), line.size());

            while (ss >> buf)
                arguments.push_back(buf);
            if (!client->isRegistered() && command->authRequired())
            {
				client->reply(ERR_NOTREGISTERED(client->getNick()));
				return;
            }
            command->execute(client, arguments);
		}
		catch (const std::out_of_range &e)
		{
			client->reply(ERR_UNKNOWNCOMMAND(client->getNick(), name));
		}

        if (client->buffer.size())
            invoke(client);
    }