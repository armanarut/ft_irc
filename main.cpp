#include "Server.hpp"
#include <cstdlib>

int main(int argc, char *argv[])
{
    if (argc != 3)
        prog_error("invalid count of arguments <port> <password>");
    /****************///check valid port
    if (!is_all_digit(argv[1]))
        prog_error("Port must to by digital, and in range [1 to 65535]");
    int port = std::atoi(argv[1]);
    if ( port < 1 || port > 65535)
        prog_error("Port must to by digital, and in range [1 to 65535]");
    Server server(port, argv[2]);

    return 0;
}