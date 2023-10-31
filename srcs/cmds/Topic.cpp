#include "Topic.hpp"
#include "Replies.hpp"
#include "IO.hpp"

Topic::Topic(Server *serv) : ACommand(serv)
{
}

Topic::~Topic()
{
}

void    Topic::exec(Client *client)
{
    if (client->getDebug())
        printDebug("Topic Command Found, Executing Topic Command");

    try
    {
        isValidFormat();
        cmdTopic(client);
        if (client->getDebug())
            printDebug("Topic Command Passed");
    }
    catch(int numeric)
    {
        std::stringstream sstm;
        sstm << numeric << " " << client->getSockFd();
        std::string msgBuf = sstm.str();
        switch (numeric)
        {
        case ERR_UNKNOWNERROR:
            msgBuf += " Topic :Invalid Format error !";
            break;

        default:
            break;
        }
		client->sendToClient(msgBuf);
    }
    _command.clear();
    _args.clear();
}

void    Topic::isValidFormat(void)
{
    // Topic <channel> [<topic>]
    if (_args.size() < 2)
        throw ERR_UNKNOWNERROR;
}

void    Topic::cmdTopic(Client *client)
{
	std::map<int, Client *> client_list = _server->getClientList();
	std::map<std::string, Channel *> channel_list = _server->getChannelList();
	std::map<std::string, Channel *>::iterator it_channel = channel_list.find(_args[1]);

	if(it_channel == channel_list.end())
	{
		client->sendToClient(_CHCHANNELNOTEXIST(client->getNickname(), _args[1]));
		return;
	}
	if(!it_channel->second->doesClientExist(client->getNickname()))
	{
		std::cout << "client not exists " << std::endl;
		client->sendToClient(_NOTONCHANNEL(client->getHostName(), client->getNickname(), _args[1]));
		return;
	}

	if (_args.size() == 2) //topic is empty
	{
		if (it_channel->second->getChannelTopic().empty() == false)
			client->sendToClient(_TOPIC(client->getNickname(), _args[1], it_channel->second->getChannelTopic()));
		else
			client->sendToClient(_NOTOPIC(client->getNickname(), _args[1]));
		return;
	}
	std::string topic;
	for (std::size_t i = 2; i < _args.size(); i++)
		topic += " " + _args[i];
	if (it_channel->second->channelHasMode("t") && !it_channel->second->doesOperatorExist(client->getNickname()))
			client->sendToClient(_CHANOPRIVSNEEDED(client->getNickname(), _args[1]));
	else
	{
		std::cout << topic << std::endl;
		it_channel->second->setChannelTopic(topic);
		it_channel->second->broadcastWithMe(_TOPIC(client->getNickname(), _args[1], it_channel->second->getChannelTopic()));
	}
}