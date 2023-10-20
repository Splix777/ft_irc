#ifndef COMMAND_PRVMSG_HPP
#define COMMAND_PRVMSG_HPP

#include "ACommand.hpp"
#include "Bot.hpp"
#include "Notice.hpp"

class Prvmsg : public Notice
{
  private:
    void* target;
    Bot*  bot;
    Prvmsg(const Prvmsg& obj);            // No use.
    Prvmsg& operator=(const Prvmsg& obj); // No use.
  public:
    Prvmsg();
    Prvmsg(Server* serv);
    ~Prvmsg();
    void prvMSGOn(Client* client);
    int  isValidFormat(void);
    int  checkClientLevel(Client* client);
    int  determineFlag(void);
    void executePrvmsg(Client* client);

    void* getTarget(Client* client);
};

#endif // COMMAND_PRVMSG_HPP
