#include "Part.hpp"
#include "Replies.hpp"
#include "IO.hpp"

Part::Part(Server *serv) : ACommand(serv)
{
}

Part::~Part()
{
}

void Part::exec(Client *client)
{
	if (client->getDebug())
		printDebug("Part Command Found, Executing Part Command");
	try
	{
		splitArgs();
		sendPart(client);
	}
	catch (int numeric)
	{
		std::string msgBuf;
		switch (numeric)
		{
		case ERR_NEEDMOREPARAMS:
		    msgBuf = _NEEDMOREPARAMS(client->getHostName(), client->getNickname(), _args[0]);
		    break;
		case ERR_NOTREGISTERED:
			msgBuf = _NOTREGISTERED(client->getHostName(), client->getNickname());
		    break;

		default:
		    break;
		}
		client->sendToClient(msgBuf);
	}
	channelNames.clear();
	_command.clear();
	_args.clear();
}

void Part::splitArgs()
{
	std::string buff;

	buff = _args[1];
	while (buff.find(",") != std::string::npos)
	{
		std::string word = buff.substr(0, buff.find(","));
		channelNames.push_back(word);
		buff.erase(0, buff.find(",") + 1);
	}
	channelNames.push_back(buff);
}

void Part::sendPart(Client *client)
{
	std::string nick = client->getNickname();
	std::string channelName;
	std::string reason = (_args.size() == 3) ? _args[2] : "";

	std::map<std::string, Channel *> channels = (*_server).getChannelList();

	for (std::size_t i = 0; i < channelNames.size(); i++)
	{
		channelName.clear();
		channelName = channelNames[i];
		std::map<std::string, Channel *>::iterator it = channels.find(channelName);

		// If channel dows not exist
		if (it == channels.end())
		{
			client->sendToClient(_CHANNELNOTEXIST(client->getHostName(), client->getNickname(), channelName));
			continue;
		}

		// If user not in channel
		if (it != channels.end() && !it->second->doesClientExist(nick))
		{
			client->sendToClient(_NOTONCHANNEL(client->getHostName(), client->getNickname(), channelName));
			continue;
		}
		
		// If channel exist and user are in
		std::cout << _PART(_user(nick, client->getUsername(), client->getHostName()), channelName, reason) << std::endl;
		it->second->deleteClientElement(client->getFd());
		client->sendToClient(_PART(_user(nick, client->getUsername(), client->getHostName()), channelName, reason));
		// notify to all in chat when part
		it->second->broadcast(_PART(_user(nick, client->getUsername(), client->getHostName()), channelName, reason), client);
	}
}

void Part::isValidFormat()
{
	// PART <partTarget> {optional:<text>}
	// 1        2                3
	if (_args.size() < 2)
		throw ERR_NEEDMOREPARAMS;
	if (_args.size() > 3)
		throw ERR_NEEDMOREPARAMS;
}
