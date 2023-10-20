#include "Bot.hpp"
#include "Operation.hpp"

Bot::Bot() : word("42Seoul"), botMessage("[🤖Bot🤖] Thanks for visiting CanServ! weLove42Seoul!\n")
{
}

Bot::~Bot()
{
}

int Bot::findWord(std::string message)
{
    if (message.find(this->word) != std::string::npos)
        return (TRUE);
    return (FALSE);
}

void Bot::executeBot(std::string message, Channel* channel)
{
    if (findWord(message) == TRUE)
    {
        std::map<int, Client*>::iterator it;

        std::string msg = ":BOT PRIVMSG " + channel->getChannelName() + " :" + botMessage + "\r\n";
        for (it = channel->getClientList().begin(); it != channel->getClientList().end(); it++)
        {
            it->second->addSendBuff(msg);
        }
    }
}

void Bot::executeBot(std::string message, Client* dstClient, Client* sndClient)
{
    if (findWord(message) == TRUE)
    {
        std::map<int, Client*>::iterator it;

        std::string msg =
            ":" + sndClient->getNickname() + " PRIVMSG " + dstClient->getNickname() + " :" + botMessage + "\r\n";
        dstClient->addSendBuff(msg);
        sndClient->addSendBuff(msg);
    }
}
