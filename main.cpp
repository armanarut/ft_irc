#include "Server.hpp"
#include <cstdlib>

int main(int argc, char *argv[])
{
    if (argc != 3)
        prog_error("invalid count of arguments <port> <password>");
    /****************///check valid port
    Server server(std::atoi(argv[1]), argv[2]);



    return 0;
}