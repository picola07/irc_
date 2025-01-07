#include "server.hpp"

class client
{
private:
    int fd;
    std::string IPadd;
public:
    client();
    ~client();
    int GetFd();
    void setFd(int _fd);
    void setIPadd(std::string _IPadd);
};
