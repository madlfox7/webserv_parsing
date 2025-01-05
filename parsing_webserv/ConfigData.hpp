#ifndef CONFIGDATA_HPP
# define CONFIGDATA_HPP

#include "ServerBlock.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>

class ConfigData
{
    public:
        ConfigData(void);
        //ConfigData(std::string configFileName);
        ConfigData(ConfigData const & obj);
        ConfigData & operator=(ConfigData const & rhs);
        ~ConfigData(void);
        
        void									ft_setConfigData(std::string & configFileName);
        std::vector<ServerBlock> &				getServerBlocks(void);
        
        void									ft_printConfigData(void); // delete before pushing to git repository
        void                                    ft_printLocation(ServerBlock & serverBlock);

    private:
    	std::string 							_configFileName;
        std::vector<ServerBlock>				_serverBlocks;
        
        void									ft_setConfigData(void);
        std::pair<std::string, unsigned int>	ft_creatPairForListen(std::string const & strListen);
        std::string								ft_creatStringForServerName(std::string const & strServerName);
        std::pair<unsigned int, std::string>	ft_creatPairForErrorPages(std::string const & strErrorPage);
        unsigned int							ft_creatUIntForClientMaxBodySize(std::string const & strClientMaxBodySize);
        LocationBlock							ft_creatLocation(std::ifstream & ifs, std::string const & strLocation);
        std::vector<std::string>				ft_creatAllowMethods(std::string const & strAllowMethods);
        std::pair<unsigned int, std::string>	ft_creatPairForReturn(std::string const & strReturn);
        std::string								ft_creatStringForRoot(std::string const & strRoot);
        std::string								ft_creatStringForAutoindex(std::string const & strAutoindex);
        std::string								ft_creatStringForIndex(std::string const & strIndex);

};

#endif
