#include "Server.hpp"

Server::Server() : maxFd(MAX_FD), socketFd(-1)
{
    // Initialize socketFd to -1 and maxFd to MAX_FD.
    // We set socketFd to -1 because we will use socketFd as a flag.
}

Server::~Server()
{
	for (std::map<int, Client*>::iterator it = this->clientList.begin(); it != this->clientList.end(); ++it)
		delete it->second;
	for (std::map<std::string, Channel*>::iterator it = this->channelList.begin(); it != this->channelList.end(); ++it)
		delete it->second;
}

void Server::initServer(char* port, char* password, bool DEBUG)
{
	this->DEBUG = DEBUG;
    try
    {
        // Parses the port number and password.
        parseArgs(port, password);
        // Creates a socket for the server.
        setSocket();
        // Sets the socket option to reuse the address (1 to enable the options).
        // SOL_SOCKET: socket level, SO_REUSEADDR: reuse address.
        int optval = 1;
        setsockopt(this->socketFd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
        // Sets the socket to non-blocking.
		fcntl(this->socketFd, F_SETFL, O_NONBLOCK);
		// Sets the server address.
        setServAddr();
        // Binds the socket to the address and port number.
        bindSocket();
		// Listens for connections on the socket.
		listen(this->socketFd, this->addr.sin_port);
		// Sets the pollfd list.
        setPollFds();
        // Displays the welcome message.
        displayWelcome();
        std::cout << "Listening on port:" << this->port << std::endl;
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}

void Server::initCommandMap(std::map<std::string, ACommand *> &cmdMap)
{
	this->cmdMap = cmdMap;
}

void Server::setServAddr()
{
    // Sets IPv4. (AF_INET: IPv4, AF_INET6: IPv6)
    this->addr.sin_family = AF_INET;
    // Sets IP address, INADDR_ANY means any address.
    this->addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // Sets port number to network byte order.
    this->addr.sin_port = htons(this->port);
    // htonl() and htons() convert the host byte order to the network byte order.
	if (DEBUG)
	{
		std::cout << BACK_CYAN << "Server Configured for IPv4" << RESET << std::endl;
		std::cout << BACK_CYAN << "Server Address: " << inet_ntoa(this->addr.sin_addr) << RESET << std::endl;
		std::cout << BACK_CYAN << "Server Port: " << ntohs(this->addr.sin_port) << RESET << std::endl;
	}

}

void Server::setPollFds()
{
	// Temporary pollfd for the server.
	pollfd serverPollFd;
	// Sets the server pollfd.
	serverPollFd.fd = this->socketFd;
	// Sets the server pollfd events to POLLIN.
	// POLLIN: there is data to read.
	serverPollFd.events = POLLIN;
	// Adds the server pollfd to the pollfd list.
	this->pollFdList.push_back(serverPollFd);
	if (DEBUG)
		std::cout << BACK_CYAN << "Server PollFd Created, its FD is: " << this->socketFd << RESET << std::endl;
}

void Server::setSocket()
{
    int fd;

    // Creates a socket() and returns a file descriptor.
    // AF_INET: IPv4, AF_INET6: IPv6
    // SOCK_STREAM: TCP, SOCK_DGRAM: UDP (TCP: reliable, UDP: unreliable)
    // 0: protocol, 0 means the default protocol.
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
        throw(Exceptions::socketCreateException());
    this->socketFd = fd;
	if (DEBUG)
		std::cout << BACK_CYAN << "Server Socket Created, its FD is: "<< this->socketFd << RESET << std::endl;
}

void Server::bindSocket()
{
    // Binds the socket to the address and port number.
    // socketFd: socket file descriptor, addr: server address, sizeof(addr): size of server address.
    if (bind(this->socketFd, (struct sockaddr*)&(this->addr), sizeof(this->addr)) < 0)
        throw(Exceptions::bindException());
}

void Server::waitForEvents()
{
    int res;

	// poll() waits for events on the pollfd list.
	// poll() returns the number of pollfd that have events(changes).
	// pollFdList: pollfd list, pollFdList.size(): number of pollfd, 5000: timeout.
	// In c++ pollFdList[0] is the same as pollFdList.at(0). Starting point of the pollfd list.
	res = poll(&this->pollFdList[0], this->pollFdList.size(), 5000);
	if (res < 0)
		throw(Exceptions::pollException());
}

void Server::pollAccept()
{
    int                clientSockFd = -1;
    struct sockaddr_in clientAddr;
    unsigned int       size = sizeof(clientAddr);

	// Accepts a connection from a client.
	// socketFd: server socket file descriptor, clientAddr: client address, size: size of client address.
    clientSockFd = accept(this->socketFd, (struct sockaddr*)&clientAddr, &size);
    if (clientSockFd < 0)
        throw(Exceptions::acceptException());

	// Checks if the number of client socket file descriptors is greater than MAX_FD.
    if (pollFdList.size() >= MAX_FD)
	{
		std::cerr << "Maximum number of clients reached. Connection denied." << std::endl;
		close(clientSockFd);
		return ;
	}
	
	// Sets the client socket to non-blocking.
	// F_SETFL: set the file status flags to the value specified by arg.
	// O_NONBLOCK: non-blocking mode.
    fcntl(clientSockFd, F_SETFL, O_NONBLOCK);

	// Temporary pollfd for the client.
    pollfd clientPollFd;
	// Sets the client pollfd.
    clientPollFd.fd = clientSockFd;
	// Sets the client pollfd events to POLLIN and POLLOUT.
	// POLLIN: there is data to read, POLLOUT: writing now will not block.
    clientPollFd.events = POLLIN | POLLOUT;

	// Adds the client pollfd to the pollfd list.
    pollFdList.push_back(clientPollFd);

	// Creates a new client and adds it to the client list.
    Client	*temp = new Client(clientAddr, clientSockFd);

	if (temp)
	{
    	clientList.insert(std::make_pair(clientSockFd, temp));
		std::cout << "User: " << temp->getSockFd() << " has joined the server.\r\n";
	}
	else
	{
		std::cerr << "Error! Client creation failed." << std::endl;
		close(clientSockFd);
		pollFdList.pop_back();
		return ;
	}
}

void Server::pollDisconnect(int fd)
{
	std::map<int, Client*>::iterator it = this->clientList.find(fd);
	if (it != this->clientList.end())
	{
		Client* client = it->second;

		this->leaveAll(fd);

		std::cout << "User: " << client->getSockFd() << " has left the server.\r\n";

		this->deleteClientElement(fd);

		close(fd);		
	}
}

void Server::pollRead(int fd)
{
	Client	*client = this->clientList.find(fd)->second;

	if (!client)
		return ;
	
	int ret = client->recvClient();

	if (ret <= 0)
	{
		this->pollDisconnect(fd);
		return ;
	}

	std::string msg = client->getRecvBuff();

	std::vector<std::string> argv = this->parser.parseOn(msg);
	
	if (argv.size() == 0)
		return ;

	std::string cmdName = argv[1];
	std::map<std::string, ACommand *>::iterator it = cmdMap.find(cmdName);
	
	if (it != cmdMap.end())
	{
		it->second->setCommand(argv);
		it->second->exec(client);
	}
	else
	{
		client->sendToClient("ERROR: Command not found.\r\n");
	}
	client->getRecvBuff().clear();
	argv.clear();
}

void Server::pollSend(int fd)
{

    Client* client = this->clientList.find(fd)->second;
    
    if (!client)
        return;
    
	// TO-DO : Implement server to client message sending.
    std::string formattedMessage = ": MESSAGE TO CLIENT \r\n";

    ssize_t bytesSent = send(fd, formattedMessage.c_str(), formattedMessage.length(), 0);
    
    if (bytesSent < 0)
        std::cout << "Error sending message to client" << std::endl;

}

void Server::addChannelElement(std::string const channelName, Channel *newChannel)
{
	if (newChannel)
    	this->getChannelList().insert(std::make_pair(channelName, newChannel));
	if (DEBUG)
		std::cout << BACK_CYAN << "Channel: " << channelName << " created" << RESET << std::endl;
}

void Server::deleteChannelElement(std::string const channelName)
{
	Channel *temp = this->getChannelList().find(channelName)->second;
	if (temp)
	{
		this->getChannelList().erase(channelName);
		delete temp;
		if (DEBUG)
			std::cout << BACK_CYAN << "Channel: " << channelName << " deleted" << RESET << std::endl;
	}
	if (DEBUG)
		std::cout << BACK_CYAN << "Channel: " << channelName << " not found" << RESET << std::endl;
}

void Server::deleteClientElement(const int fd)
{
	Client *temp = this->getClientList().find(fd)->second;
	if (temp)
	{
		this->getClientList().erase(fd);
		delete temp;
		if (DEBUG)
			std::cout << BACK_CYAN << "Client: " << fd << " deleted" << RESET << std::endl;
	}
	else
		if (DEBUG)
			std::cout << BACK_CYAN << "Client: " << fd << " not found" << RESET << std::endl;
}

void Server::parseArgs(char* port, char* password)
{
	// Parses the port number and password.
    this->port = std::atoi(port);
	// 1024-65536 Since the port number is 16 bits, the range is 0-65535.
	// Ports below 1024 are reserved for the system.
    if (this->port < 1024 || this->port > 65536)
        throw(Exceptions::invalidPortException());
    this->password = static_cast<std::string>(password);
	if (isPasswordValid(this->password) == false)
		throw(Exceptions::invalidPasswordException());
	if (DEBUG)
	{
		std::cout << BACK_CYAN << "Server Listening Port: " << this->port << RESET << std::endl;
		std::cout << BACK_CYAN << "Server Password: " << this->password << RESET << std::endl;
	}

}

int Server::getPort() const
{
    return (this->port);
}

int Server::getSocketFd() const
{
    return (this->socketFd);
}

int Server::getCurrentMaxFd() const
{
    return (this->maxFd);
}

std::string Server::getPassword() const
{
    return (this->password);
}

struct sockaddr_in Server::getAddr() const
{
    return (this->addr);
}

std::vector<pollfd>& Server::getPollFdList()
{
    return (this->pollFdList);
}

std::map<std::string, Channel *>& Server::getChannelList()
{
    return (this->channelList);
}

std::map<int, Client *>& Server::getClientList()
{
    return (this->clientList);
}

std::map<std::string, ACommand *>& Server::getCmdMap()
{
	return (this->cmdMap);
}

void Server::leaveAll(int fd)
{
	(void) fd;
	// TO DO: Implement this function.

    // Client* client = this->getClientList().find(fd)->second;
    // if (client != NULL)
    // {
    //     std::map<std::string, Channel*>::iterator it = client->getChannelList().begin();
    //     std::string                                  msgBuf;
    //     // for(it = client->getChannelList().begin(); it != client->getChannelList().end(); it++)
    //     while (client->getChannelList().size() != 0)
    //     {
    //         it = client->getChannelList().begin();
    //         msgBuf = ":" + client->getNickname() + " PART " + it->second->getChannelName() + "\r\n";
    //         it->second->broadcast(msgBuf, client);
    //         it->second->deleteClientElement(client->getSockFd());
    //         if (it->second->getClientList().size() == 0)
    //         {
    //             deleteChannelElement(it->second->getChannelName());
    //         }
    //         client->deleteChannelElement(it->second->getChannelName());
    //     }
    // }
}

void Server::terminate()
{
    std::string const closeMsg = "Server Closed! Buh-bye!!\r\n";

    for (std::map<int, Client*>::iterator it = this->clientList.begin(); it != this->clientList.end(); it++)
        send(it->second->getSockFd(), closeMsg.c_str(), closeMsg.length(), 0);
}