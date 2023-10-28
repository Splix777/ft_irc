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
	std::string channelName;
	std::string channelPassword;
	std::string channelTopic;
	std::string channelMode;
	std::map<int, Client *> groupOperatorList;
	std::map<int, Client *> clientList;
	std::map<int, Client *> kickedList;

	Channel();
	Channel(Channel const &copy);
	Channel &operator=(Channel const &copy);

public:
	Channel(std::string chanName, std::string chanPass);
	~Channel();

	std::string const &getChannelName() const;
	void setChannelName(std::string const name);

	std::string const &getChannelPassword() const;
	void setChannelPassword(std::string const password);

	std::string const &getChannelTopic() const;
	void setChannelTopic(std::string const topic);

	std::string const &getChannelMode() const;
	void setChannelMode(std::string const mode);

	std::map<int, Client *> &getGroupOperatorList();
	void addGroupOperatorElement(const int fd, Client *newClient);
	void deleteGroupOperatorElement(const int fd);

	std::map<int, Client *> &getClientList();
	void addClientElement(const int fd, Client *newClient);
	void deleteClientElement(const int fd);

	std::map<int, Client *> &getKickedList();
	void addKickedListElement(const int fd, Client *newClient);
	void delKickedListElement(const int fd);

	void broadcast(std::string const &msg, Client *client);
	void broadcastWithMe(std::string const &msg);

	bool doesClientExist(const std::string &clientName);
	bool doesOperatorExist(const std::string &clientName);
};

#endif
