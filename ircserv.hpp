#pragma once

#define ERR_NONICKNAMEGIVEN     ":No nickname given"
#define ERR_ERRONEUSNICKNAME    " :Erroneous nickname"
#define ERR_NICKNAMEINUSE       " :Nickname is already in use"
#define ERR_NORECIPIENT         ":No recipient given"
#define ERR_NOTEXTTOSEND        ":No text to send"
#define ERR_CANNOTSENDTOCHAN    " :Cannot send to channel"
#define ERR_NOSUCHNICK          " :No such nick/channel"
#define ERR_UNKNOWNCOMMAND      " :Unknown command"
#define ERR_NEEDMOREPARAMS      " :Not enough parameters"
#define ERR_BADCHANMASK         " :Bad Channel Mask"
#define ERR_NOSUCHCHANNEL       " :No such channel"
#define ERR_NOTONCHANNEL        " :You're not on that channel"

#define JOIN_CHANNEL    "You joined to channel: "
#define LEAVE_CHANNEL    "You leave the channel: "

#define SEND_CHANEL(fd, word, msg)  \
    send(fd, msg, strlen(msg), 0); \
    send(fd, word.c_str(), word.length(), 0); \
    send(fd,"\r\n\r\n", 4, 0)

#define SEND_MSG(fd, msg)  \
    send(fd, msg, strlen(msg), 0); \
    send(fd,"\r\n\r\n", 4, 0)

#define SEND_ERR(fd, word, msg)  \
    send(fd, word.c_str(), word.length(), 0); \
    send(fd, msg, strlen(msg), 0); \
    send(fd,"\r\n\r\n", 4, 0)

/****************[utils]****************/
void	prog_error(std::string err);
bool	is_all_alpha(std::string& str);