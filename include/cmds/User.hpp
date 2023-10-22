#ifndef COMMAND_USER_HPP
#define COMMAND_USER_HPP

#include "ACommand.hpp"

#define USERNAME_MAXLEN 30;

#define ERR_NEEDMOREPARAMS 461
#define ERR_ALREADYREGISTERED 462

class Server;

class User : public ACommand
{
  private:
    User();
    User(User const &obj);
    User& operator=(User const &obj);

  public:
    User(Server *serv);
    ~User();

    void exec(Client* client);
    
    void  validCheck(void);
    void setClientUser(Client* client);
    void welcome(Client *client);

    void isValidFormat(void);
    void checkClientLevel(Client* client);
};

#endif
