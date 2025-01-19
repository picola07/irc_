#include "../includes/Client.hpp"

#define PORT 8080
#define BUFFER_SIZE 1024


Client::Client() : valid(false), moderator(false), nickName("Guest"), userName("Guest")
{
    this->realName = "realname";
}

Client::Client(const std::string& _userName, const std::string& _nickName) : valid(false), moderator(false), nickName(_nickName), userName(_userName)
{
    this->realName = "realname";
}

Client::~Client()
{

}


void Client::setClientSock(int fd)
{
    this->clientSock = fd;
}

void Client::setUserName(const std::string& value)
{
    if (value.empty())
        throw std::out_of_range("OHO USERNAME");
    this->userName = value;
}

void Client::setNickName(const std::string& value)
{
    if (value.empty())
        throw std::out_of_range("OHO NICKNAME");
    this-> nickName = value;
} 

void Client::setRealName(const std::string& value)
{
    this-> realName = value;
}

void Client::setIpAddress(const std::string& _ipAddress)
{
    this->ipAddress = _ipAddress;
}

int Client::getClientSock() const
{
    return this->clientSock;
}

void Client::setValid(bool val)
{
    this->valid = val;
}


std::string Client::getNickName() const
{
    return this->nickName;
}

std::string Client::getUserName() const
{
    return this->userName;
}

std::string Client::getIpAddress() const
{
    return this->ipAddress;
}

void Client::setModerator(bool val)
{
    this->moderator = val;
}


bool Client::isModerator() const
{
    return this->moderator;
}

bool Client::isEmptyName(const std::string& nick, const std::string& user) const
{
    if (nick.empty() || user.empty() || strncmp(nick.c_str(), "Guest", sizeof("Guest")) == 0 || strncmp(user.c_str(), "Guest", sizeof("Guest")) == 0)
        return true;
    return false;
}


std::string Client::getRealName() const
{
	return this->realName;
}


bool Client::isValid() const
{
	return this->valid;
}



// Checking if the client is connected with the server with a nick and user non empty and not "Guest"
// It doesn't check if the nick is already taken a by channel's member
// Returns false when client had everything okay
bool Client::isEligible() const
{
    if (isValid() == true && !isEmptyName(getNickName(), getUserName()))
        return true;
    return false;
}