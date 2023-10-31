#include "Ping.hpp"
#include "Replies.hpp"
#include "IO.hpp"

Ping::Ping(Server *serv) : ACommand(serv)
{
}

Ping::~Ping()
{
}

void    Ping::exec(Client *client)
{
    if (client->getDebug())
        printDebug("Ping Command Found, Executing Ping Command");

    try
    {
        isValidFormat();
		sendPong(client);

        if (client->getDebug())
            printDebug("Ping Command Passed");
    }
    catch(int numeric)
    {
        std::stringstream sstm;
        sstm << numeric << " " << client->getSockFd();
        std::string msgBuf = sstm.str();
        switch (numeric)
        {
        case ERR_UNKNOWNERROR:
            msgBuf += " PING :Invalid Format error !";
            break;

        default:
            break;
        }
    }
    _command.clear();
    _args.clear();
}

void    Ping::isValidFormat(void)
{
    // PING <time>
    if (_args.size() != 2)
        throw ERR_UNKNOWNERROR;
}

void	Ping::sendPong(Client *client)
{
	client->sendToClient(_PONG(client->getNickname(), client->getRealname(), client->getHostName(), _args[1]));
}