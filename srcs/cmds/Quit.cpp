#include "Quit.hpp"
#include "IO.hpp"

Quit::Quit(Server *serv) : ACommand(serv)
{
}

Quit::~Quit()
{
}

void Quit::exec(Client *client)
{
    if (client->getDebug())
        printDebug("Quit Command Found, Executing Quit Command");

    try
    {
        isValidFormat();
        checkClientLevel(client);
        quitCmd(client);
        if (client->getDebug())
            printDebug("Quit Command Passed");
    }
    catch (int numeric)
    {
        std::stringstream sstm;
        sstm << numeric << " " << client->getSockFd();
        std::string msgBuf = sstm.str();
        switch (numeric)
        {
        case ERR_UNKNOWNERROR:
            msgBuf += " QUIT :Invalid Format error !";
            break;
        
        default:
            break;

        }
        client->sendToClient(msgBuf);
    }
    _command.clear();
    _args.clear();
}

void Quit::isValidFormat(void)
{
    // QUIT [<Quit message>]
    if (_args.size() > 2)
        throw ERR_UNKNOWNERROR;
}

void Quit::checkClientLevel(Client *client)
{
    if (!(client->getMemberLevel() & REGISTERED))
        throw ERR_NOTREGISTERED;
}

void Quit::quitCmd(Client *client)
{
    std::string msgBuf = "ERROR :Closing Link: ";
    msgBuf += client->getNickname();
    msgBuf += " (";
    msgBuf += _args[1];
    msgBuf += ")";
    client->sendToClient(msgBuf);
}