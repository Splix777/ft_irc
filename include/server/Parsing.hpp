#ifndef PARSING_HPP
#define PARSING_HPP

#include <cstddef>
#include <string>
#include <vector>

class Parsing
{
  private:
    std::string               buff;
    std::string               word;
    bool                      flag;

  public:
    Parsing();
    ~Parsing();

    std::vector<std::string>    parseOn(std::string bufferStr);

    void    parseClear(void);

    int     checkSpaceAndComma(size_t idx, std::vector<std::string> &vec);
    void    giveFlag(std::vector<std::string> &vec);
    void    makeBuffString(char* buffStr);
    void    checkColon(size_t idx, std::vector<std::string> &vec);
};

#endif