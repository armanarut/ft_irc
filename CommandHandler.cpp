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
        std::cout << "Incoming command: " << line << std::endl;

        std::string name;
        // if (line.rfind(" ") == std::string::npos)
        //     command = line;
        // else
            name = line.substr(0, line.find(" "));

        // if (line.size() == name.size())
        //     line.clear();
        // else

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
        
        // _operators(name, it, line);

        // if (!word.empty())
        // {
        //     if (word == "CAP")
        //         ;// operator_CAP(it, line.substr(word.size() + 1, line.size()));
        //     else if (word == "PING")
        //         ;// operator_PING(it, line.substr(word.size() + 1, line.size()));
        //     else if (word == "PONG")
        //         ;// operator_PONG(it, line.substr(word.size() + 1, line.size()));
        //     else if (word == "PASS")
        //     {
        //         if (CHECK_PARAM(line, word))
        //         {
        //             SEND_MSG(it->first, ERR_NEEDMOREPARAMS);
        //         }
        //         else
        //             operator_PASS(it, line.substr(word.size() + 1, line.size()));
        //     }
        //     else if (word == "USER")
        //     {
        //         if (CHECK_PARAM(line, word))
        //         {
        //             SEND_MSG(it->first, ERR_NEEDMOREPARAMS);
        //         }
        //         else
        //         operator_USER(it, line.substr(word.size() + 1, line.size()));
        //     }
        //     else if (word == "NICK")
        //     {
        //         if (CHECK_PARAM(line, word))
        //         {
        //             SEND_MSG(it->first, ERR_NEEDMOREPARAMS);
        //         }
        //         else
        //         operator_NICK(it, line.substr(word.size() + 1, line.size()));
        //     }
        //     else if (!it->second.isRegistered())
        //     {
        //         SEND_MSG(it->first, NOT_REGISTERED);
        //     }
        //     else if (word == "NOTICE")
        //     {
        //         if (CHECK_PARAM(line, word))
        //         {
        //             SEND_MSG(it->first, ERR_NEEDMOREPARAMS);
        //         }
        //         else
        //         operator_PRIVMSG(it, line.substr(word.size() + 1, line.size()), "NOTICE");
        //     }
        //     else if (word == "PRIVMSG")
        //     {
        //         if (CHECK_PARAM(line, word))
        //         {
        //             SEND_MSG(it->first, ERR_NEEDMOREPARAMS);
        //         }
        //         else
        //         operator_PRIVMSG(it, line.substr(word.size() + 1, line.size()), "PRIVMSG");
        //     }
        //     else if (word == "JOIN")
        //     {
        //         if (CHECK_PARAM(line, word))
        //         {
        //             SEND_MSG(it->first, ERR_NEEDMOREPARAMS);
        //         }
        //         else
        //         operator_JOIN(it, line.substr(word.size() + 1, line.size()));
        //     }
        //     else if (word == "PART")
        //     {
        //         if (CHECK_PARAM(line, word))
        //         {
        //             SEND_MSG(it->first, ERR_NEEDMOREPARAMS);
        //         }
        //         else
        //         operator_PART(it, line.substr(word.size() + 1, line.size()));
        //     }
		// 	else if (word == "KICK")
        //     {
        //         if (CHECK_PARAM(line, word))
        //         {
        //             SEND_MSG(it->first, ERR_NEEDMOREPARAMS);
        //         }
        //         else
		// 		operator_KICK(it, line.substr(word.size() + 1, line.size()));
        //     }
        //     else if (word == "LUSERS")
        //         operator_LUSERS(it);
        //     else
        //     {
        //         std::cout  << line << ERR_UNKNOWNCOMMAND << std::endl;
        //         SEND_ERR(it->first, word, ERR_UNKNOWNCOMMAND);
        //     }
        // }

        if (client->buffer.size())
            invoke(client);
        // std::cout << "check user deleted.\n"; // check
    }