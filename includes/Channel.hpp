#pragma once


#include "Client.hpp"
#include <unordered_map>
// #include "Server.hpp"




class Channel
{
public :
	Channel();
	Channel(const std::string& _channelName);
	~Channel();

	void AddUser2Channel(Client*);
	void removeClientFromChannel(const std::string&);
	void resizeClientLimit(const size_t&);  //set Channel limit
	void setChannelLimit(const size_t&);
	void setInviteMode(const bool &);
	void setPasswdRequired(const bool&);
	void setPasswd(const std::string&);
	void setTopicMode(const bool&);
	void addClientToVector(Client);

	bool channelIsFull();
	bool userExist(const std::string &, int);
	bool getInviteMode();
	bool channelInviteModeOnly();
	bool getPasswdRequired() const;
				// command.getChannelByName(*(it + 1))->addClientToVector(invitedClient);
	bool getTopicMode() const;

	std::string getChannelName() const;
	std::string getChannelClientByName();
	std::string getPasswd() const;
	std::string getCurrentTimestamp();

	size_t getChannelLimit() const;
	size_t getChannelClientSize();
	
	std::vector<Client> *getChannelClientsVector();

	// template<typename T>
	// Client *getClientFromChannelByT(const T& name)
	// {
	// 	std::vector<Channel>::iterator it = channels.begin();

	// 	while(it != channels.end())
	// 	{
	// 		it->


	// 		it++;
	// 	}
	// }




	Client *getClientFromChannelByName(const std::string& name);

private :
	bool requirePasswd;                    // for joining a channel
	bool inviteMode;
	bool topicMode;
	size_t channelLimit;
	std::string channelName;
	std::string channelPasswd;
	std::vector<Client> channelClients;     // Storing the clients joining a channel
};

