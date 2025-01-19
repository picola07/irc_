#include "../includes/Server.hpp"

Server::Server()
{
	this->passwd = "";
	this->serverSock = -1;
	this->monitor.clear();
	this->clients.clear();
	std::string serverName = "IRC";
}

Server::~Server()
{
}

/***********************************/
/*                                 */
/* FUNCTIONS SETTING UP THE SERVER */
/*                                 */
/***********************************/

/* Creating the server and making it ready to recieve the incoming connections */
void Server::setServerSock(int port)
{
	struct pollfd serverPoll;
	int optval = 1;
	serverAddr.sin_family = AF_INET; // for IP v4
    serverAddr.sin_addr.s_addr = INADDR_ANY; // any address
    serverAddr.sin_port = htons((uint16_t)port); //port to communicate from

	this->serverSock = socket(AF_INET, SOCK_STREAM, 0);

	if(this->serverSock < 3)
		throwError("Socket Failed", 0);

	int sockopt = setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
	if (sockopt < 0)
		std::cout << "chi l3ayba tema " << std::endl;
	if (fcntl(serverSock, F_SETFL, O_NONBLOCK) == -1) //-> set the socket option (O_NONBLOCK) for non-blocking socket
		throw(std::runtime_error("faild to set option (O_NONBLOCK) on socket"));

	int _bind = bind(serverSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if (_bind == -1)
		throwError("Bind failed => ", serverSock);

	if (listen(serverSock, SOMAXCONN) == -1)
        throwError("Listen failed", serverSock);

	serverPoll.fd = serverSock;
	serverPoll.events = POLLIN;
	this->monitor.push_back(serverPoll);
}


/* Accepting the incoming connection from the clients */
void Server::acceptNewConnection()
{
	Client client;
	int newsocket;
	
	struct sockaddr_in sockAddrClient;  // The information about the client (IPv4/v6, port) will be filled in this struct
	struct pollfd clientPoll;
	socklen_t len = sizeof(sockAddrClient);

 	newsocket = accept(serverSock, (struct sockaddr*)&sockAddrClient, &len);
	if (newsocket < 0) {
        throwError("Conenction failed", serverSock);
	}
	std::cout << BCYAN << "Connection is established successfuly with client N˚" << newsocket - 3 << ENDC << std::endl;


	clientPoll.fd = newsocket;
	clientPoll.events = POLLIN; 

	client.setIpAddress(inet_ntoa(sockAddrClient.sin_addr));
	client.setClientSock(newsocket);
	this->clients.push_back(client);
	this->monitor.push_back(clientPoll);
}



void Server::runningServer(int port, const char *av)
{
	this->setServerSock(port);
	this->setServerPassWd(av);

	while(true)
	{

		if (poll(&(monitor[0]), monitor.size(), 0) == -1)
			this->throwError("poll failed\n", 0);

		for (size_t i = 0; i < monitor.size(); i++)
		{
			if (monitor[i].revents & POLLIN)
			{
				
				if (monitor[i].fd == getServerFd())
					acceptNewConnection();
				else
					recieveData(monitor[i].fd);
			}
		}
	}
	
	this->closeFd();
}



/************************************************/
/*          									*/
/* FUCNTIONS HANDLING SENDING AND RECIEVING MSG */
/*                          			        */
/************************************************/


/* Reading msg from client */
void Server::recieveData(int clientSock)
{
	//Client &client;
	char message[1024];
	int rbyte = recv(clientSock, message, sizeof(message) - 1, 0);
	if (rbyte < 0)
	{
		close(clientSock);
		std::cout << "Client {" << clientSock - 3 << "}" << " has been Disconnected." << std::endl;
		return;
	}
	message[rbyte] = '\0';
	Client *client = this->getClientFromVectorByFd(clientSock); // Pointing to the client who sent the message in the vector
	processCommand(client, message);
}

/* Sending msg to client */

/*********/
/*       */
/* UTILS */
/*       */
/*********/

void Server::throwError(const char* msg, int fd)
{
	close(fd);
	perror(msg);
	_exit(0);
}

void Server::closeFd()
{
	for(size_t i = 0; i < monitor.size(); i++)
		close(monitor[i].fd);
}


bool Server::running = true;

void Server::signalHandler(int sig)
{
	(void)sig;
    std::cout << std::endl << "Received SIGUSR1 signal!" << std::endl;
    Server::running = false;
}


void Server::removeClientFromServer(int clientSock)
{
	std::vector<Client>::iterator it = clients.begin();
	while (it != clients.end())
	{
		if (it->getClientSock() == clientSock)
			clients.erase(it);
		it++;
	}
}



void Server::removeClientFromPollfd(int clientsock)
{
	std::vector<struct pollfd>::iterator it = monitor.begin();
	for (it = monitor.begin(); it < monitor.end(); it++)
	{
		const int &i = it->fd;
		if (i == clientsock)
		{
			close(it->fd);
			monitor.erase(it);
		}
	}
	return;
}

/***********/
/*         */
/* SETTERS */
/*         */
/***********/

void Server::setServerPassWd(const char* av)
{
	this->passwd = av;
}


int Server::getServerFd() const
{
	return this->serverSock;
}


/***********/
/*         */
/* GETTERS */
/*         */
/***********/

std::vector<struct pollfd> Server::getMonitor() const
{
	return this->monitor;
}


uint16_t Server::getMonitorSize() const
{
	return this->monitor.size();
}



Client* Server::getClientFromVectorByFd(int _clientSock)
{
	Client* p = NULL;
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if (it->getClientSock() == _clientSock)
		{
			p = &(*it);
			return p;
		}
	}
	return NULL;
}


