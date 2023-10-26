#ifndef EXCEPTION_HPP
# define EXCEPTION_HPP

# include <exception>

# define ERR_UNKNOWNERROR 400
# define ERR_NOSUCHNICK 401
# define ERR_NOSUCHCHANNEL 403
# define ERR_INPUTTOOLONG 417
# define ERR_UNKNOWNCOMMAND 421
# define ERR_ERRONEUSNICKNAME 432
# define ERR_NICKNAMEINUSE 433
# define ERR_NOTONCHANNEL 442
# define ERR_USERONCHANNEL 443
# define ERR_NOTREGISTERED 451
# define ERR_NEEDMOREPARAMS 461
# define ERR_ALREADYREGISTERED 462
# define ERR_PASSWDMISMATCH 464
# define ERR_CHANNELISFULL 471
# define ERR_BANNEDFROMCHAN 474
# define ERR_NOPRIVILEGES 481
# define ERR_TOOMANYCHANNELS 405
# define ERR_CHANNELISFULL 471
# define ERR_CHANNELPASSWDMISMATCH 475
# define ERR_INVALIDPASSWORD 469

class Exceptions
{
  public:
    class invalidPasswordException : public std::exception
    {
        virtual const char* what() const throw();
    };

    class pollException : public std::exception
    {
        virtual const char* what() const throw();
    };

    class addChannelException : public std::exception
    {
        virtual const char* what() const throw();
    };

    class sendToClientException : public std::exception
    {
        virtual const char* what() const throw();
    };

    class socketCreateException : public std::exception
    {
        virtual const char* what() const throw();
    };

    class bindException : public std::exception
    {
        virtual const char* what() const throw();
    };

    class listenException : public std::exception
    {
        virtual const char* what() const throw();
    };

    class selectException : public std::exception
    {
        virtual const char* what() const throw();
    };

    class acceptException : public std::exception
    {
        virtual const char* what() const throw();
    };

    class tooManyClientFDException : public std::exception
    {
        virtual const char* what() const throw();
    };

    class invalidPortException : public std::exception
    {
        virtual const char* what() const throw();
    };
    class setsockoptException : public std::exception
    {
        virtual const char* what() const throw();
    };
    class fcntlException : public std::exception
    {
        virtual const char* what() const throw();
    };
    class connectException : public std::exception
    {
        virtual const char* what() const throw();
    };


    class recvSocketErrorException : public std::exception
    {
        virtual const char* what() const throw();
    };

    class recvSocketClosedException : public std::exception
    {
        virtual const char* what() const throw();
    };

    class pwIncorrectException : public std::exception
    {
        virtual const char* what() const throw();
    };

    class existNickException : public std::exception
    {
        virtual const char* what() const throw();
    };

    class existUserException : public std::exception
    {
        virtual const char* what() const throw();
    };

    class NotCertificatedException : public std::exception
    {
        virtual const char* what() const throw();
    };

    class PasswordNotSameException : public std::exception
    {
        virtual const char* what() const throw();
    };
};

#endif