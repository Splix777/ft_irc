#include "Mode.hpp"
#include "Replies.hpp"
#include "IO.hpp"

Mode::Mode(Server *serv) : ACommand(serv)
{
}

Mode::~Mode()
{
}

void Mode::exec(Client *client)
{
	if (client->getDebug())
		printDebug("Mode Command Found, Executing Mode Command");

	try
	{
		isValidFormat();
		checkClientLevel(client);
		modeCmd(client);
		if (client->getDebug())
			printDebug("Mode Command Passed");
	}
	catch (int numeric)
	{
		std::stringstream sstm;
		sstm << numeric << " " << client->getSockFd();
		std::string msgBuf = sstm.str();
		switch (numeric)
		{
		case ERR_UNKNOWNERROR:
			msgBuf += " Mode :Invalid Format error !";
			break;

		case ERR_NEEDMOREPARAMS:
			msgBuf += " :Not enough parameters";
			break;

		case ERR_NOSUCHCHANNEL:
			msgBuf += " :No such channel";
			break;

		case ERR_NOTREGISTERED:
			msgBuf += " :You have not registered";
			break;

		default:
			break;
		}
		client->sendToClient(msgBuf);
	}
	_command.clear();
	_args.clear();
}

void Mode::isValidFormat(void)
{
	// Mode [<channel> [<o>]]

	// Right now we only support Mode <channel>
	// In the future Mode will support Mode <mask> and Mode <mask> o

	// Check if there are 2 arguments
	// if (_args.size() > 4)
	// 	throw ERR_UNKNOWNERROR;
	if (_args.size() < 2)
		throw ERR_NEEDMOREPARAMS;
	// Check if the first argument is a channel name
	// if (_args[1][0] != '#')
	//     throw ERR_UNKNOWNERROR;
}

void Mode::checkClientLevel(Client *client)
{
	if (!(client->getMemberLevel() & REGISTERED))
		throw ERR_NOTREGISTERED;
}

void Mode::modeCmd(Client *client)
{
	// Mode [<channel> [<o>]]

	// Check if the channel exists.
	// if (_server->getChannelList().find(_args[1]) == _server->getChannelList().end())
	//    throw ERR_NOSUCHCHANNEL;

	std::map<std::string, Channel *> channels = _server->getChannelList();
	std::map<std::string, Channel *>::iterator it_channel = channels.find(_args[1]);
	// Find target
	// if (getTargetType() == CHANNEL)
	// {
	if (it_channel == channels.end())
	{
		client->sendToClient(_CHANNELNOTEXIST(client->getHostName(), client->getNickname(), _args[1]));
		return;
	}
	if (_args.size() == 2)
	{
		std::string msgBuf = ":IRC 324 " + client->getNickname() + " " + _args[1] + " ";
		std::string modes;
		for (std::vector<std::string>::const_iterator it = it_channel->second->getChannelModes().begin(); it != it_channel->second->getChannelModes().end(); ++it)
		{
			modes += *it;
		}
		client->sendToClient(msgBuf + modes);
		return;
	}
	if (it_channel->second->doesOperatorExist(client->getNickname()))
		setChannelMode(client, it_channel->second);
	// 	return;
	// }
	// // Set client mode
	// setClientMode(client, it_channel->second);
}

