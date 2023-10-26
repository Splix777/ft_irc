#include "Exception.hpp"

// exception
const char* Exceptions::invalidPasswordException::what() const throw()
{
    return ("Error! Password must be 2-8 Printable Characters.");
}

const char* Exceptions::pollException::what() const throw()
{
    return ("Error! Poll error.");
}

const char* Exceptions::addChannelException::what() const throw()
{
    return "Client addChhannel : Failed !";
}

const char* Exceptions::sendToClientException::what() const throw()
{
    return "Client sendToClient : couldn't send socket !";
}

const char* Exceptions::socketCreateException::what() const throw()
{
    return ("Error! Can't create the socket.");
}

const char* Exceptions::bindException::what() const throw()
{
    return ("Bind error");
}

const char* Exceptions::listenException::what() const throw()
{
    return ("Listen error");
}

const char* Exceptions::selectException::what() const throw()
{
    return ("Select error");
}

const char* Exceptions::acceptException::what() const throw()
{
    return ("Error! Client FD creation failed. ");
}

const char* Exceptions::tooManyClientFDException::what() const throw()
{
    return ("There are too many client exception");
}

const char* Exceptions::invalidPortException::what() const throw()
{
    return ("Error! Port Num invalid.");
}
const char* Exceptions::setsockoptException::what() const throw()
{
    return ("Error! setsockopt error.");
}
const char* Exceptions::fcntlException::what() const throw()
{
    return ("Error! fcntl error.");
}


// IO

const char* Exceptions::recvSocketErrorException::what() const throw()
{
    return ("Error! Could not recv() client socket.");
}

const char* Exceptions::recvSocketClosedException::what() const throw()
{
    return ("Error! client socket is closed during recv().");
}

const char* Exceptions::pwIncorrectException::what() const throw()
{
    return ("Error! Password incorrect.");
}

const char* Exceptions::PasswordNotSameException::what() const throw()
{
    return ("Error! Password is different. Try another one!");
}

const char* Exceptions::existNickException::what() const throw()
{
    return ("Error! There is the same Nickname in this server. Try another one!");
}

const char* Exceptions::existUserException::what() const throw()
{
    return ("Error! Teere is the same Username in this server. Try another one!");
}
const char* Exceptions::NotCertificatedException::what() const throw()
{
    return ("Error! There is no certification! Please finish making a password, username and nickname.");
}

const char* Exceptions::connectException::what() const throw()
{
    return ("Error! Connect to Remote Server error.");
}
