#include "Channel.hpp"

Channel::Channel(std::string const channelName) : channelName(channelName)
{
}

Channel::~Channel()
{
}

std::string const   &Channel::getChannelName() const
{
    return (this->channelName);
}

void    Channel::setChannelName(std::string const name)
{
    this->channelName = name;
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
        delete temp;
    }
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
        delete temp;
    }
}

void Channel::broadcast(std::string const &msg, Client *client)
{
    std::map<int, Client*>::iterator it;

    for (it = this->getClientList().begin(); it != this->getClientList().end(); it++)
    {
        if (it->second != client)
            it->second->addSendBuff(msg);
    }
}

void Channel::broadcastWithMe(std::string const &msg)
{
    std::map<int, Client*>::iterator it;

    for (it = this->getClientList().begin(); it != this->getClientList().end(); it++)
    {
        it->second->addSendBuff(msg);
    }
}

