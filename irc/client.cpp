#include "client.hpp"

client::client(){};
client::~client(){};

int client::GetFd()
{
    return fd;
}
void client::setFd(int _fd)
{
    fd = _fd;
}

void client::setIPadd(std::string _IPadd)
{
    IPadd = _IPadd;
}
