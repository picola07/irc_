#pragma once
#include <ostream>

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdint.h>
#include <vector>
#include <poll.h>
#include <sstream>
#include <unistd.h>


class Client
{
public :
	
	Client();
	Client(const std::string &_userName, const std::string &_nickName);
	~Client();
	
	int 			getClientSock() const;

	void 			setClientSock(int fd);
	void 			setIpAddress(const std::string& _ipAddres);
	void 			setNickName(const std::string& value);
	void 			setModerator(bool val);
	void 			setUserName(const std::string& );
	void 			setRealName(const std::string& value);
	void 			setValid(bool );

	bool			isEligible() const;
	bool 			isEmptyName(const std::string& nick, const std::string& user) const;
	bool 			isModerator() const;
	bool 			isValid() const;

	std::string 	getIpAddress() const; 
	std::string 	getNickName() const;
	std::string 	getRealName() const;
	std::string 	getUserName() const;

	
private :
	bool valid;  //when entering correct passwd
	bool moderator; //when creator of channel or has been turned moderator
	int clientSock;
	std::string nickName;
	std::string userName;
	std::string ipAddress;
	std::string realName;
};
