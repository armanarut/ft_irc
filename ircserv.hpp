#pragma once

#define BUF_SIZE 1024

#define ERR_NORECIPIENT(source, command)        "411 " + source + " :No recipient given (" + command + ")"
#define ERR_NOTEXTTOSEND(source)                "412 " + source + " :No text to send"
#define ERR_UNKNOWNCOMMAND(source, command)     "421 " + source + " " + command + " :Unknown command"
#define ERR_NEEDMOREPARAMS(source, command)     "461 " + source + " " + command + " :Not enough parameters"
#define ERR_ALREADYREGISTERED(source)           "462 " + source + " :You may not reregister"
#define ERR_PASSWDMISMATCH(source)              "464 " + source + " :Password incorrect"
#define ERR_NOTREGISTERED(source)               "451 " + source + " :You have not registered"
#define ERR_ERRONEUSNICKNAME(source, command)   "432 " + source + " " + command + " :Erroneous nickname"
#define ERR_NICKNAMEINUSE(source, command)      "433 " + source + " " + command + " :Nickname is already in use"
#define ERR_NONICKNAMEGIVEN(source)             "431 " + source + " :No nickname given"
#define ERR_NOSUCHNICK(source, command)         "401 " + source + " " + command + " :No such nick/channel"
#define ERR_CANNOTSENDTOCHAN(source, command)   "404 " + source + " " + command + " :Cannot send to channel"

#define ERR_BADCHANMASK         " :Bad Channel Mask"
#define ERR_NOSUCHCHANNEL       " :No such channel"
#define ERR_NOTONCHANNEL        " :You're not on that channel"
#define ERR_CHANOPRIVSNEEDED	" :You're not channel operator"
#define ERR_USERNOTINCHANNEL	" :They aren't on that channel"
 
#define RPL_LUSEROP             " :operator(s) online"
#define RPL_MSG(prefix, command, target, massage)    ":" + prefix + " " + command + " " + target + " :" + massage
#define RPL_WELCOME(source)                     "001 " + source + " :Welcome to the ft_irc Network"

#define SINTAX_ERROR   " :Sintax error"
#define JOIN_CHANNEL    "You joined to channel: "
#define LEAVE_CHANNEL    "You leave the channel: "

/****************[utils]****************/
void	prog_error(std::string err);
bool	is_all_alpha(std::string str);
bool	is_all_digit(std::string str);
