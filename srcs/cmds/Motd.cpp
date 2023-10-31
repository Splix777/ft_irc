#include "Motd.hpp"
#include "Replies.hpp"
#include "IO.hpp"

Motd::Motd(Server* serv) : ACommand(serv)
{
}

Motd::~Motd()
{
}

void Motd::exec(Client *client)
{
	if (client->getDebug())
        printDebug("Motd Command Found, Executing Nick Command");
    try
    {
        isValidFormat();
		cmdMotd(client);
        if (client->getDebug())
            printDebug("Motd Command Passed");
    }
    catch (int numeric)
    {
        std::stringstream sstm;
        sstm << numeric << " " << client->getSockFd();
        std::string msgBuf = sstm.str();
        switch (numeric)
        {
        case ERR_UNKNOWNERROR:
            msgBuf += " MOTD :Invalid Format error !";
            break;
        default:
            break;
        }
        client->sendToClient(msgBuf);
    }
    _command.clear();
    _args.clear();
}

void 	Motd::isValidFormat(void)
{
	// MOTD [<target>]
	if(_args.size() > 2)
		throw ERR_UNKNOWNERROR;
}

void	Motd::cmdMotd(Client *client)
{
	// Init message of the day
	client->sendToClient(_MOTDSTART(client->getNickname(), "IRC"));
	// Indiscutible perfect ascii art
	client->sendToClient(_MOTD(client->getNickname(), "    ______ _     _____          "));
	client->sendToClient(_MOTD(client->getNickname(), "   |  ____| |   |_   _|         "));
	client->sendToClient(_MOTD(client->getNickname(), "   | |__  | |_    | |  _ __ ___ "));
	client->sendToClient(_MOTD(client->getNickname(), "   |  __| | __|   | | | '__/ __|"));
	client->sendToClient(_MOTD(client->getNickname(), "   | |    | |_   _| |_| | | (__ "));
	client->sendToClient(_MOTD(client->getNickname(), "   |_|     \\__| |_____|_|  \\___|"));
	client->sendToClient(_MOTD(client->getNickname(), "            ______              "));
	client->sendToClient(_MOTD(client->getNickname(), "           |______|             "));
	
	// End of message of the day
	client->sendToClient(_ENDMOTD(client->getNickname()));       
}