#ifndef SERVER_HPP
# define SERVER_HPP

// arpa/inet.h : inet_ntoa (IP address)
# include <arpa/inet.h>
// exception: exception
# include <exception>
// fcntl.h: fcntl (file control)
# include <fcntl.h>
// iostream: cout, endl
# include <iostream>
// cstdlib: atoi (string to integer)
# include <cstdlib>
// netinet/in.h: sockaddr_in (socket address)
# include <netinet/in.h>
// string: string
# include <string>
// poll.h: poll (polling)
# include <poll.h>
// sys/socket.h & sys/types.h: socket, bind, listen, accept
# include <sys/socket.h>
# include <sys/types.h>
// unistd.h: close (close file descriptor)
# include <unistd.h>
// map: map
# include <map>
// vector: vector
# include <vector>
// utility: pair (pair.first, pair.second)
# include <utility>


// Server Headers
# include "ACommand.hpp"
# include "Client.hpp"
# include "Channel.hpp"
# include "Exception.hpp"
# include "Colors.hpp"
# include "Parsing.hpp"
# include "utils.hpp"
// Command Headers
# include "ACommand.hpp"
# include "Bot.hpp"
# include "Join.hpp"
# include "Kick.hpp"
# include "Nick.hpp"
# include "Notice.hpp"
# include "Part.hpp"
# include "Pass.hpp"
# include "Ping.hpp"
# include "Prvmsg.hpp"
# include "Quit.hpp"
# include "User.hpp"
# include "Cap.hpp"
# include "Who.hpp"

# define MAX_FD 100
# define BUFFER_SIZE 512
# define SERVERNAME "IRC"

class Server
{
  private:
	// Server Info
    int         port;
    std::string password;

	// MAX_FD (Users + Server)
    int maxFd;

	// Socket Info
    int                socketFd;
    struct sockaddr_in addr;

    // Parsing
    Parsing parser;

    // Commands
    Pass	*cmdPass;
    User	*cmdUser;
    Nick	*cmdNick;
    Join	*cmdJoin;
    Cap     *cmdCap;
    Who     *cmdWho;
    // Quit	*cmdQuit;
    // Prvmsg	*cmdPrvmsg;
    // Ping	*cmdPing;
    // Part	*cmdPart;
    Notice	*cmdNotice;
    // Kick	*cmdKick;

	// pollFdList
    std::vector<pollfd> pollFdList;

	// Client, Channel, Command Maps
    std::map<int, Client *>             clientList;
    std::map<std::string, Channel *>    channelList;
    std::map<std::string, ACommand *>   cmdMap;

    // Debug Mode
    bool DEBUG;

	// Not Implemented
    Server(Server const &copy);
    Server& operator=(Server const &copy);

  public:
    Server();
    ~Server();

    // Server Init
    void initServer(char* port, char* password, bool DEBUG);

    // Command Init
    void initCommandMap();

    // Socket Init
    void setSocket();
    void setSocketOptions();
    void setSocketNonBlocking();
    void setServAddr();
    void bindSocket();
    void listenSocket();
    void setPollFds();

    // run
    void waitForEvents();
    void pollAccept();
    void pollDisconnect(int fd);
    void pollRead(int fd);
    void pollSend(int fd);

    // utils
    void addChannelElement(std::string const channelName, Channel* newChannel);
    void deleteChannelElement(std::string const channelName);
    void deleteClientElement(const int fd);
    void deletePollFdElement(const int fd);
    void parseArgs(char* port, char* password);


    // getter
    int	getPort() const;
    int	getSocketFd() const;
    int	getCurrentMaxFd() const;
    std::string getPassword() const;

    struct sockaddr_in	getAddr() const;
    
    std::vector<pollfd>					&getPollFdList();
    std::map<int, Client *>				&getClientList();
    std::map<std::string, Channel *>	&getChannelList();
    std::map<std::string, ACommand *>	&getCmdMap();

    void  terminate();
};

#endif