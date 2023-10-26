#include "Prvmsg.hpp"
#include "IO.hpp"

Prvmsg::Prvmsg(Server *serv) : Notice(serv)
{
}

Prvmsg::~Prvmsg()
{
}

void Prvmsg::exec(Client *client)
{
	if (client->getDebug())
		printDebug("PRIVMSG Command Found, Executing PRIVMSG Command");
	try
	{
		isValidFormat();
		checkClientLevel(client);
		sendPrivmsg(client);
	}
	catch (int numeric)
	{
		std::stringstream sstm;
		sstm << numeric << " " << client->getSockFd();
		std::string msgBuf = sstm.str();
		switch (numeric)
		{
		case ERR_NEEDMOREPARAMS:
		    msgBuf += " NOTICE :Not enough parameters";
		    break;

		case ERR_NOTREGISTERED:
			throw ERR_NOTREGISTERED;
		    msgBuf += " :You have not registered";
		    break;

		default:
		    break;
		}
		client->sendToClient(msgBuf);
		_command.clear();
		_args.clear();
	}
}

void Prvmsg::sendPrivmsg(Client *client)
{
	std::map<int, Client *> client_list = (*_server).getClientList();
	std::map<std::string, Channel *> channel_list = (*_server).getChannelList();

	// Concat <text>
	std::string msgPart;
	for (std::size_t i = 2; i < _args.size(); i++)
		msgPart += " " + _args[i];

	// Notice to channel
	if (getNoticeType() == CHANNEL)
	{
		std::map<std::string, Channel *>::iterator it_channel = channel_list.find(_args[1]);
		if (it_channel == channel_list.end())
			return;
		// # define RPL_PRIVMSG(nick, username, target, message) (":" + nick + "!" + username + "@localhost PRIVMSG " + target + " " + message + "\r\n")
		std::string msg = ":" + client->getNickname() + "!" + client->getRealname() + " PRIVMSG " + _args[1] + msgPart;
		it_channel->second->broadcast(msg, client);
	}
	else
	{
		std::map<std::string, Channel *>::iterator it_channel = channel_list.find(_args[1]);
		std::map<int, Client *>::iterator it_target = client_list.begin();
		while (it_target != client_list.end())
		{
			if (it_target->second->getNickname() == _args[1])
				break; // Found user
			it_target++;
		}
		// If user and channel doesn't exist
		if (it_target == client_list.end() && it_channel == channel_list.end())
			return;
		else
		{
			// if the user does not exist but the channel does
			if (it_target == client_list.end())
			{
				if (isUserinChannel(it_target, it_channel) == true)
				{
					std::string msg = ":" + client->getNickname() + "!" + client->getRealname() + " PRIVMSG " + _args[1].insert(1, "#") + msgPart;
					it_channel->second->broadcast(msg, client);
				}
				else
					return;
			}
			else
			{
				std::string msg = ":" + client->getNickname() + "!" + client->getRealname() + " PRIVMSG " + _args[1] + msgPart;
				it_target->second->sendToClient(msg);
			}
		}
	}
}
