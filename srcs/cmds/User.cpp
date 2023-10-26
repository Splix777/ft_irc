#include "User.hpp"
#include "IO.hpp"

User::User(Server *serv) : ACommand(serv)
{
}

User::~User()
{
}

void User::exec(Client *client)
{
    if (client->getDebug())
        printDebug("User Command Found, Executing User Command");
    try
    {
        isValidFormat();
        checkClientLevel(client);
        validCheck();
        setClientUser(client);
        if ((client->getMemberLevel() & REGISTERED) == REGISTERED)
            welcome(client);
        if (client->getDebug())
            printDebug("User Command Passed");
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
        client->sendToClient(msgBuf);
    }
    _command.clear();
    _args.clear();
}

void User::validCheck(void)
{
    // USER <username> 0 * <realname>
    // 0     1         2 3  4

    if (_args[1].length() < 1 || _args[4].length() < 1)
        throw(ERR_NEEDMOREPARAMS);
}

void User::setClientUser(Client* client)
{
    // USER <username> 0 * <realname>
    // 0     1         2 3  4

    client->setUsername(_args[1]);
    client->setRealname(_args[4]);
    client->setMemberLevel(USER_SET);
    if ((client->getMemberLevel() & (PASS_SET | NICK_SET)) == (PASS_SET | NICK_SET))
        client->setMemberLevel(REGISTERED);
}

void User::welcome(Client *client)
{
    // 001 <client> :<msg>

    client->sendToClient(":IRC 001 " + client->getUsername() + " :Welcome, " + client->getUsername() + "! Your host is " + SERVERNAME);
}

void User::isValidFormat(void)
{
    // USER <username> 0 * <realname>
    if (_args.size() != 5)
        throw ERR_UNKNOWNERROR;
    if (_args[2] != "0" || _args[3] != "*")
        throw ERR_UNKNOWNERROR;
    if (_args[4][0] != ':')
        throw ERR_UNKNOWNERROR;
}

void User::checkClientLevel(Client* client)
{
    if ((client->getMemberLevel() & PASS_SET) != PASS_SET)
        throw ERR_NOTREGISTERED;
    else if ((client->getMemberLevel() & USER_SET) == USER_SET || (client->getMemberLevel() & REGISTERED) == REGISTERED)
        throw(ERR_ALREADYREGISTERED);
}

