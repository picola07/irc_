#pragma once
#define CRLF "\r\n"
#
#define ERR_FUNCSEND "FATAL : send() Failed!"
#include <iostream>
#include <cstring>
#include <sys/socket.h>
// #include <sys/type.h>
#include <arpa/inet.h>
#include <netinet/in.h>
//#include <cstdint> 
#include <vector>
#include <poll.h>
#include <sstream>
#include <unistd.h>
#include <string>

#include "Channel.hpp"
#include "Client.hpp"
#include "Replies.hpp"


class Command
{
public :
	Command();
	~Command();

	
	bool channelExist(const std::string &);
	
	

	void	eligibiltyErr(Client *, const std::string&); //processing the returned value of isEligible() function
	void 	joinCommand(Client *, const std::string &, const std::string&);
	void 	modeCommand(Client *,const std::string &, const std::string &, const std::string &);
	void 	passCommand(Client *, const std::string &, const std::string &);
	void 	privmsgCommandChannel(const std::string &, Client *, const std::string &);
	void 	privmsgCommandUser(Client *, const std::string &);
	void 	sendData(int, const std::string &);
	void	removeClientFromAllChannels(const int& toremove);

	std::string standardMsg(std::string, std::string, std::string);


	std::vector<Channel> 	 getChannelVector() const;
	std::vector<std::string> getWords(const std::string &);


	Channel	*getChannelByName(const std::string &);

	int	 inviteclientcheck(Client* client, std::string const &name);
	void kickCommand(Client *client, std::vector<std::string> vec, std::vector<std::string>::iterator it);
	int  clientinthechannel(Client* client, std::string const &name);
	int	 userinthechannel(Client* client, std::string const &name, std::string const &usname);
	int 	kickClientFromChannel(const std::string &chaine, const std::string& toremove);
	void inputClient(Client *client, std:: string name);
	void sendToAll(Client *client,const std::string msg, std::string name);

private :
	std::string commandLine;
	std::string parameter1;
	std::string parameter2;
	std::vector<Channel> channels;
};


