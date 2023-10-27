#include "Channel.hpp"

Channel::Channel(std::string chanName, std::string chanPass) : channelName(chanName)
{
    if (chanPass.size() == 0)
        this->channelPassword = "";
    else
        this->channelPassword = chanPass;
}

Channel::~Channel()
{
    groupOperatorList.clear();
    clientList.clear();
    kickedList.clear();
}

std::string const   &Channel::getChannelName() const
{
    return (this->channelName);
}

void    Channel::setChannelName(std::string const name)
{
    this->channelName = name;
}

std::string const   &Channel::getChannelPassword() const
{
    return (this->channelPassword);
}

void    Channel::setChannelPassword(std::string const password)
{
    this->channelPassword = password;
}

std::map<int, Client*>  &Channel::getGroupOperatorList()
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

std::map<int, Client*>  &Channel::getClientList()
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

bool Channel::doesClientExist(const std::string &clientName)
{	
	std::map<int, Client*>::iterator it;
	for (it = clientList.begin(); it != clientList.end(); ++it) {
		if (it->second->getNickname() == clientName)
			return true;
	}
	return false;
}

std::map<int, Client *>   &Channel::getKickedList()
{
    return (this->kickedList);
}

void Channel::addKickedListElement(const int fd, Client *newClient)
{
    if (newClient)
        this->kickedList.insert(std::make_pair(fd, newClient));
}

void Channel::delKickedListElement(const int fd)
{
    Client *temp = this->kickedList.find(fd)->second;
    if (temp)
    {
        this->kickedList.erase(fd);
    }
}

void Channel::broadcast(std::string const &msg, Client *client)
{
    std::map<int, Client*>::iterator it;

    for (it = this->getClientList().begin(); it != this->getClientList().end(); it++)
    {
        if (it->second == client)
			continue;
		it->second->sendToClient(msg);
    }
}

void Channel::broadcastWithMe(std::string const &msg)
{
    std::map<int, Client*>::iterator it;

    for (it = this->getClientList().begin(); it != this->getClientList().end(); it++)
    {
        it->second->sendToClient(msg);
    }
}

