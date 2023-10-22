#include "Exception.hpp"

// exception
const char* Exceptions::invalidPasswordException::what() const throw()
{
    return ("Error! Password must be 2-8 Printable Characters. \r\n");
}

const char* Exceptions::pollException::what() const throw()
{
    return ("Error! Poll error. \r\n");
}

const char* Exceptions::addChannelException::what() const throw()
{
    return "Client addChhannel : Failed ! \r\n";
}

const char* Exceptions::sendToClientException::what() const throw()
{
    return "Client sendToClient : couldn't send socket ! \r\n";
}

const char* Exceptions::socketCreateException::what() const throw()
{
    return ("Error! Can't create the socket. \r\n");
}

const char* Exceptions::bindException::what() const throw()
{
    return ("Bind error \r\n");
}

const char* Exceptions::listenException::what() const throw()
{
    return ("Listen error \r\n");
}

const char* Exceptions::selectException::what() const throw()
{
    return ("Select error \r\n");
}

const char* Exceptions::acceptException::what() const throw()
{
    return ("Error! Client FD creation failed.  \r\n");
}

const char* Exceptions::tooManyClientFDException::what() const throw()
{
    return ("There are too many client exception \r\n");
}

const char* Exceptions::invalidPortException::what() const throw()
{
    return ("Error! Port Num invalid. \r\n");
}
const char* Exceptions::setsockoptException::what() const throw()
{
    return ("Error! setsockopt error. \r\n");
}
const char* Exceptions::fcntlException::what() const throw()
{
    return ("Error! fcntl error. \r\n");
}


// IO

const char* Exceptions::recvSocketErrorException::what() const throw()
{
    return ("Error! Could not recv() client socket. \r\n");
}

const char* Exceptions::recvSocketClosedException::what() const throw()
{
    return ("Error! client socket is closed during recv(). \r\n");
}

const char* Exceptions::pwIncorrectException::what() const throw()
{
    return ("Error! Password incorrect. \r\n");
}

const char* Exceptions::PasswordNotSameException::what() const throw()
{
    return ("Error! Password is different. Try another one! \r\n");
}

const char* Exceptions::existNickException::what() const throw()
{
    return ("Error! There is the same Nickname in this server. Try another one! \r\n");
}

const char* Exceptions::existUserException::what() const throw()
{
    return ("Error! Teere is the same Username in this server. Try another one! \r\n");
}
const char* Exceptions::NotCertificatedException::what() const throw()
{
    return ("Error! There is no certification! Please finish making a password, username and nickname. \r\n");
}
