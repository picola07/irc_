#include "../includes/Server.hpp"


void Server::processCommand(Client* client, const char* message)
{
	
	const std::string& msg = message;
	std::vector<std::string> vec(getWords_(msg));               // Splitting the message sent by client word by word
	std::vector<std::string>::iterator it = vec.begin();        // Iterating through the vector containing the message  
	std::cout << message;
	// (void)client;


	while ((it != vec.end()))
	{
		notCommand(*it);

		if (equalStrings(*it, "PASS") && !client->isValid())
		{	
			if ((it + 1) == vec.end())
			{
				const std::string& msg = ERR_PASSWDMISMATCH(*(it + 1));
				command.sendData(client->getClientSock(), msg);
				return; // !! mandatory return statement
			}
			if (equalStrings(getRangeAsString(vec, it + 1, vec.size(), " "), getPasswd()))
			{
				client->setValid(true);
				command.sendData(client->getClientSock(), RPL_WELCOME(client->getNickName(), "IRC"));
			}
		}

		if (equalStrings(*it, "USER") && client->isValid())
		{
			if (emptyParam(vec, (it + 1), client->getClientSock(), ERR_NEEDMOREPARAMS(client->getNickName(), *it)))
				return;
			client->setUserName(*(it + 1));
		}
	

		if (equalStrings(*it, "NICK") && client->isValid())
		{
			if (emptyParam(vec, (it + 1), client->getClientSock(), ERR_NEEDMOREPARAMS(client->getNickName(), *it)))
				return;
			client->setNickName(*(it + 1));
			const std::string& msg = ":" + client->getNickName() + "!" + client->getUserName() + "@" + client->getIpAddress() + " NICK " + ":" + client->getNickName() + "\r\n";
			command.sendData(client->getClientSock(), msg);		
		}

		if (equalStrings(*it, "JOIN") && client->isEligible())
		{
			Channel *chan = NULL;
			const std::string& name = *(it + 1);
			if (emptyParam(vec, (it + 1), client->getClientSock(), ERR_NEEDMOREPARAMS(client->getNickName(), *it)))
				return;
			if (name[0] != '#' || name.size() == 1)
			{
				const std::string& msg = command.standardMsg(client->getNickName(), client->getUserName(), client->getIpAddress()) + " JOIN " + ERR_NEEDMOREPARAMS(client->getNickName(), "JOIN") ;
				command.sendData(client->getClientSock(), msg);
				return;
			}
			chan = command.getChannelByName(name);
			if (chan != NULL)
			{
				if (chan->getPasswdRequired())
				{
					if ((it + 2) == vec.end())
					{
						command.sendData(client->getClientSock(), ERR_CHANHASPASS(chan->getChannelName()));
						return;
					}
				}
			}
			command.joinCommand(client, name, *(it + 2));
		}

		if (equalStrings(*it, "PRIVMSG") && client->isEligible())
		{
			if (emptyParam(vec, (it + 1), client->getClientSock(), ERR_NEEDMOREPARAMS(client->getNickName(), *it)))
				return;
				
			const std::string& param = *(it + 1); 
			if (param[0] == '#') // channel's name
			{
				if ((it + 2) != vec.end())
					command.privmsgCommandChannel(param, client, getRangeAsString(vec, it + 2, vec.size(), " "));
				else
				{
					const std::string& msg =  ":" + client->getNickName() + "!" + client->getUserName() + "@" + client->getIpAddress() + ".IP PRIVMSG " + *(it + 1) + " " + ERR_NEEDMOREPARAMS(client->getNickName(), *it);
					command.sendData(client->getClientSock(), msg);
				}
				return; //!!
			}
			Client *_client = getClientFromServer(param);               //client to whom the msg will be sent
			if(_client != NULL)
			{
			
				command.privmsgCommandUser(_client, getRangeAsString(vec, it + 2, vec.size(), " "));
				return; //!!
			}
			else
			{
				command.sendData(client->getClientSock(), ERR_NOSUCHNICK(client->getNickName(), param));
				return ;
			}
		}

		

		if (equalStrings(*it, "MODE") && client->isEligible())
		{
			if ((it + 2) == vec.end())
				return;

			const std::string& name = *(it + 1);
			if ((name[0] != '#' || name.size() == 1) && ((it + 1) != vec.end() && (it + 2) != vec.end()) && command.channelExist(*(it + 1)))
			{
				const std::string& msg = ":" + client->getNickName()+ "!" + client->getUserName() + "@" + client->getIpAddress() + ".IP JOIN " + ERR_NEEDMOREPARAMS(client->getNickName(), "JOIN") ;
				command.sendData(client->getClientSock(), msg);
				return;
			}

		
			if ((equalStrings(*(it + 2), "+k") || equalStrings(*(it + 2), "+o") || equalStrings(*(it + 2), "+l") || equalStrings(*(it + 2), "-o")) && (it + 3) != vec.end())
				command.modeCommand(client, *(it + 1), *(it + 2), *(it + 3));

			else
				command.modeCommand(client, *(it + 1), *(it + 2), "NULL");

			return;
		}

		if(equalStrings(*it, "INVITE") && client->isEligible())
		{
			// std::cout << *(it + 1) << "\n";
			// std::cout << *(it + 2) << "\n";
			// if (it != vec.end() && ((it + 1) == vec.end() || (it + 2) == vec.end()))
			// {
			// 	const std::string &msg = ":IRC " + ERR_NEEDMOREPARAMS(client->getNickName(), *(it));
			// 	send(client->getClientSock(), msg.c_str(), msg.size(), 0);
			// 	return ;
			// }

			if (emptyParam(vec, (it + 1), client->getClientSock(), ERR_NEEDMOREPARAMS(client->getNickName(), *it)) 
				|| emptyParam(vec, (it + 2), client->getClientSock(), ERR_NEEDMOREPARAMS(client->getNickName(), *it)))
			{
				return ;
			}

			if (getClientFromServer(*(it + 1)) != NULL)
			{
				Client *invitedClient = getClientFromServer(*(it + 1));
				if (!command.channelExist(*(it + 2)))
				{
					const std::string &msg = ":IRC " + ERR_NOSUCHCHANNEL(client->getNickName(), *(it + 2));
					send(client->getClientSock(), msg.c_str(), msg.size(), 0);
					return ;
				}
				if(client->isModerator() != true)
				{
					const std::string &msg = ":IRC " + ERR_CHANOPRIVSNEEDED(client->getNickName(), *(it + 1));
					send(client->getClientSock(), msg.c_str(), msg.size(), 0);
					return ;
				}
				int check;
				check = command.inviteclientcheck(invitedClient, *(it + 2));
				if(check == 3)
				{
					const std::string &msg = ":IRC " + ERR_USERONCHANNEL(client->getNickName(), *(it + 2), *(it + 1));
					send(client->getClientSock(), msg.c_str(), msg.size(), 0);
					return;
				}

				command.getChannelByName(*(it + 2))->AddUser2Channel(invitedClient);
				const std::string& msg = command.standardMsg(client->getNickName(), client->getUserName(), client->getIpAddress()) + ".IP INVITE " + invitedClient->getNickName() + " " + command.getChannelByName(*(it + 2))->getChannelName() + "\r\n";
				command.sendData(invitedClient->getClientSock(), msg);

				// std::cout << "T33333333\n"; 
				const std::string& name = command.getChannelByName(*(it + 2))->getChannelName();
				const std::string& ms = command.standardMsg(invitedClient->getNickName(), invitedClient->getUserName(), invitedClient->getIpAddress()) + " JOIN " + name + " * " + invitedClient->getRealName() + " " + RPL_WELCOME(invitedClient->getNickName(), "IRC");

				command.sendData(invitedClient->getClientSock(), ms);
				return;
			}
			else
			{
				const std::string &msg = ":IRC " + ERR_NOSUCHNICK(client->getNickName(), *(it + 1));
				send(client->getClientSock(), msg.c_str(), msg.size(), 0);
				return;
				return;
			}
			return;

		}
		if(equalStrings(*it, "KICK") )
		{
			if (emptyParam(vec, (it + 1), client->getClientSock(), ERR_NEEDMOREPARAMS(client->getNickName(), *it)) 
				|| emptyParam(vec, (it + 2), client->getClientSock(), ERR_NEEDMOREPARAMS(client->getNickName(), *it)))
			{
				return ;
			}
			command.kickCommand(client, vec, it);
			int k = command.kickClientFromChannel(*(it + 1), *(it + 2));
			if (k != 0)
			{
				const std::string &msg = ":IRC " + ERR_USERNOTINCHANNEL(client->getNickName(), *(it + 2), (*(it + 1)));
				send(client->getClientSock(), msg.c_str(), msg.size(), 0);
				return ;
			}
		// 	else 
		// 	{
		// 		//send message to evryone tell them that the user is kicked 
		// 	}
		// 	// }
		}
		if (*it == "QUIT" && *(it + 1) == ":Leaving")
		{
			command.removeClientFromAllChannels(client->getClientSock()); 
			close(client->getClientSock());
			return;
		}
		it++;
	}
}