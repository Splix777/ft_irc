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
    std::vector<std::string>    ftSplit(std::string bufferStr, std::string const &delim);

    // Server Parsers
    bool    isPasswordValid(std::string const &password);
};

#endif