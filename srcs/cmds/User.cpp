#include "User.hpp"
#include "Operation.hpp"

User::User(Server* serv) : ACommand(serv)
{
}

User::~User()
{
}

void User::exec(Client *client)
{
    std::cout << "User EXECUTE" << std::endl;
    try
    {
        isValidFormat();
        checkClientLevel(client);
        validCheck();
        setClientUser(client);
        if ((client->getMemberLevel() & REGISTERED) == REGISTERED)
            welcome2CanServ(client);
        else
        {
            std::string msgBuf = "001: Welcome " + client->getUsername() + "! Please set your nickname to finalize registration." + "\r\n";
            client->sendToClient(msgBuf);
        }
    }
    catch (int numeric)
    {
        std::stringstream sstm;
        sstm << numeric << " " << client->getSockFd();
        std::string msgBuf = sstm.str();
        switch (numeric)
        {
        case ERR_UNKNOWNERROR:
            msgBuf += " USER :Invalid Format error !";
            break;
        case ERR_NOTREGISTERED:
            msgBuf += " :You have not registered. Register PASS, USER, NICK !";
            break;

        case ERR_NEEDMOREPARAMS:
            msgBuf += " USER :Invalid Format error !";
            break;

        case ERR_ALREADYREGISTERED:
            msgBuf += " :You may not reregister, you have already registered !";
            break;

        default:
            break;
        }

        msgBuf += "\r\n";
        client->sendToClient(msgBuf);
    }
}

int User::validCheck(void)
{
    // flag USER <username> 0 * <realname>
    //  0   1     2         3 4  5

    if (_args[2].length() < 1 || _args[5].length() < 1)
    {
        throw(ERR_NEEDMOREPARAMS);
    }
    return (TRUE);
}

void User::setClientUser(Client* client)
{
    // flag USER <username> 0 * <realname>
    //  0   1     2         3 4  5

    client->setUsername(_args[2]);
    client->setRealname(_args[5]);
    client->setMemberLevel(USER_SET);
    if ((client->getMemberLevel() & (PASS_SET | NICK_SET)) == (PASS_SET | NICK_SET))
        client->setMemberLevel(REGISTERED);
}

void User::welcome2CanServ(Client* client)
{
    // 001 <client> :<msg>
    std::string userName = _args[2];
    std::string serverName = static_cast<std::string>(SERVERNAME);
    std::string msgBuf = "001 " + userName + " :Welcome, " + userName + "! Your host is " + serverName + "\r\n";
    client->sendToClient(msgBuf);
}

int User::isValidFormat(void)
{
    // flag USER <username> 0 * <realname>
    if (_args.size() != 6)
        throw ERR_UNKNOWNERROR;
    if (determineFlag() == 0)
        throw ERR_UNKNOWNERROR;
    return (TRUE);
}

int User::checkClientLevel(Client* client)
{
    if ((client->getMemberLevel() & PASS_SET) != PASS_SET)
    {
        throw ERR_NOTREGISTERED;
    }
    else if ((client->getMemberLevel() & USER_SET) == USER_SET)
    {
        throw(ERR_ALREADYREGISTERED);
    }
    return (TRUE);
}

int User::determineFlag(void)
{
    if (_args[0] == "1")
        return (1);
    return (0);
}
