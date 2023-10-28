#include "Server.hpp"

Server::Server() : maxFd(MAX_FD), socketFd(-1)
{
	// Initializes the parser.
    this->parser 	= Parsing();

	// Initializes the commands.
    this->cmdPass	= new Pass(this);
    this->cmdUser	= new User(this);
    this->cmdNick	= new Nick(this);
    this->cmdJoin	= new Join(this);
	this->cmdCap	= new Cap(this);
	this->cmdWho	= new Who(this);
	this->cmdMode	= new Mode(this);
    // this->cmdQuit	= new Quit(this);
    this->cmdPrvmsg = new Prvmsg(this);
    // this->cmdPing	= new Ping(this);
    this->cmdPart	= new Part(this);
    this->cmdNotice = new Notice(this);
	this->cmdList 	= new List(this);
    // this->cmdKick	= new Kick(this);

	// Initializes the command map.
	this->initCommandMap();
}

Server::~Server()
{
	// Clear the Client List.
	for (std::map<int, Client*>::iterator it = this->clientList.begin(); it != this->clientList.end(); ++it)
		delete it->second;

	// Clear the Channel List.
	for (std::map<std::string, Channel*>::iterator it = this->channelList.begin(); it != this->channelList.end(); ++it)
		delete it->second;

	// Clear the Command Map.	
	for (std::map<std::string, ACommand *>::iterator it = cmdMap.begin(); it != cmdMap.end(); ++it)
		delete it->second;

	// Clear the PollFd List.
	this->pollFdList.clear();

	// Close the socket.
	close(this->socketFd);

}

