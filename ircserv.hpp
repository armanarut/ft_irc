#pragma once

#define BUF_SIZE 1024

#define ERR_NONICKNAMEGIVEN     " :No nickname given"
#define ERR_ERRONEUSNICKNAME    " :Erroneous nickname"
#define ERR_NICKNAMEINUSE       " :Nickname is already in use"
#define ERR_NORECIPIENT         " :No recipient given"
#define ERR_NOTEXTTOSEND        " :No text to send"
#define ERR_CANNOTSENDTOCHAN    " :Cannot send to channel"
#define ERR_NOSUCHNICK          " :No such nick/channel"
#define ERR_UNKNOWNCOMMAND      " :Unknown command"
#define ERR_NEEDMOREPARAMS      " :Not enough parameters"
#define ERR_BADCHANMASK         " :Bad Channel Mask"
#define ERR_NOSUCHCHANNEL       " :No such channel"
#define ERR_NOTONCHANNEL        " :You're not on that channel"
#define ERR_CHANOPRIVSNEEDED	" :You're not channel operator"
#define ERR_USERNOTINCHANNEL	" :They aren't on that channel"

#define RPL_LUSEROP             " :operator(s) online"

#define JOIN_CHANNEL    "You joined to channel: "
#define LEAVE_CHANNEL    "You leave the channel: "

#define SEND_CHANEL(fd, word, msg)  \
    send(fd, msg, strlen(msg), 0); \
    send(fd, word.c_str(), word.length(), 0); \
    send(fd,"\r\n", 2, 0)

#define SEND_MSG(fd, msg)  \
    send(fd, msg, strlen(msg), 0); \
    send(fd,"\r\n", 2, 0)

#define SEND_STRING(fd, str)  \
    send(fd, str.c_str(), str.length(), 0); \
    send(fd,"\r\n", 2, 0)

#define SEND_ERR(fd, word, msg)  \
    send(fd, word.c_str(), word.length(), 0); \
    send(fd, msg, strlen(msg), 0); \
    send(fd,"\r\n", 2, 0)

#define SEND_CLIENT(fd, nickname, command, nick_or_channel, msg)  \
    const std::string _nickname(nickname);                                    \
    const std::string _command(command);                                    \
    const std::string _nick_or_channel(nick_or_channel);                     \
    const std::string _msg(msg);                                              \
    send(fd, (":" + _nickname + " " + _command + " ").c_str(), _nickname.length() + _command.length() + 3, 0);    \
    send(fd, (_nick_or_channel + " :").c_str(), _nick_or_channel.length() + 2, 0);    \
    send(fd, _msg.c_str(), _msg.length(), 0); \
    send(fd,"\r\n", 2, 0)

#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <map>
#include <vector>
#include <iostream>
// #include <cerrno>
// #include <cstring>
// #include <sstream>

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>

#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Bot.hpp"


/****************[utils]****************/
void	prog_error(std::string err);
bool	is_all_alpha(std::string str);
bool	is_all_digit(std::string str);
