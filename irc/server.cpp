#include "server.hpp"

bool server::signal = false;

server::server()
{
    ssfd = -1;
}
void server::sinit()
{
    this->port = 4444;
    ssocket();

    std::cout << "Server <" << ssfd << "> Connected " << std::endl;
    std::cout << "waiting to accept a connection ..." << std::endl;
}

void server::clearclients(int fd)
{
    size_t i = 0;
    while(i < fds.size())
    {
        if(fds[i].fd == fd)
        {
            fds.erase(fds.begin() + i);
            break;
        }
        i++;
    }
}

void server::closefds()
{
    size_t i = 0;
    while(i < clients.size())
    {
        std::cout << "Client <" << clients[i].GetFd() << "> Disconnected" << std::endl;
        close(clients[i].GetFd());
    }
    if(ssfd != -1)
    {
        std::cout << "server <" << ssfd << "> Disocnnected " << std::endl;
        close (ssfd);
    }
}

void server::shandler(int fd)
{
    (void)fd;
    std::cout << std::endl << "Signal recieved !!" << std::endl;
    server::signal = true;
}

void server::ssocket()
{
    //setting the address type, port, and which ip address im using 
    struct sockaddr_in add;
    struct pollfd NewPoll;
    add.sin_family = AF_INET;
    add.sin_port = htons(this->port);
    add.sin_addr.s_addr = INADDR_ANY;
    
    ssfd = socket(AF_INET, SOCK_STREAM, 0);
    if(ssfd == -1)
        throw(std::runtime_error("failed to create socket"));
    int a = 1;

    if(setsockopt(ssfd, SOL_SOCKET, SO_REUSEADDR, &a, sizeof(a)) == -1)
        throw(std::runtime_error("failed to set option (SO_REUSEADDR) on socket"));
    if(fcntl(ssfd,F_SETFL, O_NONBLOCK) == -1)
        throw(std::runtime_error("failed to set option (O_NONBLOCK) on socket"));
    if(bind (ssfd,(struct sockaddr *)&add, sizeof(add)) == -1)
        throw(std::runtime_error("failed to bind socket"));
    if(listen(ssfd, SOMAXCONN) == -1)
        throw(std::runtime_error("listen() failed"));

    NewPoll.fd = ssfd;
    NewPoll.events = POLLIN;
    NewPoll.revents = 0;
    fds.push_back(NewPoll);
}

int main()
{
    server ser;
    std::cout << "------------------server---------------------------\n";
    try {
        signal(SIGINT, server::shandler);
        signal(SIGQUIT, server::shandler);
        ser.sinit();
    }
    catch(const std::exception& e){
        ser.closefds();
        std::cerr << e.what() << std::endl;
    }
    std::cout << "the server closed !!" << std::endl;
}
