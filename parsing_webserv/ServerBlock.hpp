#ifndef SERVERBLOCK_HPP
# define SERVERBLOCK_HPP

#include "LocationBlock.hpp"
#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <vector>

class ServerBlock
{
    public:
        ServerBlock(void);
        //ServerBlock(ServerBlock const & obj);
        //ServerBlock & operator=(ServerBlock const & rhs);
        ~ServerBlock(void);
        
        std::multimap<std::string, unsigned int> &  getListen(void);
        std::string &							    getServerName(void);
        std::map<unsigned int, std::string> &	    getErrorPages(void);
        unsigned int &                              getClientMaxBodySize(void);
        std::vector<LocationBlock> &			    getLocations(void);
        void									    ft_addToListens(std::pair<std::string, unsigned int> pairForListen);
		void									    ft_setServerName(std::string & serverName);
		void									    ft_addToErrorPages(std::pair<unsigned int, std::string> pairForErrorPages);
		void									    ft_setClientMaxBodySize(unsigned int clientMaxBodySize);
		void									    ft_addToLocations(LocationBlock location);
		
    private:
        std::multimap<std::string, unsigned int>    _listen;
        std::string							        _serverName;
        std::map<unsigned int, std::string>	        _errorPages;
        unsigned int						        _clientMaxBodySize;
        std::vector<LocationBlock>					_locations;
};

#endif
