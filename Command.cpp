#include "Command.hpp"

void    CommandPASS::execute(Client *client, std::vector<std::string> arguments)
{
    if (client->isRegistered())
    {
        client->reply(ERR_ALREADYREGISTERED(client->getNick()));
        return ;
    }
    if (arguments.empty())
    {
        client->reply(ERR_NEEDMOREPARAMS(client->getNick(), "PASS"));
        return ;
    }
    if (_server->getPass() != arguments[0].substr(arguments[0][0] == ':' ? 1 : 0))
    {
        client->reply(ERR_PASSWDMISMATCH(client->getNick()));
        return ;
    }
    client->unlockPasswd();
    client->registering();
}

void    CommandNICK::execute(Client *client, std::vector<std::string> arguments)
{
    if (arguments.empty())
    {
        client->reply(ERR_NONICKNAMEGIVEN(client->getNick()));
        return ;
    }
    std::string nickname = arguments[0];
    if (!is_all_alpha(nickname))
    {
        client->reply(ERR_ERRONEUSNICKNAME(client->getNick(), nickname));
        return ;
    }
    Client* client_avel = _server->getClient(nickname);
    if (client_avel != nullptr &&  client_avel != client)
    {
        client->reply(ERR_NICKNAMEINUSE(client->getNick(), nickname));
        return ;
    }
    _server->setUser(nickname, client->getFd());
    client->setNick(nickname);
    if (!client->isRegistered())
        client->registering();
}

// void	Operators::operator_KICK(iterator &it, const std::string& line)
// {
//     std::string channel_name, user_name, msg_text;

//     int pos = 0;

//     channel_name = line.substr(pos, line.find(" ", pos) - pos);
//     pos += channel_name.size() + 1;
//     user_name = line.substr(pos, line.find(" ", pos) - pos);
//     pos += user_name.size(); // + 1;
//     msg_text = line.substr(pos, line.size());

//     if (!channel_name.size() || !user_name.size())
//     {
//         SEND_MSG(it->first, ERR_NEEDMOREPARAMS);
//     }
//     else if (_channel.find(channel_name) == _channel.end())
//     {
//         SEND_ERR(it->first, channel_name, ERR_NOSUCHCHANNEL);
//     }
//     else
//     {
//         if (!(_channel[channel_name].isAdmin(&it->second)))
//         {
//             SEND_MSG(it->first, ERR_CHANOPRIVSNEEDED);
//             return;
//         }
//         else if (_user.find(user_name) == _user.end())
//         {
//             SEND_MSG(it->first, ERR_NOSUCHNICK);
//             return;
//         }
//         else if (!_channel[channel_name].search_user(&_client.at(_user[user_name])))
//         {
//             SEND_MSG(it->first, ERR_USERNOTINCHANNEL);
//         }
//         else
//         {
//             if (!msg_text.empty())
//             {
//                 if (msg_text[0] != ':')
//                 {
//                     SEND_MSG(it->first, SINTAX_ERROR);
//                     return ;
//                 }
//                 msg_text.erase(msg_text.begin());
//                 SEND_STRING(_user[user_name], msg_text);
//             }
//             _channel[channel_name].leave_chanel(&_client.at(_user[user_name]));
//         }
//     }
// }

// void    Operators::operator_LUSERS(iterator &it, const std::string& line)
// {
//     (void)it; (void)line;
//     // std::string msg = (it->second.getNick() + " " + std::to_string(_user.size()));
//     // SEND_ERR(it->first, msg, RPL_LUSEROP);
// }

// void    Operators::operator_CAP(iterator &it, const std::string& line)
// {
//     (void)it; (void)line;
// //     std::string sub;

// //     sub = line.substr(0, line.find(" "));
// //     if (sub == "LS")
// //     {
// //         ;// send(it->first, "002 [admin]\r\n", 11, 0);
// //         // SEND_CLIENT(it->first, "SERVER", "CAP", "*", "multi-prefix sasl");
// //     }
// }

// void    Operators::operator_PING(iterator &it, const std::string& line)
// {
//     std::string text;

//     text = line.substr(0, line.find(" "));
//     if (text[0] != ':')
//     {
//         SEND_MSG(it->first, SINTAX_ERROR);
//         return ;
//     }
//     else
//     {
//         SEND_CLIENT(it->first, "SERVER", "PING", " ", line.substr(1, text.size()).c_str());
//     }
// }

// void    Operators::operator_PONG(iterator &it, const std::string& line)
// {
//     std::string text;

//     text = line.substr(0, line.find(" "));
//     if (text[0] != ':')
//     {
//         SEND_MSG(it->first, SINTAX_ERROR);
//         return ;
//     }
//     else
//     {
//         SEND_CLIENT(it->first, "SERVER", "PONG", " ", line.substr(1, text.size()).c_str());
//     }
// }

// void    Operators::operator_USER(iterator &it, const std::string& line)
// {
//     if (it->second.isRegistered())
//     {
//         SEND_MSG(it->first, ERR_ALREADYREGISTERED);
//         return ;
//     }
//     int pos = 0;
//     std::string word;
//     std::string user;
//     std::string host;

