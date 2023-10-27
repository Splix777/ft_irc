#include "Cap.hpp"
#include "IO.hpp"

Cap::Cap(Server *serv) : ACommand(serv)
{
}

Cap::~Cap()
{
}

void    Cap::exec(Client *client)
{
    if (client->getDebug())
        printDebug("Cap Command Found, Executing Cap Command");

    try
    {
        isValidFormat();
        checkClientLevel(client);
        serverCapabilities(client);
        if (client->getDebug())
            printDebug("Cap Command Passed");
    }
    catch(int numeric)
    {
        std::stringstream sstm;
        sstm << numeric << " " << client->getSockFd();
        std::string msgBuf = sstm.str();
        switch (numeric)
        {
        case ERR_UNKNOWNERROR:
            msgBuf += " CAP :Invalid Format error !";
            break;

        default:
            break;
        }
    }
    _command.clear();
    _args.clear();
}

void    Cap::isValidFormat(void)
{
    // CAP <subcommand> [<capability> [<parameters>]]
    if (_args.size() != 3)
        throw ERR_UNKNOWNERROR;
}

void    Cap::checkClientLevel(Client *client)
{
    if (client->getMemberLevel() < UNREGISTERED)
        throw(ERR_NOPRIVILEGES);
}

void    Cap::serverCapabilities(Client *client)
{
    // CAP LS 302
    // CAP REQ :sasl
    // CAP END
    std::string subCommand = _args[1];
    std::string capability = _args[2];
    if (subCommand == "LS")
    {
        if (capability == "302")
        {
            std::string msgBuf = ":IRC CAP * LS :multi-prefix sasl";
            client->sendToClient(msgBuf);
        }
    }
    else if (subCommand == "REQ")
    {
        if (capability == "sasl")
        {
            std::string msgBuf = ":IRC CAP * ACK :sasl";
            client->sendToClient(msgBuf);
        }
    }
    else if (subCommand == "END")
    {
        std::string msgBuf = ":IRC CAP * ACK :sasl";
        client->sendToClient(msgBuf);
    }
    else
        throw ERR_UNKNOWNERROR;
}