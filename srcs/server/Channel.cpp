#include "Channel.hpp"

Channel::Channel(std::string chanName, std::string chanPass, int maxUsers) : channelName(chanName)
{
	if (chanPass.size() == 0)
		this->channelPassword = "";
	else
		this->channelPassword = chanPass;
	this->usersInChannel = maxUsers;
}

Channel::~Channel()
{
	groupOperatorList.clear();
	clientList.clear();
	bannedList.clear();
}

int const &Channel::getMaxUsersInChannel() const
{
	return (this->usersInChannel);
}

void Channel::setMaxUsersInChannel(int const max)
{
	this->usersInChannel = max;
}

std::string const &Channel::getChannelName() const
{
	return (this->channelName);
}

void Channel::setChannelName(std::string const name)
{
	this->channelName = name;
}

std::string const &Channel::getChannelPassword() const
{
	return (this->channelPassword);
}

void Channel::setChannelPassword(std::string const password)
{
	this->channelPassword = password;
}

std::string const &Channel::getChannelTopic() const
{
	return (this->channelTopic);
}

void Channel::setChannelTopic(std::string const topic)
{
	this->channelTopic = topic;
}

std::vector<std::string> &Channel::getChannelModes()
{
	return (this->channelModes);
}

void Channel::addChannelMode(std::string const mode)
{
	if (!channelHasMode(mode))
	{
		channelModes.push_back(mode);
	}
}

void Channel::removeChannelMode(std::string const mode)
{
	for (std::vector<std::string>::iterator it = channelModes.begin(); it != channelModes.end(); ++it)
	{
		if (*it == mode)
		{
			channelModes.erase(it);
			break;
		}
	}
}

bool Channel::channelHasMode(std::string const mode)
{
	bool modeExists = false;
	for (std::vector<std::string>::const_iterator it = channelModes.begin(); it != channelModes.end(); ++it)
	{
		if (*it == mode)
		{
			modeExists = true;
			break;
		}
	}
	return (modeExists);
}

std::map<int, Client *> &Channel::getInvitationList()
{
	return (this->invitationList);
}

void Channel::addInvitationElement(const int fd, Client *newClient)
{
	if (newClient)
		this->invitationList.insert(std::make_pair(fd, newClient));
}

void Channel::deleteInvitationElement(const int fd)
{
	Client *temp = this->invitationList.find(fd)->second;
	if (temp)
	{
		this->invitationList.erase(fd);
	}
}

std::map<int, Client *> &Channel::getGroupVoicedList()
{
	return (this->goupVoicedList);
}

void Channel::addGroupVoicedElement(const int fd, Client *newClient)
{
	if (newClient)
		this->goupVoicedList.insert(std::make_pair(fd, newClient));
}

void Channel::deleteGroupVoicedElement(const int fd)
{
	Client *temp = this->goupVoicedList.find(fd)->second;
	if (temp)
	{
		this->goupVoicedList.erase(fd);
	}
}

std::map<int, Client *> &Channel::getGroupOperatorList()
{
	return (this->groupOperatorList);
}

void Channel::addGroupOperatorElement(const int fd, Client *newClient)
{
	if (newClient)
		this->groupOperatorList.insert(std::make_pair(fd, newClient));
}

void Channel::deleteGroupOperatorElement(const int fd)
{
	Client *temp = this->groupOperatorList.find(fd)->second;
	if (temp)
	{
		this->groupOperatorList.erase(fd);
	}
}

std::map<int, Client *> &Channel::getClientList()
{
	return (this->clientList);
}

void Channel::addClientElement(const int fd, Client *newClient)
{
	if (newClient)
		this->clientList.insert(std::make_pair(fd, newClient));
}

void Channel::deleteClientElement(const int fd)
{
	Client *temp = this->clientList.find(fd)->second;
	if (temp)
	{
		this->clientList.erase(fd);
	}
}

std::map<int, Client *> &Channel::getBannedList()
{
	return (this->bannedList);
}

void Channel::addBannedListElement(const int fd, Client *newClient)
{
	if (newClient)
		this->bannedList.insert(std::make_pair(fd, newClient));
}

void Channel::delBannedListElement(const int fd)
{
	Client *temp = this->bannedList.find(fd)->second;
	if (temp)
	{
		this->bannedList.erase(fd);
	}
}

void Channel::broadcast(std::string const &msg, Client *client)
{
	std::map<int, Client *>::iterator it;

	for (it = this->getGroupOperatorList().begin(); it != this->getGroupOperatorList().end(); it++)
	{
		if (it->second == client)
			continue;
		it->second->sendToClient(msg);
	}
	for (it = this->getClientList().begin(); it != this->getClientList().end(); it++)
	{
		if (it->second == client)
			continue;
		it->second->sendToClient(msg);
	}
}

void Channel::broadcastWithMe(std::string const &msg)
{
	std::map<int, Client *>::iterator it;

	for (it = this->getGroupOperatorList().begin(); it != this->getGroupOperatorList().end(); it++)
	{
		it->second->sendToClient(msg);
	}
	for (it = this->getClientList().begin(); it != this->getClientList().end(); it++)
	{
		it->second->sendToClient(msg);
	}
}

bool Channel::doesClientExist(const std::string &clientName)
{
	std::map<int, Client *>::iterator it;
	for (it = clientList.begin(); it != clientList.end(); ++it)
	{
		if (it->second->getNickname() == clientName)
			return true;
	}
	return false;
}

bool Channel::doesOperatorExist(const std::string &clientName)
{
	std::map<int, Client *>::iterator it;
	for (it = groupOperatorList.begin(); it != groupOperatorList.end(); ++it)
	{
		if (it->second->getNickname() == clientName)
			return true;
	}
	return false;
}

bool Channel::doesVoicedExist(const std::string &clientName)
{
	std::map<int, Client *>::iterator it;
	for (it = goupVoicedList.begin(); it != goupVoicedList.end(); ++it)
	{
		if (it->second->getNickname() == clientName)
			return true;
	}
	return false;
}

bool Channel::doesinvitationExist(const std::string &clientName)
{
	std::map<int, Client *>::iterator it;
	for (it = invitationList.begin(); it != invitationList.end(); ++it)
	{
		if (it->second->getNickname() == clientName)
			return true;
	}
	return false;
}

bool Channel::doesBanExist(const std::string &clientName)
{
	std::map<int, Client *>::iterator it;
	for (it = bannedList.begin(); it != bannedList.end(); ++it)
	{
		std::cout << it->second->getNickname() << std::endl;
		if (it->second->getNickname() == clientName)
			return true;
	}
	return false;
}
