#include "Bot.hpp"

Bot::Bot(char **av) :
                    m_host (av[1]),
                    m_port ((short)std::atoi(av[2])),
                    m_pass (av[3]),
                    m_nick (av[4]) { readBook(); }

Bot::~Bot(){ createBook(); }

void    Bot::start_bot() {
    struct sockaddr_in mysock;

    m_fd = socket(AF_INET, SOCK_STREAM, 0); // Получить ФД
    getnameinfo((struct sockaddr*)&mysock, sizeof(mysock), m_host, NI_MAXHOST, NULL, 0, NI_NUMERICSERV);
    memset (&mysock, 0, sizeof (mysock)); // Инициализировать структуру
    mysock.sin_family = AF_INET; // Установить семейство адресов
    mysock.sin_port = htons(m_port); // Установить порт
    mysock.sin_addr.s_addr = inet_addr(m_host);; // Установить адрес
    write (1, "Connect .", 9) ;
    int n = 0; 
    while (1)
    {
        if (connect(m_fd, (struct sockaddr*)&mysock, sizeof (mysock)) != -1){
            std::cout << "Server :" << m_host << ":" << m_port << " status :connected!" << std::endl;
            fcntl(m_fd, F_SETFL, O_NONBLOCK);
            run();
            std::cout << "Bot exit!" << std::endl;
            break;
        }
        else
        {
            sleep(1);
            n++;
            write (1, ".", 1) ;
        }
        if (n == 15)
        {
            write (1, "\nNot Connected! Wait 5s.", 25) ;
            sleep(5);
            write (1, "\nConnect .",11) ;
            n = 0;
        }    
    } 
}
    
void    Bot::run(){
    
    sending("NICK " + m_nick);
    sending("PASS " + m_pass);
    sending("USER BOT" + m_nick + " " + my_to_string(m_port) + " " + m_host + " :Noname");

    std::string in_text;
    char buffer[BUF_SIZE];
    int val = 0;
    while (true)
    {
        if(buffer[0])
            memset(buffer, 0, BUF_SIZE);
        val = recv(m_fd, buffer, BUF_SIZE, 0);
        if (val == 0)
            break; 
        in_text="";
        in_text.append(buffer);
        if (!in_text.empty()){
            if (check_command(in_text))
                break;  
        }
    }
}

int Bot::check_command(std::string &in_text){
    std::cout << "Incoming: " << in_text; //cheking
    std::string msg;
    std::string command = parsing_text(in_text, &msg);
    // std::cout << "command-|" << command << "|   msg-|" << msg << "|" << std::endl;//cheking

    if (m_user.empty() || command.empty())
        return 0;
    else if (check_book(command, &msg))
        ;
    else if (command == "SAVE")
        createBook();
    else if (command == "EXIT")
        return (0);
    else if (command == "TIME")
        msg = get_time();
    else if (command == "HELLO")
        msg = " Hello! I'm " + m_nick + "!";
    else if (command == "HELP")
        msg = help_command();
    else if (command == "LEARN")
        command_LEARN(&msg);
    else
        msg = " Anhaskanali hramana (xntrum em chisht grek)!";
   
    // std::cout << ("PRIVMSG " + m_user + msg) << std::endl; // cheking
    if (!msg.empty())
        sending("PRIVMSG " + m_user + msg);
    return 0;
}

bool    Bot::check_book(std::string &in_text, std::string *msg){
    std::map <std::string, std::string>::iterator it;

    it = m_book.find(in_text);
    if (it != m_book.end())
    {
        *msg = " " + (it->second);
        return true;
    }
    return false;
}

void    Bot::command_LEARN(std::string *msg)
{
    std::string key = *msg;
    std::string value = key.substr(key.find(":", 1) + 1, key.size());
    key = key.substr(0, key.size() - value.size());
    key = trim(key, " :");
    value = trim(value," :");
    toUpperCase(key);
    *msg = " I'am learning :" + key;
    if (key != "LEARN")
        m_book[key] = value;
    else
        *msg = "Not learning :" + key;
}

std::string Bot::parsing_text(std::string &in_text, std::string *msg){
    m_user = "";
    if (in_text.find("PRIVMSG") > in_text.size())
        return "";
    m_user = in_text.substr(1, in_text.find("!")-1);
    std::string temp = in_text.substr(in_text.find(":", 1) + 1, in_text.size());
    std::string command = temp;
    temp = temp.substr(temp.find(":", 0) + 1, temp.size());
    
    if (temp.size() < command.size())
        command = command.substr(0, command.size() - temp.size());
    else
        temp ="";
    temp = trim(temp, " :\r\n\t");
    command = trim(command," :\r\n");
    toUpperCase(command);
    *msg = temp;
    return command;
}


std::string Bot::help_command(){
    std::string str = " < TIME > - show time and date. < HELLO > - Hello! I'm bot!. < HELP > - command list. < LEARN > - Learn < :command :reaple> ";
    return str;
}

void	Bot::sending(const std::string& massage)
{
    std::string buffer = massage + "\r\n";

    if (send(m_fd, buffer.c_str(), buffer.length(), 0) < 0)
        std::cout << "Error: can't send message to client." << std::endl;
}

void    Bot::readBook()
{
    std::string _filename = "./book.txt";
    std::string line;
    std::ifstream in(_filename.c_str());
    if (!in.is_open())
        return;
    std::string key;
    std::string value;
    while(std::getline(in, line)) {
        key = line;
        value = key.substr(key.find(":", 1) + 1, key.size());
        key = key.substr(0, key.size() - value.size() - 1);
        m_book[key] = value;
    }
    in.close();
}

void    Bot::createBook()
{
    std::string _filename = "./book.txt";
    std::ofstream out(_filename.c_str());
    if (!out.is_open())
        std::cerr << "failed to create file!" << std::endl;
    for (std::map<std::string, std::string>::iterator it = m_book.begin(); it != m_book.end(); ++it) {
        out<< it->first <<":" << it->second << std::endl;
    }
    out.close();
}

int main(int ac,char **av)
{
    if (ac != 5) {
        std::cout << " /host/ /port/ /password/ /nick/" << std::endl;
        return 0;
    }
    if (!is_all_digit(av[2]))
        prog_error("Port must to by digital, and in range [1 to 65535]");
    int port = std::atoi(av[2]);
    if ( port < 1 || port > 65535)
        prog_error("Port must to by digital, and in range [1 to 65535]");
    Bot bot(av);
    bot.start_bot();
    return 0;
}