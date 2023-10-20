#ifndef COMMAND_NICK_HPP
#define COMMAND_NICK_HPP

#include "ACommand.hpp"

#define TRUE 1
#define FALSE 0

class Nick : public ACommand
{
  private:
    int         flag;
    static char invalid[8];
    Nick(const Nick& obj);            // No use.
    Nick& operator=(const Nick& obj); // No use.

  public:
    Nick();
    Nick(Server* serv);
    ~Nick();
    void nickOn(Client* client);
    int  validCheck(void);
    int  checkUsedNick(void);
    void setClientNick(Client* client);

    int isValidFormat(void);
    int checkClientLevel(Client* client);
    int determineFlag(void);

    void welcome2CanServ(Client* client);
};

#endif // COMMAND_NICK_HPP
