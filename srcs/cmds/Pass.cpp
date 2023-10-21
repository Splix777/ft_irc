#include "Pass.hpp"
#include "Operation.hpp"

Pass::Pass(Server *serv) : ACommand(serv)
{
}

Pass::~Pass()
{
}

void Pass::exec(Client *client)
{
    std::cout << "Pass EXECUTE" << std::endl;
    try
    {
        isValidFormat();
        checkClientLevel(client);
        passCmp(client);
        client->sendToClient("001 " + client->getNickname() + " :Welcome to the Internet Relay Network " + client->getNickname() + "\r\n");
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

        msgBuf += "\r\n";
        client->sendToClient(msgBuf);
    }
    _command = "";
    _args.clear();
}

void Pass::passCmp(Client* client)
{
    // flag PASS <password>
    std::string serverPW = _server->getPassword();
    std::string clientPW = _args[2];
    if (serverPW == clientPW)
        client->setMemberLevel(PASS_SET);
    else
        throw(ERR_PASSWDMISMATCH);
}

void Pass::isValidFormat(void)
{
    if (_args.size() != 3)
        throw ERR_UNKNOWNERROR;
}

void Pass::checkClientLevel(Client* client)
{
    if (client->getMemberLevel() & REGISTERED)
        throw(ERR_ALREADYREGISTERED);
}

void Pass::determineFlag(void)
{
}
