#ifndef BOT_HPP
#define BOT_HPP

#include "Channel.hpp"
#include <iostream>
#include <string>

class Bot
{
  private:
    std::string word;
    std::string botMessage;
    Bot(const Bot& obj);            // No use.
    Bot& operator=(const Bot& obj); // No use.
  public:
    Bot();
    ~Bot();
    int  findWord(std::string message);
    void sendMessage(Channel* channel);
    void executeBot(std::string message, Client* dstClient, Client* sndClient);
    void executeBot(std::string message, Channel* channel);
};

#endif