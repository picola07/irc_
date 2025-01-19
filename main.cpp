#include "includes/Server.hpp"
#include "includes/Client.hpp"


int main(int ac, const char **av)
{
    if (ac != 3)
    {
        std::cerr << "Error!\nUsage : ./exe PORT PASSWD" << std::endl;
        return 0;
    }
    
    Server server;
    int nb;
    std::stringstream  s(av[1]);
    s >> nb;
    try
    {
        // signal(SIGINT, Server::signalHandler);
        // signal(SIGQUIT, Server::signalHandler);
        server.runningServer(nb, av[2]);
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    std::cout << "bazinga" << std::endl;
    return 0;
}
