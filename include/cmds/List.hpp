#ifndef COMMAND_LIST_HPP
#define COMMAND_LIST_HPP

#include "ACommand.hpp"

class Server;

class List : public ACommand
{
  private:
    List();
    List(List const &obj);
    List  &operator=(List const &obj);

  public:
    List(Server* serv);
    ~List();

    void exec(Client *client);

    void isValidFormat(void);
    void checkClientLevel(Client* client);
    void listCmd(Client* client);
	std::string	getResponse(std::string nick, std::map<std::string, Channel *>::iterator &channel_it);
};


#endif