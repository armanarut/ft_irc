#include "Bot.hpp"

Bot::Bot(char **av) :
                    m_host (av[1]),
                    m_port ((short)std::atoi(av[2])),
                    m_pass (av[3]),
                    m_nick (av[4]) {}

Bot::~Bot(){}
    
void    Bot::start_bot() {
    struct sockaddr_in mysock;

    m_fd = socket(AF_INET, SOCK_STREAM, 0); // Получить ФД
    getnameinfo((struct sockaddr*)&mysock, sizeof(mysock), m_host, NI_MAXHOST, NULL, 0, NI_NUMERICSERV);
    memset (&mysock, 0, sizeof (mysock)); // Инициализировать структуру
    mysock.sin_family = AF_INET; // Установить семейство адресов
    mysock.sin_port = htons(m_port); // Установить порт
    mysock.sin_addr.s_addr = inet_addr(m_host);; // Установить адрес
    // std::cout  <<  mysock.sin_addr.s_addr << std::endl; // Cheking
    
        if (connect(m_fd, (struct sockaddr*)&mysock, sizeof (mysock)) != -1){
            std::cout << "Server connected!" << std::endl;
            fcntl(m_fd, F_SETFL, O_NONBLOCK);
            run();
            std::cout << "Bot exit!" << std::endl;
            // break;
        }
        else
            std::cout << "Not Connected!" << std::endl;    
}
    
void    Bot::run(){
    
    sending("NICK " + m_nick);
    sending("PASS " + m_pass);
    sending("USER BOT" + m_nick + " " + my_to_string(m_port) + " " + m_host + " :Noname");

    int n = 0;
    std::string in_text;
    char buffer[BUF_SIZE];
    int val = 0;
    while (true)
    {
        // std::cout << m_fd << std::endl;// cheking
        n++;
        memset(buffer, 0, BUF_SIZE);
        val = recv(m_fd, buffer, BUF_SIZE, 0);
        if (val == 0)
            break; 
        in_text="";
        in_text.append(buffer);
        if (!in_text.empty()){
            // std::cout << in_text << "readCount " << valread << std::endl; // cheking
            if (check_command(in_text))
                break;  
        }
        if (n > 50000000){
            sending("PING " + m_nick + " " + m_host);
            n = 0;
        }


    }
}

int Bot::check_command(std::string in_text){
    std::string command = parsing_text(in_text);
    std::string msg;

    if (m_user.empty())
        return 0;//std::cout << "User not identifit" << std::endl;
    else if (command.empty())
        ;
    else if (command == "TIME\r\n")
        msg = get_time();
    else if (command == "HELLO\r\n")
        msg = " Hello! I'm " + m_nick + "!";
    else if (command == "HELP\r\n")
        msg = help_command();
    else
        msg = " Anhaskanali hramana (xntrum em chisht grek)!";
   
    std::cout << ("PRIVMSG " + m_user + msg) << std::endl; // cheking
    sending("PRIVMSG " + m_user + msg);
    
    return 0;
}

std::string Bot::help_command(){
    std::string str = " TIME - show time and date. HELLO - Hello! I'm bot!. HELP - command list.";
    return str;
}

std::string Bot::parsing_text(std::string in_text){
    m_user = "";
    size_t pos = in_text.find("PRIVMSG");
    if (!(pos < in_text.size()))
        return "";
    m_user = in_text.substr(1, in_text.find("!")-1);
    std::string msg = in_text.substr(in_text.find(":", 1) + 1, in_text.size());
    toUpperCase(msg);
    std::cout << "user :" + m_user + " / command :" + msg; // cheking
    return (msg);
}

std::string Bot::get_time(void)
{
    time_t rawtime;
    struct tm * timeinfo;
    time( &rawtime );
    timeinfo = localtime ( &rawtime );
    std::string dt = asctime (timeinfo);
    std::vector<std::string>  data_time;
    std::string temp;
    int pos = 0;
    
    temp = dt.substr(pos, dt.find(" ", pos) - pos);
    while(!temp.empty())
    {
        while(dt[pos] == ' ') pos++;
        temp = dt.substr(pos, dt.find(" ", pos + 1) - pos);
        pos += temp.size();
        data_time.push_back(temp);
    }
    data_time[4].resize(4);
    temp = " DATE : /" + data_time[0] + "/ " + data_time[2] + " ";
    temp += data_time[1] + " " + data_time[4] + "\n";
    temp += "TIME : " + data_time[3];
    return temp;
}

void	Bot::sending(const std::string& massage)
{
    std::string buffer = massage + "\r\n";

    if (send(m_fd, buffer.c_str(), buffer.length(), 0) < 0)
        throw std::runtime_error("Error: can't send message to client.");
}

std::string my_to_string(int num)
{
    std::stringstream stream;
    std::string str;
    stream << num;
    stream >> str;
    return str;
}

void  toUpperCase(std::string& str)
{
    for(size_t i = 0 ; i < str.size(); ++i) 
    { str[i] = std::toupper(str[i]); }
}

int main(int ac,char **av)
{
    if (ac != 5) {
        std::cout << " /host/ /port/ /password/ /nick/" << std::endl;
        return 0;
    }
    Bot bot(av);
    bot.start_bot();
    return 0;
}