void Mode::setClientMode(Client *client, Channel *channel)
{
	// (void)channel;
	// std::string msgBuf = ":IRC 324 " + client->getNickname() + " " + _args[1] + " +nt";
	// client->sendToClient(msgBuf);
	std::map<int, Client *>::iterator it_target = channel->getClientList().begin();
	while (it_target != channel->getClientList().end())
	{
		if (it_target->second->getNickname() == _args[3])
			break; // Found user
		it_target++;
	}

	// If user not found in channel
	if (it_target == channel->getClientList().end())
	{
		client->sendToClient(_NONICKORCHANNEL(client->getNickname(), _args[3]));
		return;
	}
	std::string response;
	std::string mode = "n";
	for (size_t i = 0; i < _args[3].length(); i++)
	{
		if (_args[3][i] == '+' || _args[2][i] == '-')
		{
			mode = "+";
			if (_args[3][i] == '-')
				mode = '-';
			i++;
		}
		else if (mode == "n") // invalid mode rcv, do nothing
			return;
		std::string toAdd(1, _args[3][i]);
		if (toAdd == "o") // set channel max users limit
		{
			if (mode == "+")
			{
				channel->addGroupOperatorElement(it_target->second->getFd(), it_target->second);
				channel->deleteClientElement(it_target->second->getFd());
				it_target->second->setMemberLevel(OPERATOR);
				response = _USERMODESET(it_target->second->getNickname(), "+o");
			}
			else
			{
				channel->deleteGroupOperatorElement(it_target->second->getFd());
				channel->addClientElement(it_target->second->getFd(), it_target->second);
				client->setMemberLevel(REGISTERED);
				response = _USERMODESET(it_target->second->getNickname(), "-o");
			}
		}
		else if (toAdd == "v") // Add/change key to room
		{
			if (mode == "+")
			{
				it_target->second->setMemberLevel(VOICED);
				response = _USERMODESET(it_target->second->getNickname(), "+v");
			}
			else
			{
				it_target->second->setMemberLevel(REGISTERED);
				response = _USERMODESET(it_target->second->getNickname(), "-v");
			}
		}
		// send to emisor
		client->sendToClient(response);
		// send to target
		if (it_target->second->getNickname() != client->getNickname())
			it_target->second->sendToClient(response);
	}
}

Mode::typeSend Mode::getTargetType()
{
	// Mode to channel MODE <#msgtarget> <modes> ?<user>
	// Mode to user MODE <msgtarget> <modes> ?<user>

	return _args[1][0] == '#' ? CHANNEL : USER;
}

void Mode::setChannelMode(Client *client, Channel *channel)
{
	std::string mode = "n";
	std::size_t paramIndex = 3;

	// Process input modes
	for (size_t i = 0; i < _args[2].length(); i++)
	{
		if (_args[2][i] == '+' || _args[2][i] == '-')
		{
			mode = "+";
			if (_args[2][i] == '-')
				mode = '-';
			i++;
		}
		else if (mode == "n") // invalid mode rcv, do nothing
			return;
		// add or remove from channel
		std::string toAdd(1, _args[2][i]);
		// if/elif manage commands with arguments
		if (toAdd == "l") // set channel max users limit
		{
			// If returns false, the command have a error, then proced to next command
			if (!processCommandL(client, channel, toAdd, mode, paramIndex))
				continue;
		}
		else if (toAdd == "k") // Add/change key to room
		{
			// If returns false, the command have a error, then proced to next command
			if (!processCommandK(client, channel, toAdd, mode, paramIndex))
				continue;
		}
		else if (toAdd == "o") // Set operator
		{
			if (!processCommandO(client, channel, toAdd, mode, paramIndex))
				continue;
		}
		else if (toAdd == "v") // Set voice
		{
			if (!processCommandV(client, channel, toAdd, mode, paramIndex))
				continue;
		}
		else
		{
			// If request mode are aditive
			if (mode == "+")
			{
				if (!channel->channelHasMode(toAdd))
				{
					channel->addChannelMode(toAdd);
					channel->broadcastWithMe(_MODESET(channel->getChannelName(), ("+" + toAdd)));
				}
			}
			else
			{
				if (channel->channelHasMode(toAdd))
				{
					channel->removeChannelMode(toAdd);
					channel->broadcastWithMe(_MODESET(channel->getChannelName(), ("-" + toAdd)));
				}
			}
		}
	}
}

bool Mode::processCommandO(Client *client, Channel *channel, std::string toAdd, std::string mode, std::size_t &paramIndex)
{
	// If no max user param given
	if (_args.size() < paramIndex)
		return false;
	std::map<int, Client *>::iterator it_target = channel->getClientList().begin();
	while (it_target != channel->getClientList().end())
	{
		if (it_target->second->getNickname() == _args[paramIndex])
			break; // Found user
		it_target++;
	}

	// If user not found in channel
	if (it_target == channel->getClientList().end())
	{
		client->sendToClient(_NONICKORCHANNEL(client->getNickname(), _args[paramIndex]));
		paramIndex++;
		return false;
	}
	if (mode == "+")
	{
		channel->addGroupOperatorElement(it_target->second->getFd(), it_target->second);
		//channel->deleteClientElement(it_target->second->getFd());
		// send to emisor
		client->sendToClient(_MODESETWITHPARAM(channel->getChannelName(), "+" + toAdd, it_target->second->getNickname()));
		// send to target
		if (it_target->second->getNickname() != client->getNickname())
			it_target->second->sendToClient(_MODESETWITHPARAM(channel->getChannelName(), "+" + toAdd, it_target->second->getNickname()));
	}
	else
	{
		channel->deleteGroupOperatorElement(it_target->second->getFd());
		//channel->addClientElement(it_target->second->getFd(), it_target->second);
		client->setMemberLevel(REGISTERED);
		// send to emisor
		client->sendToClient(_MODESETWITHPARAM(channel->getChannelName(), "-" + toAdd, it_target->second->getNickname()));
		// send to target
		if (it_target->second->getNickname() != client->getNickname())
			it_target->second->sendToClient(_MODESETWITHPARAM(channel->getChannelName(), "-" + toAdd, it_target->second->getNickname()));
	}
	paramIndex++;
	return true;
}

