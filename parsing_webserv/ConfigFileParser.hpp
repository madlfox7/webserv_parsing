#ifndef CONFIGFILEPARSER_HPP
# define CONFIGFILEPARSER_HPP

#include "ServerBlock.hpp"
#include "Parser.hpp"
#include <algorithm>
#include <vector>

class ConfigFileParser
{
    public:
        ConfigFileParser(void);
        ConfigFileParser(ConfigFileParser const & obj);
        ConfigFileParser & operator=(ConfigFileParser const & rhs);
        ~ConfigFileParser(void);
        static void ft_parseConfigFile(std::string &configFileName);

    private:
};

#endif