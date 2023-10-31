#include "Invite.hpp"
#include "Replies.hpp"
#include "IO.hpp"

Invite::Invite(Server* serv) : ACommand(serv)
{
}

Invite::~Invite()
{
}

void Invite::exec(Client *client)
{
	if (client->getDebug())
        printDebug("Invite Command Found, Executing Nick Command");
    try
    {
        isValidFormat();
		cmdInvite(client);
        if (client->getDebug())
            printDebug("Invite Command Passed");
    }
    catch (int numeric)
    {
        std::stringstream sstm;
        sstm << numeric << " " << client->getSockFd();
        std::string msgBuf = sstm.str();
        switch (numeric)
        {
        case ERR_NEEDMOREPARAMS:
            msgBuf += " Invite :Not enough parameters.";
            break;
        default:
            break;
        }
        client->sendToClient(msgBuf);
    }
    _command.clear();
    _args.clear();
}

void Invite::isValidFormat(void)
{
	// INVITE <nickname> <channel>
	if(_args.size() != 3)
		throw ERR_NEEDMOREPARAMS;
}

void Invite::cmdInvite(Client *client)
{
	//check if target channel exists
	std::map<int, Client *> client_list = _server->getClientList();
	std::map<std::string, Channel *> channel_list = _server->getChannelList();
	std::map<std::string, Channel *>::iterator it_channel = channel_list.find(_args[2]);
	// If channel not exist
	if(it_channel == channel_list.end())
	{
		client->sendToClient(_CHCHANNELNOTEXIST(client->getNickname(), _args[2]));
		return;
	}
	
	// Check if target user exists
	std::map<int, Client *>::iterator it_target = client_list.begin();
	while (it_target != client_list.end())
	{
		if (it_target->second->getNickname() == _args[1])
			break;
		it_target++;
	}
	if(it_target == client_list.end())
	{
		std::cout << "1" << std::endl;
		client->sendToClient(_NOTONCHANNEL(client->getHostName(), client->getNickname(), _args[2]));
		return;
	}
	// Check if sender is in channel
	if (!it_channel->second->doesClientExist(client->getNickname()) && !it_channel->second->doesOperatorExist(client->getNickname()))
	{
		client->sendToClient(_NOTONCHANNEL(client->getHostName(), client->getNickname(), _args[2]));
		return;
	}
	// Check if recipent is in channel
	if (it_channel->second->doesClientExist(_args[1]))
	{
		client->sendToClient(_USERONCHANNEL(client->getNickname(), _args[1], _args[2]));
		return;
	}
	// All ok, send invitation
	// Send response to sender
	client->sendToClient(_INVITING(_user(client->getNickname(), client->getUsername(), client->getHostName()), client->getNickname(), _args[1], _args[2]));
	// Send response to target
	it_target->second->sendToClient(_INVITE(_user(client->getNickname(), client->getUsername(), client->getHostName()), _args[1], _args[2]));
}