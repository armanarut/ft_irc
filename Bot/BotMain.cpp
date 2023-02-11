#include "Bot.hpp"

int main(int ac,char **av)
{
    if (ac != 5) {
        std::cout << " [host] <port> password nick" << std::endl;
        return 0;
    }
    Bot bot(av);
    bot.start_bot();
    return 0;
}