#pragma once

#include "ircserv.hpp"

// class Server;

// class Bot : public Client
// {
//     public:
//         Bot(int fd) : Client(fd) {}
//         Bot(){}
//         ~Bot(){}

//     void    start_bot(Server *sr, std::map<int, Client>::iterator it, std::string line)
//     {
//         std::string word = line;
//         (void)sr;
//        if (word.size())
//         {
//             if (word == "TIME")
//                 ;// operator_CAP(it, line.substr(word.size() + 1, line.size()));
//             else if (word == "CLIST")
//                 ;// operator_PING(it, line.substr(word.size() + 1, line.size()));
//             else if (word == "LIST")
//                 ;// operator_PONG(it, line.substr(word.size() + 1, line.size()));
//             else
//             {
//                 SEND_ERR(it->first, word, ERR_UNKNOWNCOMMAND);
//             }
//         }
//     }
// };