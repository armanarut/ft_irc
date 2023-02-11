#pragma once

#include <iostream>
#include <sstream> 
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>

#define BUF_SIZE 1024

std::string my_to_string(int num);

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
        

    private: // metods
        void        run();;
        void	    sending(const std::string& massage);
        int         check_command(std::string in_text);;
        std::string parsing_text(std::string in_text);;
        std::string get_time(void);
};