//     user = line.substr(pos, line.find(" ", pos) - pos);
//     pos += user.size() + 1;
//     word = line.substr(pos, line.find(" ", pos) - pos);
//     pos += word.size() + 1;
//     word = line.substr(pos, line.find(" ", pos) - pos);
//     if (word[0] != ':')
//     {
//         pos += word.size() + 1;
//         host = word;
//     }
//     it->second.init(user, host, line.substr(pos + 1, line.size()));
//     it->second.registering();
// }

// void    Operators::operator_PART(iterator &it, const std::string& line)
// {
//     std::string word;

//     word = line.substr(0, line.find(" "));
//     if (!word.size())
//     {
//         SEND_MSG(it->first, ERR_NEEDMOREPARAMS);
//     }
//     else if (_channel.find(word) == _channel.end())
//     {
//         SEND_ERR(it->first, word, ERR_NOSUCHCHANNEL);
//     }
//     else if (!_channel[word].isAvelabel(&it->second))
//     {
//         SEND_ERR(it->first, word, ERR_NOTONCHANNEL);
//     }
//     else
//     {
//         _channel[word].leave_chanel(&it->second);
//         SEND_CHANEL(it->first, word, LEAVE_CHANNEL);
//     }
// }

// void    Operators::operator_JOIN(iterator &it, const std::string& line)
// {
//     std::string word;

//     word = line.substr(0, line.find(" "));
//     if (!word.size())
//     {
//         SEND_MSG(it->first, ERR_NEEDMOREPARAMS);
//     }
//     else if (word[0] != '#')
//     {
//         SEND_ERR(it->first, word, ERR_BADCHANMASK);
//     }
//     else
//     {
//         if (_channel.find(word) == _channel.end())
//             _channel.insert(std::make_pair(word, Channel(word)));
//         _channel[word].add_user(&it->second);
//         SEND_CHANEL(it->first, word, JOIN_CHANNEL);
//     }
// }

// void    Operators::operator_PRIVMSG(iterator &it, const std::string& line)
// {
//     std::string word;
//     std::string text;

//     word = line.substr(0, line.find(" "));
//     if (!word.size() || line.rfind(" ") == std::string::npos)
//     {
//         SEND_MSG(it->first, ERR_NORECIPIENT);
//         return ;
//     }
//     text = line.substr(word.size() + 1, line.size());
//     if (!text.size())
//     {
//         SEND_MSG(it->first, ERR_NOTEXTTOSEND);
//         return ;
//     }
//     if (text[0] != ':')
//     {
//         SEND_MSG(it->first, SINTAX_ERROR);
//         return ;
//     }
//     else
//         text.erase(text.begin());
//     if (word[0] == '#')
//     {
//         if (_channel.find(word) == _channel.end())
//         {
//             SEND_ERR(it->first, word, ERR_NOSUCHNICK);
//         }
//         else if (!_channel[word].isAvelabel(&it->second))
//         {
//             SEND_ERR(it->first, word, ERR_CANNOTSENDTOCHAN);
//         }
//         else
//             _channel[word].sendMsg(&it->second, text, "PRIVMSG");
//     }
//     else
//     {
//         if (_user.find(word) == _user.end())
//         {
//             SEND_ERR(it->first, word, ERR_NOSUCHNICK);
//         }
//         else
//             it->second.sendMsg(_user[word], text, word, "PRIVMSG");
//     }
// }

// void    Operators::operator_NOTICE(iterator &it, const std::string& line)
// {
//     std::string word;
//     std::string text;

//     word = line.substr(0, line.find(" "));
//     if (!word.size() || line.rfind(" ") == std::string::npos)
//     {
//         SEND_MSG(it->first, ERR_NORECIPIENT);
//         return ;
//     }
//     text = line.substr(word.size() + 1, line.size());
//     if (!text.size())
//     {
//         SEND_MSG(it->first, ERR_NOTEXTTOSEND);
//         return ;
//     }
//     if (text[0] != ':')
//     {
//         SEND_MSG(it->first, SINTAX_ERROR);
//         return ;
//     }
//     else
//         text.erase(text.begin());
//     if (word[0] == '#')
//     {
//         if (_channel.find(word) == _channel.end())
//         {
//             SEND_ERR(it->first, word, ERR_NOSUCHNICK);
//         }
//         else if (!_channel[word].isAvelabel(&it->second))
//         {
//             SEND_ERR(it->first, word, ERR_CANNOTSENDTOCHAN);
//         }
//         else
//             _channel[word].sendMsg(&it->second, text, "NOTICE");
//     }
//     else
//     {
//         if (_user.find(word) == _user.end())
//         {
//             SEND_ERR(it->first, word, ERR_NOSUCHNICK);
//         }
//         else
//             it->second.sendMsg(_user[word], text, word, "NOTICE");
//     }
// }
