#include "Pass.hpp"
#include "IO.hpp"

Pass::Pass(Server *serv) : ACommand(serv)
{
}

Pass::~Pass()
{
}

void Pass::exec(Client *client)
{
    if (client->getDebug())
        printDebug("Pass Command Found, Executing Pass Command");

    try
    {
        isValidFormat();
        checkClientLevel(client);
        passCmp(client);
        if (client->getDebug())
            printDebug("Pass Command Passed");
    }
    catch (int numeric)
    {
        std::stringstream sstm;
        sstm << numeric << " " << client->getSockFd();
        std::string msgBuf = sstm.str();
        switch (numeric)
        {
        case ERR_UNKNOWNERROR:
            msgBuf += " PASS :Invalid Format error !";
            break;

        case ERR_PASSWDMISMATCH:
            msgBuf += " :Password incorrect";
            break;

        case ERR_ALREADYREGISTERED:
            msgBuf += " :Is already registered";
            break;

        default:
            break;
        }
        client->sendToClient(msgBuf);
    }
    _command.clear();
    _args.clear();
}

void Pass::passCmp(Client* client)
{
    // PASS <password>
    std::string serverPW = _server->getPassword();
    std::string clientPW = _args[1];
    if (serverPW == clientPW)
        client->setMemberLevel(PASS_SET);
    else
        throw(ERR_PASSWDMISMATCH);
}

void Pass::isValidFormat(void)
{
    if (_args.size() != 2)
        throw ERR_UNKNOWNERROR;
}

void Pass::checkClientLevel(Client* client)
{
    if (client->getMemberLevel() & REGISTERED)
        throw(ERR_ALREADYREGISTERED);
}