std::string Server::getPasswd() const
{
	return this->passwd;
}

std::string Server::getName() const
{
	return this->serverName;
}



std::vector<std::string> Server::getWords_(const std::string &str)
{

	std::vector<std::string> words;
	std::istringstream stream(str);
	std::string word;

	// Extract words using the stream extraction operator
	while (stream >> word)
	{
		words.push_back(word);
	}
	return words;
}


std::string Server::getRangeAsString(const std::vector<std::string>& vec, std::vector<std::string>::iterator it,  size_t end, std::string delimiter) 
{
	if (it >= vec.end() || end > vec.size()) 
		throw std::out_of_range("Invalid range specified.");
	// Extract range and concatenate strings
	std::string result = "";
	while (it != vec.begin() + end) 
	{
		if (!result.empty()) 
			result += delimiter; // Add a space between words
		result += *it;
		it++;
	}
	return result;
}


// std::string getRangeAsString(std::vector<std::string> vec, size_t start, size_t end, std::string delimiter) 
// {
// 	if (start > vec.size() || end > vec.size() || start > end) 
// 		throw std::out_of_range("Invalid range specified.");
// 	// Extract range and concatenate strings
// 	std::string result = "";
// 	std::vector<std::string>::iterator it = vec.begin() + start;
// 	while (it != vec.begin() + end) 
// 	{
// 		if (!result.empty()) 
// 			result += delimiter; // Add a space between words
// 		result += *it;
// 		it++;
// 	}
// 	return result;
// }

Client *Server::getClientFromServer(const std::string& nickname)
{
	size_t i = 0;
	while(i != clients.size())
	{
		if (clients[i].getNickName() == nickname)
			return &(clients[i]);
		i++;
	}
	return NULL;
}


Client *Server::getServerClient(const std::string &str)
{
	size_t i = 0;
	while (i != this->clients.size())
	{
		if (clients[i].getNickName() == str)
			return &(clients[i]);
		i++;	
	}
	return NULL;
}

// true when strings are identical
bool Server::equalStrings(const std::string& it, const std::string& compare)
{
	return (strncmp(it.c_str(), compare.c_str(), compare.size()) == 0);
}


void Server::notCommand(const std::string& str)
{
	if (str != "PASS"
		&& str != "INVITE"
		&& str != "MODE"
		&& str != "NICK"
		&& str != "USER"
		&& str != "KICK"
		&& str != "PRIVMSG"
		&& str != "PART"
		&& str != "WHO")
		return;
}

bool Server::emptyParam(const std::vector<std::string>& vec, const std::vector<std::string>::iterator& param, const int &fd, const std::string &msg)
{
	
	if (param == vec.end())
	{
		send(fd, msg.c_str(), msg.size(), 0);
		return true;
	}
	return false;
}

