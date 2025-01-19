#include <iostream>
#include <vector>


void Command::removeClientFromChannel(const std::string& name, const std::string& client)
{
	Channel *channel = getChannelByName(name);
	if (channel)
	{
		std::vector<std::string> vec((*channel).getChannelClientsVector());
		std::vector<std::string>::iterator it = channel->getChannelClientsVector().begin();

	}
}