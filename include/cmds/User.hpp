#ifndef COMMAND_USER_HPP
#define COMMAND_USER_HPP

#include "ACommand.hpp"

#define USERNAME_MAXLEN 30;

#define ERR_NEEDMOREPARAMS 461
#define ERR_ALREADYREGISTERED 462

class User : public ACommand
{
  private:
    User(const User& obj);            // No use.
    User& operator=(const User& obj); // No use.

  public:
    User();
    User(Server* serv);
    ~User();
    void userOn(Client* client);
    int  validCheck(void);
    void setClientUser(Client* client);
    void welcome2CanServ(Client* client);

    int isValidFormat(void);
    int checkClientLevel(Client* client);
    int determineFlag(void);
};

#endif // COMMAND_USER_HPP
