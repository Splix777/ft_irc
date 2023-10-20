#ifndef COMMAND_NOTICE_HPP
#define COMMAND_NOTICE_HPP

#include "ACommand.hpp"

class Notice : public ACommand
{
  protected:
    Notice(const Notice& obj);            // No use.
    Notice& operator=(const Notice& obj); // No use.

  public:
    Notice();
    Notice(Server* serv);
    ~Notice();
    void noticeOn(Client* client);

    void        noticeToChannel(void);
    Channel* isExistChannelName(std::string name);
    Client*  isExistNickname(std::string name);
    int         isKicked(Client* clinet, Channel* channel);

    void executeNotice(Client* client);

    int isValidFormat(void);
    int checkClientLevel(Client* client);
    int determineFlag(void);
};

#endif // COMMAND_NOTICE_HPP
