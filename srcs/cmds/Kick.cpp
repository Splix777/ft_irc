#include "Kick.hpp"
#include "Replies.hpp"
#include "IO.hpp"

Kick::Kick(Server* serv) : ACommand(serv)
{
}

Kick::~Kick()
{
}

void Kick::exec(Client *client)
{
	if (client->getDebug())
        printDebug("Kick Command Found, Executing Nick Command");
    try
    {
        isValidFormat();
		cmdKick(client);
        if (client->getDebug())
            printDebug("Kick Command Passed");
    }
    catch (int numeric)
    {
        std::stringstream sstm;
        sstm << numeric << " " << client->getSockFd();
        std::string msgBuf = sstm.str();
        switch (numeric)
        {
        case ERR_NEEDMOREPARAMS:
            msgBuf += " Kick :Not enough parameters.";
            break;
        default:
            break;
        }
        client->sendToClient(msgBuf);
    }
    _command.clear();
    _args.clear();
}

void Kick::isValidFormat(void)
{
	// KICK <channel> <user> [<:reasson>]
	if(_args.size() < 3 || _args.size() > 4)
		throw ERR_NEEDMOREPARAMS;
}

void Kick::cmdKick(Client *client)
{
	// Check client is operator
	std::string reason = (_args.size() == 4) ? _args[3] : "";

	std::map<int, Client *> client_list = _server->getClientList();
	std::map<std::string, Channel *> channel_list = _server->getChannelList();
	std::map<std::string, Channel *>::iterator it_channel = channel_list.find(_args[1]);
	// Check client is operator
	if(it_channel != channel_list.end() && !it_channel->second->doesOperatorExist(client->getNickname()))
	{
		client->sendToClient(_CHANOPRIVSNEEDED(client->getNickname(), _args[1]));
		return;
	}
	// If channel not exist
	if(it_channel == channel_list.end())
	{
		client->sendToClient(_CHCHANNELNOTEXIST(client->getNickname(), _args[1]));
		return;
	}
	//if user not in channel
	if(!it_channel->second->doesClientExist(_args[2]))
	{
		client->sendToClient(_USERNOTINCHANNEL(client->getNickname(), _args[2], _args[1]));
		return;
	}
	std::map<int, Client *>::iterator it_target = client_list.begin();
	while (it_target != client_list.end())
	{
		if (it_target->second->getNickname() == _args[2])
			break;
		it_target++;
	}
	if(it_target == client_list.end())
		return;
	
	it_channel->second->broadcastWithMe(_KICK(_user(client->getNickname(), client->getUsername(), client->getHostName()), _args[1], _args[2], reason));
	it_channel->second->deleteClientElement(it_target->second->getFd());
}