#include "List.hpp"
#include "Replies.hpp"
#include "IO.hpp"

List::List(Server *serv) : ACommand(serv)
{
}

List::~List()
{
}

void List::exec(Client *client)
{
	if (client->getDebug())
		printDebug("List Command Found, Executing List Command");

	try
	{
		isValidFormat();
		checkClientLevel(client);
		listCmd(client);
		if (client->getDebug())
			printDebug("List Command Passed");
	}
	catch (int numeric)
	{
		std::stringstream sstm;
		sstm << numeric << " " << client->getSockFd();
		std::string msgBuf = sstm.str();
		switch (numeric)
		{
		case ERR_UNKNOWNERROR:
			msgBuf += " List :Invalid Format error !";
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

void List::isValidFormat(void)
{
	// WHO [<channel> [<o>]]

	// Right now we only support WHO <channel>
	// In the future WHO will support WHO <mask> and WHO <mask> o

	// Check if there are 2 arguments
	// if (_args.size() > 2)
	//     throw ERR_UNKNOWNERROR;
	// if (_args.size() < 2)
	//     throw ERR_NEEDMOREPARAMS;
	// // Check if the first argument is a channel name
	// if (_args[1][0] != '#')
	//     throw ERR_UNKNOWNERROR;
}

void List::checkClientLevel(Client *client)
{
	if (!(client->getMemberLevel() & REGISTERED))
		throw ERR_NOTREGISTERED;
}

void List::listCmd(Client *client)
{

	std::string to_channel;
	std::string response;

	// Check if have channel parameter
	if (_args.size() > 1)
		to_channel = (_args[1].find("#") == std::string::npos) ? "" : _args[1];

	// If is for all channels "/LIST"
	if (to_channel.empty())
	{
		// If server dont have channels
		if (_server->getChannelList().empty())
		{
			client->sendToClient(_EOFLIST(client->getNickname()));
		}
		else
		{
			// Send list of channels
			client->sendToClient(_INITLIST(client->getNickname()));
			std::map<std::string, Channel *>::iterator it = _server->getChannelList().begin();
			while (it != _server->getChannelList().end())
			{
				response.clear();
				// check mode
				client->sendToClient(getResponse(client->getNickname(), it));
				it++;
			}
			client->sendToClient(_EOFLIST(client->getNickname()));
		}
	}
}

std::string List::getResponse(std::string nick, std::map<std::string, Channel *>::iterator &channel_it)
{
	std::size_t clientCount = channel_it->second->getClientList().size();
	std::ostringstream oss;
	oss << clientCount;
	std::string sizeClients = oss.str();

	// check mode: if channel is private and user are not in
	if (channel_it->second->channelHasMode("p") && channel_it->second->doesClientExist(nick) == false)
		return _PRIVATETOPIC(nick, channel_it->second->getChannelName(), sizeClients);

	return _LIST(nick, channel_it->second->getChannelName(), sizeClients, channel_it->second->getChannelTopic());
}