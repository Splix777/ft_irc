#ifndef PARSING_HPP
# define PARSING_HPP

# include <cstddef>
# include <string>
# include <vector>

class Parsing
{
  private:

  public:
    Parsing();
    ~Parsing();

    // Command Parsers
    std::vector<std::string>    cmdSplit(std::string bufferStr);

    // Server Parsers
    bool    isPasswordValid(std::string const &password);
};

#endif