bool Mode::processCommandV(Client *client, Channel *channel, std::string toAdd, std::string mode, std::size_t &paramIndex)
{
	// If no max user param given
	if (_args.size() < paramIndex)
		return false;
	std::map<int, Client *>::iterator it_target = channel->getClientList().begin();
	while (it_target != channel->getClientList().end())
	{
		if (it_target->second->getNickname() == _args[paramIndex])
			break; // Found user
		it_target++;
	}

	// If user not found in channel
	if (it_target == channel->getClientList().end())
	{
		client->sendToClient(_NONICKORCHANNEL(client->getNickname(), _args[paramIndex]));
		paramIndex++;
		return false;
	}
	if (mode == "+")
	{
		it_target->second->setMemberLevel(VOICED);
		// send to emisor
		client->sendToClient(_MODESETWITHPARAM(it_target->second->getNickname(), "+" + toAdd, it_target->second->getNickname()));
		// send to target
		if (it_target->second->getNickname() != client->getNickname())
			it_target->second->sendToClient(_MODESETWITHPARAM(channel->getChannelName(), "+" + toAdd, it_target->second->getNickname()));
	}
	else
	{
		it_target->second->setMemberLevel(REGISTERED);
		// send to emisor
		client->sendToClient(_MODESETWITHPARAM(it_target->second->getNickname(), "-" + toAdd, it_target->second->getNickname()));
		// send to target
		if (it_target->second->getNickname() != client->getNickname())
			it_target->second->sendToClient(_MODESETWITHPARAM(channel->getChannelName(), "-" + toAdd, it_target->second->getNickname()));
	}
	paramIndex++;
	return true;
}

bool Mode::processCommandL(Client *client, Channel *channel, std::string toAdd, std::string mode, std::size_t &paramIndex)
{
	if (mode == "+")
	{
		// If no max user param given
		if (_args.size() < paramIndex)
			return false;
		int maxUsersParam = atoi(_args[paramIndex].c_str());
		// If atoi fail or imput is <= 0 (min clients in channel 1)
		if (maxUsersParam <= 0)
		{
			client->sendToClient(_INVALIDMODEPARAM(client->getNickname(), channel->getChannelName(), "+" + toAdd, _args[paramIndex]));
			paramIndex++;
			return false;
		}
		channel->setMaxUsersInChannel(maxUsersParam);
		client->sendToClient(_MODESETWITHPARAM(channel->getChannelName(), "+" + toAdd, _args[paramIndex]));
		paramIndex++;
	}
	else
	{
		channel->setMaxUsersInChannel(MAX_MEMBER);
		channel->broadcastWithMe(_MODESET(channel->getChannelName(), ("-" + toAdd)));
	}
	return true;
}

bool Mode::processCommandK(Client *client, Channel *channel, std::string toAdd, std::string mode, std::size_t &paramIndex)
{
	if (mode == "+")
	{
		// If no password user param given
		if (_args.size() < paramIndex)
			return false;

		std::string password = _args[paramIndex];
		// password checks
		if (!printable(password) || password.length() > 50)
		{
			client->sendToClient(_INVALIDMODEPARAM(client->getNickname(), channel->getChannelName(), "+" + toAdd, _args[paramIndex]));
			paramIndex++;
			return false;
		}
		channel->setChannelPassword(password);
		client->sendToClient(_MODESETWITHPARAM(channel->getChannelName(), "+" + toAdd, _args[paramIndex]));
		paramIndex++;
	}
	else
	{
		channel->setChannelPassword("");
		channel->broadcastWithMe(_MODESET(channel->getChannelName(), ("-" + toAdd)));
	}
	return true;
}