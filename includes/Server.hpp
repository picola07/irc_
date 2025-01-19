#pragma once

#include <iostream>
#include <cstring>
#include <sys/socket.h>
// #include <sys/type.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdint.h>
#include <vector>
#include <cerrno>
#include <cstdio>
#include <poll.h>
#include <sstream>
#include <unistd.h>
#include "Client.hpp"
#include "Command.hpp"
#include "Channel.hpp"
#include <fcntl.h>
#include "Channel.hpp"
#include "Replies.hpp"

#define RPL_NICKCHANGE(oldnickname, nickname) (":" + oldnickname + " NICK " + nickname + CRLF)

#define BUFFER_SIZE 1024 * 1024
#define ERRNOSUCHCHANNEL()


class Server
{
public :
	
	/* FUNCTIONS SETTING UP THE SERVER */
	void setServerSock(int port);
	void acceptNewConnection();
	void runningServer(int port, const char *av);
	void processCommand(Client *, const char* message);

	bool equalStrings(const std::string& it, const std::string& compare);
	/* FUCNTIONS HANDLING SENDING AND RECIEVING MSG */
	void recieveData(int clienSock);
	void parseCommand(int newsocket);
	void removeClientFromServer(int clientSock);

	/* UTILS */
	void removeClientFromChannel(Client *client, const std::string& name, const std::string& toremove);

	static void signalHandler(int sig);
	void throwError(const char* msg, int fd);
	// void removeClientFromServer(int clientsock);
	void removeClientFromPollfd(int clientsock);
	void closeFd();

	
	/* SETTERS */
	void setServerPassWd(const char* av);
	
	/* GETTERS */
	int		getServerFd() const;
	bool 	emptyParam(const std::vector<std::string>&, const  std::vector<std::string>::iterator&, const int&, const std::string &);
	void	notCommand(const std::string&);

	std::string 	getPasswd() const;
	std::string 	getName() const;
	std::string		getRangeAsString(const std::vector<std::string>&, std::vector<std::string>::iterator,  size_t, std::string);
	// std::string getRangeAsString(std::vector<std::string> vec, size_t start, size_t end, std::string delimiter);


	std::vector<std::string>	getWords_(const std::string &);
	std::vector<struct pollfd>	getMonitor() const;
	
	Client	*getClientFromServer(const std::string&);
	Client	*getClientFromVectorByFd(int);
	Client	*getServerClient(const std::string &);

	uint16_t 					getMonitorSize() const;

	Server();
	~Server();

private	:
	int serverSock ;							//socket file descriptor for the server 
	std::string passwd;                     // Passwd jo join the server
	std::string serverName;
    struct sockaddr_in serverAddr;
	static bool running;
	std::vector<struct pollfd> monitor;
	std::vector<Client> clients;            // Storing the clients who successfully establishde a connection wiht the server
	std::vector<Client> invitee;            // Storing the clients who successfully establishde a connection wiht the server
	Command command;

};






