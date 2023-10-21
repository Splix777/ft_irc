#include "Operation.hpp"

Operation::Operation(char* s1, char* s2, bool DEBUG) : DEBUG(DEBUG)
{
    // Initialize server
    this->server 	= new Server();
    this->server->initServer(s1, s2, DEBUG);
    // Inititalize rest of classes
    this->parser 	= Parsing();

    this->cmdUser	= new User(this->server);
    // this->cmdQuit	= new Quit(this->server);
    // this->cmdPrvmsg = new Prvmsg(this->server);
    // this->cmdPing	= new Ping(this->server);
    this->cmdPass	= new Pass(this->server);
    // this->cmdPart	= new Part(this->server);
    // this->cmdNotice = new Notice(this->server);
    // this->cmdNick	= new Nick(this->server);
    // this->cmdJoin	= new Join(this->server);
    // this->cmdKick	= new Kick(this->server);
    // Inititalize command map
    initCommandMap();
}

Operation::~Operation()
{
    for (std::map<std::string, ACommand *>::iterator it = cmdMap.begin(); it != cmdMap.end(); ++it)
    {
        delete it->second;
    }
    delete server;
}

void	Operation::initCommandMap()
{
    cmdMap.insert(std::make_pair("PASS", cmdPass));
    // cmdMap.insert(std::make_pair("NICK", cmdNick));
    cmdMap.insert(std::make_pair("USER", cmdUser));
    // cmdMap.insert(std::make_pair("PING", cmdPing));
    // cmdMap.insert(std::make_pair("JOIN", cmdJoin));
    // cmdMap.insert(std::make_pair("PART", cmdPart));
    // cmdMap.insert(std::make_pair("KICK", cmdKick));
    // cmdMap.insert(std::make_pair("NOTICE", cmdNotice));
    // cmdMap.insert(std::make_pair("PRIVMSG", cmdPrvmsg));
    // cmdMap.insert(std::make_pair("QUIT", cmdQuit));
    server->initCommandMap(cmdMap);
}

void	Operation::pollLoop()
{

    server->waitForEvents();
    for (std::vector<pollfd>::iterator it = server->getPollFdList().begin(); it != server->getPollFdList().end(); ++it)
    {
        if (it->revents == 0)
            continue ;
        if (it->revents & POLLHUP)
        {
            server->pollDisconnect(it->fd);
            break ;
        }
        // if (it->revents & POLLOUT)
        // {
        //     server->pollSend(it->fd);
        //     break ;
        // }
        if (it->revents & POLLIN)
        {
            if (it->fd == server->getSocketFd())
            {
                server->pollAccept();
                break ;
            }
            else
            {
                server->pollRead(it->fd);
                break ;
            }
        }
    }
}

void	Operation::terminate()
{
    server->terminate();
}