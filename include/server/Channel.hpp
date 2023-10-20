#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include <exception>
#include <iostream>
#include <map>
#include <vector>

class Channel
{
  private:
    std::string	channelName;
    std::map<int, Client *>	clientList;
    std::map<int, Client *>	kickedList;

    Channel();
    Channel(Channel const &copy);
    Channel	&operator=(Channel const &copy);

  public:
    Channel(std::string const channelName);
    ~Channel();

    std::string const	&getChannelName() const;
    void				setChannelName(std::string const name);

    std::map<int, Client *>	&getClientList();
    void					addClientElement(const int fd, Client *newClient);
    void					deleteClientElement(const int fd);


    std::map<int, Client *>	&getKickedList();
    void					addKickedListElement(const int fd, Client *newClient);
    void					delKickedListElement(const int fd);

    void broadcast(std::string const &msg, Client* client);
    void broadcastWithMe(std::string const &msg);

};

#endif // CAN_CHANNEL_HPP
