#include "Mode.hpp"
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
    if (_args.size() > 2)
        throw ERR_UNKNOWNERROR;
    if (_args.size() < 2)
        throw ERR_NEEDMOREPARAMS;
    // Check if the first argument is a channel name
    if (_args[1][0] != '#')
        throw ERR_UNKNOWNERROR;

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
    //if (_server->getChannelList().find(_args[1]) == _server->getChannelList().end())
    //    throw ERR_NOSUCHCHANNEL;
	
	std::map<std::string, Channel *> channels = _server->getChannelList();
	std::map<std::string, Channel *>::iterator it_channel = channels.find(_args[1]);
	// Find target
	if (getTargetType() == CHANNEL)
	{
		if (it_channel == channels.end())
		{
			//SEND CHANNEL NOT EXIST
			return;
		}
		setChannelMode(client, it_channel->second);
		return;
	}
	// Set client mode
    setClientMode(client, it_channel->second);
}

void Mode::setChannelMode(Client *client, Channel *channel)
{
	(void)client;
	(void)channel;
	//Check if modes are given
	if (_args.size() > 3)
	{
		return;
	}
	//Check if the client is operator of channel
	

}

void Mode::setClientMode(Client *client, Channel *channel)
{
	(void)channel;
	std::string msgBuf = ":IRC 324 " + client->getNickname() + " " + _args[1] + " +nt";
    client->sendToClient(msgBuf);
}

Mode::typeSend Mode::getTargetType()
{
	// Mode to channel MODE <#msgtarget> <modes>
	// Mode to user MODE <msgtarget> <modes>
	return _args[1][0] == '#' ? CHANNEL : USER;
}

void Mode::parseModes()
{
	/* CURRENTLY TESTING THIS FUNCTION */
    std::string modes;
    std::vector<std::string> options;

    for (size_t i = 0; i < _args.size(); ++i) {
        if (_args[i].length() > 1 && (_args[i][0] == '+' || _args[i][0] == '-')) {
            // Argument start with + or -
            for (size_t j = 1; j < _args[i].length(); ++j) {
                //modes += _args[i][0]; // Add el + o - to mode
                modes += _args[i][j]; // Add mode
            }
            if (i + 1 < _args.size() && _args[i + 1][0] != '+' && _args[i + 1][0] != '-') {
                // If there is an argument after the mode, it is considered as an associated option
                options.push_back(_args[i + 1]);
            }
        }
    }

    std::cout << "Modes: " << modes << std::endl;
    std::cout << "Options: ";
    for (std::vector<std::string>::iterator it = options.begin(); it != options.end(); ++it)
	{
        std::cout << *it << std::endl;
    }
    std::cout << std::endl;
}