#include "Join.hpp"
#include "Replies.hpp"
#include "IO.hpp"

Join::Join(Server *serv) : ACommand(serv)
{
}

Join::~Join()
{
}

void    Join::exec(Client *client)
{
    if (client->getDebug())
        printDebug("Join Command Found, Executing Join Command");
    try
    {
        validCheck(client);
        joinChannels(client);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void    Join::validCheck(Client *client)
{
    try
    {
        isValidFormat();
        checkClientLevel(client);
        splitArgs();
        if (client->getDebug())
        {
            for(size_t i = 0; i < channelNames.size(); i++)
                printDebug("Channel Name: " + channelNames[i]);
            for(size_t i = 0; i < channelPassword.size(); i++)
                printDebug("Channel Password: " + channelPassword[i]);
        }
    }
    catch(int numeric)
    {
        std::stringstream sstm;
        sstm << numeric << " " << client->getSockFd();
        std::string msgBuf = sstm.str();
        switch (numeric)
        {
        case ERR_NEEDMOREPARAMS:
            msgBuf += " JOIN :Not enough parameters";
            break;

        case ERR_NOTREGISTERED:
            msgBuf += " :You have not registered";
            break;

        default:
            break;
        }
        client->sendToClient(msgBuf);
        channelNames.clear();
        channelPassword.clear();
        _command.clear();
        _args.clear();
    }
}

void    Join::isValidFormat()
{
    // JOIN <channel>{,<channel>} [<key>{,<key>}]
    // 1    2                     3 
    if (_args.size() != 3 && _args.size() != 2)
        throw ERR_NEEDMOREPARAMS;
}   

void    Join::checkClientLevel(Client *client)
{
    if ((client->getMemberLevel() & REGISTERED) != REGISTERED)
        throw ERR_NOTREGISTERED;
}

void    Join::splitArgs()
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

    if (_args.size() == 3)
    {
        buff = _args[2];
        while (buff.find(",") != std::string::npos)
        {
            std::string word = buff.substr(0, buff.find(","));
            channelPassword.push_back(word);
            buff.erase(0, buff.find(",") + 1);
        }
        channelPassword.push_back(buff);
    }

    if (channelNames.size() < channelPassword.size())
        throw ERR_NEEDMOREPARAMS;

    while (channelNames.size() > channelPassword.size())
        channelPassword.push_back("");
}

void    Join::joinChannels(Client *client)
{
    for (size_t i = 0; i < channelNames.size(); i++)
    {
        try
        {
            parseName(channelNames[i]);
            parsePassword(channelPassword[i]);
            if (channelExists(channelNames[i]) == false)
            {
                if (client->getDebug())
                    printDebug("Channel " + channelNames[i] + " does not exist so creating it");
                createChannel(channelNames[i], channelPassword[i], client);
            }
            addClientToChannel(channelNames[i], channelPassword[i], client);
            welcome(client, channelNames[i]);
            if (client->getDebug())
                printDebug("Client " + toString(client->getSockFd()) + " joined channel " + channelNames[i]);
        }
        catch(int numeric)
        {
            std::stringstream sstm;
            sstm << numeric << " " << client->getSockFd();
            std::string msgBuf = sstm.str();
            switch (numeric)
            {
            case ERR_NOSUCHCHANNEL:
                msgBuf += " " + channelNames[i] + " :No such channel";
                break;
            
            case ERR_TOOMANYCHANNELS:
                msgBuf += " " + channelNames[i] + " :You have joined too many channels";
                break;

            case ERR_CHANNELISFULL:
                msgBuf += " " + channelNames[i] + " :Cannot join channel (+l)";
                break;

            case ERR_CHANNELPASSWDMISMATCH:
                msgBuf += " " + channelNames[i] + " :Cannot join channel (+k)";
                break;

            case ERR_INVALIDPASSWORD:
                msgBuf += " " + channelNames[i] + " :Invalid password";
                break;

            default:
                break;
            }
            client->sendToClient(msgBuf);
        }
    }
    channelNames.clear();
    channelPassword.clear();
    _command.clear();
    _args.clear();
}

void    Join::parseName(std::string const &name)
{
    if (!printable(name))
        throw ERR_NOSUCHCHANNEL;
    if (name.length() > 50)
        throw ERR_NOSUCHCHANNEL;
    if (name[0] != '#' && name[0] != '&')
        throw ERR_NOSUCHCHANNEL;
}

void    Join::parsePassword(std::string const &password)
{
    if (!printable(password))
        throw ERR_INVALIDPASSWORD;
    if (password.length() > 50)
        throw ERR_INVALIDPASSWORD;
}

bool    Join::channelExists(std::string const &name)
{
    if (_server->getChannelList().find(name) == _server->getChannelList().end())
        return (false);
    return (true);
}

void    Join::createChannel(std::string const &name, std::string const &password, Client *client)
{
    Channel *newChannel = new Channel(name, password, MAX_MEMBER);
    if (newChannel)
    {
        _server->addChannelElement(name, newChannel);
        (void) client;
        // client->setMemberLevel(client->getMemberLevel() | OPERATOR);
    }
}

void    Join::addClientToChannel(std::string const &name, std::string const &password, Client *client)
{
    Channel *temp = _server->getChannelList().find(name)->second;

    if (temp->getClientList().size() >= MAX_MEMBER || (int)temp->getClientList().size() >= temp->getMaxUsersInChannel())
        throw ERR_CHANNELISFULL;

    if (temp->getChannelPassword() != password)
        throw ERR_CHANNELPASSWDMISMATCH;
        
    if ((client->getMemberLevel() & OPERATOR) || temp->getGroupOperatorList().empty())
        temp->addGroupOperatorElement(client->getSockFd(), client);
    else
        temp->addClientElement(client->getSockFd(), client);
    client->addChannelElement(name, temp);
}

void    Join::welcome(Client *client, std::string const &channelName)
{
    //  :<client> <command> :<channel_name>
    client->sendToClient(_JOIN(client->getNickname(), channelName));

    // Send RPL_NAMEREPLY followed by RPL_ENDOFNAMES
    // :<server> 353 <client> = <channel_name> :<nicknames>
    // :<server> 366 <client> <channel_name> :End of /NAMES list
    std::string msgBuf;
    Channel *temp = _server->getChannelList().find(channelName)->second;
    std::map<int, Client *> clientList = temp->getClientList();
    std::map<int, Client *> groupOperatorList = temp->getGroupOperatorList();
    for (std::map<int, Client *>::iterator it = groupOperatorList.begin(); it != groupOperatorList.end(); it++)
        msgBuf += "@" + it->second->getNickname() + " ";
    for (std::map<int, Client *>::iterator it = clientList.begin(); it != clientList.end(); it++)
        msgBuf += it->second->getNickname() + " ";
    // Send to all clients in channel
    for (std::map<int, Client *>::iterator it = groupOperatorList.begin(); it != groupOperatorList.end(); it++)
    {
        // it->second->sendToClient(msgBuf);
        // client->sendToClient(":IRC 366 " + it->second->getNickname() + " " + channelName + " :End of /NAMES list");
		it->second->sendToClient(_NAMES(client->getNickname(), channelName, msgBuf));
		client->sendToClient(_EOFNAMES(it->second->getNickname(), channelName));
	}
    for (std::map<int, Client *>::iterator it = clientList.begin(); it != clientList.end(); it++)
    {
        // it->second->sendToClient(msgBuf);
        // client->sendToClient(":IRC 366 " + it->second->getNickname() + " " + channelName + " :End of /NAMES list");
		it->second->sendToClient(_NAMES(client->getNickname(), channelName, msgBuf));
		client->sendToClient(_EOFNAMES(it->second->getNickname(), channelName));
    }
}