#include "ServerBlock.hpp"

ServerBlock::ServerBlock(void)
{
    return ;
}

/*ServerBlock::ServerBlock(ServerBlock const & obj)
{
    *this = obj;
    return ;
}*/

/*ServerBlock & ServerBlock::operator=(ServerBlock const & rhs)
{
    if(this != &rhs)
    {
		return (*this);
    }
	return (*this);
}*/

ServerBlock::~ServerBlock(void)
{
    return ;
}

std::multimap<std::string, unsigned int> & ServerBlock::getListen(void)
{
    return (this->_listen);
}

std::string & ServerBlock::getServerName(void)
{
    return (this->_serverName);
}

std::map<unsigned int, std::string> & ServerBlock::getErrorPages(void)
{
	return (this->_errorPages);
}

unsigned int & ServerBlock::getClientMaxBodySize(void)
{
	return (this->_clientMaxBodySize);
}

std::vector<LocationBlock> & ServerBlock::getLocations(void)
{
	return (this->_locations);
}

void ServerBlock::ft_addToListens(std::pair<std::string, unsigned int> pairForListen)
{
	_listen.insert(pairForListen);
	return ;
}

void ServerBlock::ft_setServerName(std::string & serverName)
{
	_serverName = serverName;
	return ;
}

void ServerBlock::ft_addToErrorPages(std::pair<unsigned int, std::string> pairForErrorPages)
{
	_errorPages.insert(pairForErrorPages);
	return ;
}

void ServerBlock::ft_setClientMaxBodySize(unsigned int clientMaxBodySize)
{
	_clientMaxBodySize = clientMaxBodySize;
	return ;
}

void ServerBlock::ft_addToLocations(LocationBlock location)
{
	_locations.push_back(location);
	return ;
}
