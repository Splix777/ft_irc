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
		/*
		case ERR_NOTEXTTOSEND:
			msgBuf += " " + client->getNickname() + " :No text to send";
			break;
		case ERR_NORECIPIENT:
			msgBuf += " " + client->getNickname() + " :No recipient given PRIVMSG";
			break;
		*/
		case ERR_NEEDMOREPARAMS:
		    msgBuf += " NOTICE :Not enough parameters";
		    break;

		case ERR_NOTREGISTERED:
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
		{
			std::string msg = "401 " + client->getNickname() + " " + _args[1] + " :No such nick/channel";
			client->sendToClient(msg);
		}
		else
		{
			std::string msg = ":" + client->getNickname() + "!" + client->getRealname() + " PRIVMSG " + _args[1] + msgPart;
			it_channel->second->broadcast(msg, client);
		}
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
		{
			std::string msg = "401 " + client->getNickname() + " " + _args[1] + " :No such nick/channel";
			client->sendToClient(msg);
			//addToClientBuffer(server, client_fd, ERR_NOSUCHNICK(it_client->second.getNickname(), target));
		}
		else
		{
			// if the user does not exist but the channel does
			if (it_target == client_list.end())
			{
				if (isUserinChannel(it_target, it_channel) == true)
				{
					std::string msg = ":" + client->getNickname() + "!" + client->getRealname() + " @" + client->getHostName() + " PRIVMSG " + _args[1].insert(1, "#") + msgPart;
					it_channel->second->broadcast(msg, client);
				}
				else
				{
					std::string msg = "401 " + client->getNickname() + " " + _args[1] + " :No such nick/channel";
					client->sendToClient(msg);
				}
			}
			else
			{
				std::string msg = ":" + client->getNickname() + "!" + client->getRealname() + " PRIVMSG " + _args[1] + msgPart;
				it_target->second->sendToClient(msg);
			}
		}
	}
}

/*
void Prvmsg::isValidFormat()
{
	// NOTICE <msgtarget> <text>
	// 1          2         3
	std::string msg;
	std::string target;
	for (std::size_t i = 0; i < _args.size(); i++)
		msg += " " + _args[i];
	std::cout << msg << std::endl;
	size_t delimiter = msg.rfind(":");
	if (delimiter == std::string::npos)
		throw ERR_NOTEXTTOSEND;
	
	target = msg.substr(1, delimiter - 1);
	if (target.empty())
		throw ERR_NORECIPIENT;
	
	if (_args.size() < 3)
		throw ERR_NEEDMOREPARAMS;
}
*/