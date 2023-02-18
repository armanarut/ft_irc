#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024


void        toUpperCase(std::string& str);
void        prog_error(std::string err);
bool        is_all_digit(std::string str);
std::string my_to_string(int num);
std::string get_time(void);
std::string ltrim(const std::string &s, const std::string &trimed);
std::string rtrim(const std::string &s, const std::string &trimed);
std::string trim(const std::string &s, const std::string &trimed);

class Bot {

    public:
        
        Bot(char **av);
        ~Bot();
    
        void        start_bot();

    private: // constr
        Bot() {};
        Bot(const Bot & ) {};
        Bot & operator=(const Bot &) { return *this; };

    private: // parametrs
        char *m_host;
        short m_port;
        int m_fd;
        std::string m_pass;
        std::string m_nick;
        std::string m_user;
        std::map <std::string, std::string> m_book;
        

    private: // metods
        void        run();
        void        readBook();
        void        createBook();
        void	    sending(const std::string& massage);
        int         check_command(std::string &in_text);
        bool        check_book(std::string &in_text, std::string *msg);
        std::string parsing_text(std::string &in_text, std::string *msg);
        std::string help_command();
        void        command_LEARN(std::string *msg);
};


// ftp server sintax : /DCC send <nickName> :