#include "Notice.hpp"
#include "Replies.hpp"
#include "IO.hpp"

Notice::Notice(Server *serv) : ACommand(serv)
{
}

Notice::~Notice()
{
}

void Notice::exec(Client *client)
{
	if (client->getDebug())
		printDebug("Notice Command Found, Executing Notice Command");
	try
	{
		isValidFormat();
		checkClientLevel(client);
		sendNotice(client);
	}
	catch (int numeric)
	{
		// Dont send error in NOTICE command
		_command.clear();
		_args.clear();
	}
}

void Notice::sendNotice(Client *client)
{
	std::map<int, Client *> client_list = _server->getClientList();
	std::map<std::string, Channel *> channel_list = _server->getChannelList();

	// Concat <text>
	std::string msgPart;
	for (std::size_t i = 2; i < _args.size(); i++)
		msgPart += " " + _args[i];


	// Notice to channel
	if (getTargetType() == CHANNEL)
	{
		std::map<std::string, Channel *>::iterator it_channel = channel_list.find(_args[1]);
		if (it_channel == channel_list.end())
			return;
		
		//std::string msg = ":" + client->getNickname() + "!" + client->getRealname() + "@localhost" + " NOTICE " + _args[1] + msgPart;
		//it_channel->second->broadcast(msg, client);
		it_channel->second->broadcast(_NOTICE(client->getNickname(), client->getRealname(), client->getHostName(), _args[1], msgPart), client);
	}
	else
	{
		std::map<std::string, Channel *>::iterator it_channel = channel_list.find(_args[1]);
		std::map<int, Client *>::iterator it_target = client_list.begin();
		while (it_target != client_list.end())
		{
			if (it_target->second->getNickname() == _args[1])
				break;
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
				if (it_channel->second->doesClientExist(_args[1]))//if (isUserinChannel(it_target, it_channel) == true)
				{
					//std::string msg = ":" + client->getNickname() + "!" + client->getRealname() + "@localhost" + " NOTICE " + _args[1].insert(1, "#") + msgPart;
					//it_channel->second->broadcast(msg, client);
					it_channel->second->broadcast(_NOTICE(client->getNickname(), client->getRealname(), client->getHostName(), _args[1].insert(1, "#"), msgPart), client);
				}
				else
					return;
			}
			else
			{
				//std::string msg = ":" + client->getNickname() + "!" + client->getRealname() + "@localhost" + " NOTICE " + _args[1] + msgPart;
				//it_target->second->sendToClient(msg);
				it_target->second->sendToClient(_NOTICE(client->getNickname(), client->getRealname(), client->getHostName(), _args[1], msgPart));
			}
		}
	}
}

bool Notice::isUserinChannel(std::map<int, Client *>::iterator it_client, std::map<std::string, Channel *>::iterator it_channel)
{
	const std::string &clientNickname = it_client->second->getNickname();
	std::map<int, Client *> &channelClients = it_channel->second->getClientList();
	for (std::map<int, Client *>::iterator it = channelClients.begin(); it != channelClients.end(); ++it)
	{
		if (it->second->getNickname() == clientNickname)
		{
			return true;
		}
	}
	return false;
}

Notice::typeSend Notice::getTargetType()
{
	// Notice to channel NOTICE <#msgtarget> <text>
	// Notice to user NOTICE <msgtarget> <text>
	return _args[1][0] == '#' ? CHANNEL : USER;
}

void Notice::checkClientLevel(Client *client)
{
	if ((client->getMemberLevel() & REGISTERED) != REGISTERED)
		throw ERR_NOTREGISTERED;
}

void Notice::isValidFormat()
{
	// NOTICE <msgtarget> <text>
	// 1          2         3
	if (_args.size() < 3)
		throw ERR_NEEDMOREPARAMS;
}