void	Server::initServer(char* port, char* password, bool DEBUG)
{
	this->DEBUG = DEBUG;
	// Displays the welcome message for Debug Mode.
	if (DEBUG)
		displayWelcome();
    try
    {
        // Parses the port number and password.
        parseArgs(port, password);

        // Creates a socket for the server.
        setSocket();

        // Sets the socket option to reuse the address (1 to enable the options).
        // SOL_SOCKET: socket level, SO_REUSEADDR: reuse address.
        setSocketOptions();

        // Sets the socket to non-blocking.
		setSocketNonBlocking();

		// Sets the server address.
        setServAddr();

        // Binds the socket to the address and port number.
        bindSocket();

		// Listens for connections on the socket.
		listenSocket();

		// Sets the pollfd list.
        setPollFds();
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}

void	Server::initServer(char* host, char* port, char* password, bool DEBUG)
{
	this->DEBUG = DEBUG;
	// Displays the welcome message for Debug Mode.
	if (DEBUG)
		displayWelcome();
    try
    {
        // Parses the port number and password.
        parseArgs(port, password);

        // Creates a socket for the server.
        setSocket();

        // Sets the socket option to reuse the address (1 to enable the options).
        // SOL_SOCKET: socket level, SO_REUSEADDR: reuse address.
        setSocketOptions();

        // Sets the socket to non-blocking.
		setSocketNonBlocking();

		// Sets the server address.
        setServAddr();

        // Binds the socket to the address and port number.
        bindSocket();

		// Listens for connections on the socket.
		listenSocket();

		// Sets the pollfd list.
        setPollFds();

		// Connects to the remote server.
		connectToRemoteServer(host);
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}

void	Server::connectToRemoteServer(char *s1)
{
	// Parses the remote server address, port number, and password.
	std::vector<std::string> remoteServerInfo = this->parser.ftSplit(s1, ":");
	if (remoteServerInfo.size() != 3)
		throw(Exceptions::connectException());
	std::string remoteServer = remoteServerInfo[0];
	std::string remotePort = remoteServerInfo[1];
	std::string password = remoteServerInfo[2];

	if (DEBUG)
		printDebug("Connecting to Remote Server: " + remoteServer + ":" + remotePort + " with password: " + password);
	
	// Create a sockaddr_in for the remote server.
	struct sockaddr_in remoteAddr;
	// Create a socket file descriptor for the remote server.
	int remoteSocketFd;

	// Sets the remote server address with the forwarded variables
	remoteAddr.sin_family = AF_INET;
	remoteAddr.sin_port = htons(std::atoi(remotePort.c_str()));
	remoteAddr.sin_addr.s_addr = inet_addr(remoteServer.c_str());

	// Creates a socket() and returns a file descriptor.
	// AF_INET: IPv4, AF_INET6: IPv6
	// SOCK_STREAM: TCP, SOCK_DGRAM: UDP (TCP: reliable, UDP: unreliable)
	// 0 means the default protocol.
	remoteSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (remoteSocketFd < 0)
	{
		close(remoteSocketFd);
		throw(Exceptions::socketCreateException());
	}

	// Connects to the remote server.
	// remoteSocketFd: socket file descriptor, remoteAddr: remote server address, sizeof(remoteAddr): size of remote server address.
	if (connect(remoteSocketFd, (struct sockaddr*)&remoteAddr, sizeof(remoteAddr)) < 0)
	{
		close(remoteSocketFd);
		throw(Exceptions::connectException());
	}

	std::string request = "PASS " + password + "\r\n" + "SERVER IRC 1 1 :Howdy Partner!\r\n" ;
	// Sends PASS <password> to the remote server.
	// Sends SERVER <servername> <hopcount> <token> <info> to the remote server.
	if (send(remoteSocketFd, request.c_str(), request.length(), 0) < 0)
	{
		close(remoteSocketFd);
		throw(Exceptions::sendToClientException());
	}

	// Adds the remote server socket to the pollfd list.
	pollfd remotePollFd;
	remotePollFd.fd = remoteSocketFd;
	remotePollFd.events = POLLIN | POLLOUT;
	this->pollFdList.push_back(remotePollFd);
	if (DEBUG)
		printDebug("Remote Server PollFd Created, its FD is: " + toString(remoteSocketFd));
}

void	Server::initCommandMap()
{
    cmdMap.insert(std::make_pair("PASS", cmdPass));
    cmdMap.insert(std::make_pair("USER", cmdUser));
    cmdMap.insert(std::make_pair("NICK", cmdNick));
    cmdMap.insert(std::make_pair("JOIN", cmdJoin));
    cmdMap.insert(std::make_pair("CAP", cmdCap));
	cmdMap.insert(std::make_pair("WHO", cmdWho));
	cmdMap.insert(std::make_pair("MODE", cmdMode));
	// cmdMap.insert(std::make_pair("PING", cmdPing));
    cmdMap.insert(std::make_pair("PART", cmdPart));
    // cmdMap.insert(std::make_pair("KICK", cmdKick));
    cmdMap.insert(std::make_pair("NOTICE", cmdNotice));
    cmdMap.insert(std::make_pair("PRIVMSG", cmdPrvmsg));
	cmdMap.insert(std::make_pair("LIST", cmdList));
    // cmdMap.insert(std::make_pair("QUIT", cmdQuit));
}

void	Server::setSocket()
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
		printDebug("Server Socket Created, its FD is: " + toString(this->socketFd));
}

void	Server::setSocketOptions()
{
	// Sets the socket option to reuse the address (1 to enable the options).
	// SOL_SOCKET: socket level, SO_REUSEADDR: reuse address.
	int optval = 1;
	if (setsockopt(this->socketFd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
		throw(Exceptions::setsockoptException());
}

void	Server::setSocketNonBlocking()
{
	// Sets the socket to non-blocking.
	// F_SETFL: set the file status flags to the value specified by arg.
	// O_NONBLOCK: non-blocking mode.
	if (fcntl(this->socketFd, F_SETFL, O_NONBLOCK) < 0)
		throw(Exceptions::fcntlException());
}

void	Server::setServAddr()
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
		printDebug("Server Configured for IPv4");
		printDebug("Server Address: " + static_cast<std::string>(inet_ntoa(this->addr.sin_addr)));
		printDebug("Server Port: " + toString(ntohs(this->addr.sin_port)));
	}

}

void	Server::bindSocket()
{
    // Binds the socket to the address and port number.
    // socketFd: socket file descriptor, addr: server address, sizeof(addr): size of server address.
    if (bind(this->socketFd, (struct sockaddr*)&(this->addr), sizeof(this->addr)) < 0)
        throw(Exceptions::bindException());
}

void	Server::listenSocket()
{
	// Listens for connections on the socket.
	// socketFd: socket file descriptor, 10: backlog, the maximum length to which the queue of pending connections for socketFd may grow.
	if (listen(this->socketFd, 10) < 0)
		throw(Exceptions::listenException());
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
		printDebug("Server PollFd Created, its FD is: " + toString(this->socketFd));
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
		if (DEBUG)
			printDebug("Maximum number of clients reached. Connection denied.");
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
    Client	*temp = new Client(clientAddr, clientSockFd, DEBUG);

	if (temp)
	{
    	clientList.insert(std::make_pair(clientSockFd, temp));
		if (DEBUG)
			printDebug("Client: " + toString(clientSockFd) + " created");
	}
	else
	{
		if (DEBUG)
			printDebug("Client: " + toString(clientSockFd) + " failed to create");
		close(clientSockFd);
		pollFdList.pop_back();
		return ;
	}
}

void Server::pollDisconnect(int fd)
{
	std::map<int, Client *>::iterator it = this->clientList.find(fd);
	if (it != this->clientList.end())
	{
		Client* client = it->second;

		it->second->leaveAllRooms();

		if (DEBUG)
			printDebug("Client " + toString(fd) + ": " + client->getNickname() + " disconnected");

		this->deleteClientElement(fd);
		this->deletePollFdElement(fd);

		close(fd);		
	}
}

void Server::pollRead(int fd)
{
	// Checks if the client is in the client list.
    std::map<int, Client*>::iterator clientIt = this->clientList.find(fd);
    if (clientIt == this->clientList.end() || !clientIt->second)
		return ;

	// Gets the client from the client list.
	Client	*client = clientIt->second;

	// Recieves data from the client.
	int ret = client->recvClient();

	// Checks if there is an error.
	if (ret <= 0)
	{
		this->pollDisconnect(fd);
		return ;
	}

	// Checks if the client has a complete message. There is a complete message if the message ends with \r\n.
	// There could be multiple messages that were queued up.
	std::string msg = client->getRecvMsg();

	// Split the messages by \r\n. This separates the all the complete messages.
	std::vector<std::string> msgQueue = this->parser.ftSplit(msg, "\r\n");

	if (DEBUG)
	{
		for(size_t i = 0; i < msgQueue.size(); i++)
			printDebug("Split Message [" + toString(i) + "]: " + msgQueue[i]);
	}
	// Loop through all the complete messages.
	for (size_t i = 0; i < msgQueue.size(); i++)
	{
		// Split the message by spaces. This separates the command and the arguments.
		std::vector<std::string> argv = this->parser.ftSplit(msgQueue[i], " ");
		// Checks if the message is empty.
		if (argv.size() == 0)
			continue ;

		if (DEBUG)
		{
			for(size_t i = 0; i < argv.size(); i++)
				printDebug("Split Message [" + toString(i) + "]: " + argv[i]);
		}
		// Gets the command name.
		std::string cmdName = argv[0];
		// Checks if the command is in the command map.
		std::map<std::string, ACommand *>::iterator it = cmdMap.find(cmdName);
		// If the command is in the command map, then execute the command.
		if (it != cmdMap.end())
		{
			it->second->setCommand(argv);
			it->second->exec(client);
		}
		else
		{
			if (DEBUG)
				client->sendToClient("ERROR: Command not found.");
		}
	}
	// Clears the recvMsg.
	client->getRecvMsg().clear();
}

void Server::pollSend(int fd)
{
	// Checks if the client is in the client list.
    std::map<int, Client*>::iterator clientIt = this->clientList.find(fd);

    if (clientIt == this->clientList.end() || !clientIt->second)
		return;

	Client	*client = clientIt->second;

	if (!client)
		return ;

	// If the send buffer is empty, then return.
	if (client->getSendBuff().size() == 0)
		return ;
	// Sends the data to the client.
	int ret = send(fd, client->getSendBuff().c_str(), client->getSendBuff().length(), 0);
	// Checks if there was an error.
	if (ret < 0)
		throw Exceptions::sendToClientException();
	// Clears the send buffer.
	client->getSendBuff().clear();
}

void Server::addChannelElement(std::string const channelName, Channel *newChannel)
{
	if (newChannel)
    	this->getChannelList().insert(std::make_pair(channelName, newChannel));
	if (DEBUG)
		printDebug("Channel: " + channelName + " created");
}

void Server::deleteChannelElement(std::string const channelName)
{
	Channel *temp = this->getChannelList().find(channelName)->second;
	if (temp)
	{
		this->getChannelList().erase(channelName);
		delete temp;
		if (DEBUG)
			printDebug("Channel: " + channelName + " deleted");
	}
	if (DEBUG)
		printDebug("Channel: " + channelName + " not found");
}

void Server::deleteClientElement(const int fd)
{
	Client *temp = this->getClientList().find(fd)->second;
	if (temp)
	{
		this->getClientList().erase(fd);
		delete temp;
		if (DEBUG)
			printDebug("Client " + toString(fd) + ":" + " deleted");
	}
	else
		if (DEBUG)
			printDebug("Client " + toString(fd) + ":" + " not found");
}

void Server::deletePollFdElement(const int fd)
{
	for (std::vector<pollfd>::iterator it = this->getPollFdList().begin(); it != this->getPollFdList().end(); ++it)
	{
		if (it->fd == fd)
		{
			this->getPollFdList().erase(it);
			if (DEBUG)
				printDebug("PollFd " + toString(fd) + ":" + " deleted");
			return ;
		}
	}
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
	if (parser.isPasswordValid(this->password) == false)
		throw(Exceptions::invalidPasswordException());
	if (DEBUG)
	{
		printDebug("Server Listening Port: " + toString(this->port));
		printDebug("Server Password: " + this->password);
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

std::vector<pollfd>	&Server::getPollFdList()
{
    return (this->pollFdList);
}

std::map<std::string, Channel *>	&Server::getChannelList()
{
    return (this->channelList);
}

std::map<int, Client *>	&Server::getClientList()
{
    return (this->clientList);
}

std::map<std::string, ACommand *>	&Server::getCmdMap()
{
	return (this->cmdMap);
}

void Server::terminate()
{
    std::string const closeMsg = "Server Closed! Buh-bye!!";

    for (std::map<int, Client *>::iterator it = this->clientList.begin(); it != this->clientList.end(); it++)
	{
		it->second->sendToClient(closeMsg);
		this->pollSend(it->first);
	}
}