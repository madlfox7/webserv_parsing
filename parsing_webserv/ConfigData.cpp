#include "ConfigData.hpp"

ConfigData::ConfigData(void)
{
    return ;
}

/*ConfigData::ConfigData(std::string configFileName) : _configFileName(configFileName)
{
    ft_setConfigData();
	ft_printConfigData();
    return ;
}*/

ConfigData::ConfigData(ConfigData const & obj)
{
    *this = obj;
    return ;
}

ConfigData & ConfigData::operator=(ConfigData const & rhs)
{
	if(this != &rhs)
    {
		return (*this);
    }
	return (*this);
}

ConfigData::~ConfigData(void)
{
    return ;
}

void ConfigData::ft_setConfigData(std::string & configFileName) // : _configFileName(configFileName) (should it work ?)
{
	std::ifstream	ifs;
	std::string		tmpStr;
	
	_configFileName = configFileName;
	ifs.open(_configFileName.c_str());
	if (!ifs.is_open())
	{
		//TODO: if need clear and close before exit
		//std::cerr << "Error: couldn't open " << _configFileName << " file" << std::endl;
		//ifs.close();
		throw(std::runtime_error("⚠ Error: couldn't open " + _configFileName + " file"));
		//exit(EXIT_FAILURE);
	}
	for(unsigned int i = 0; std::getline(ifs, tmpStr, '\n'); ++i)
	{
		//std::cout << "****** IN FOR ******" << std::endl; // delete
		//std::cout << tmpStr << std::endl;
		/*for(unsigned long i = 0; i < tmpStr.size(); ++i)
		{
			std::cout << "(" << tmpStr[i] << ")" << std::endl;
		}*/
		if(tmpStr == "server")
		{
			ServerBlock tmpServerBlock;
			for(; std::getline(ifs, tmpStr, '\n'); ++i)
			{
				//std::cout << "tmpStr(" << tmpStr << ")" << std::endl;
				if(tmpStr.find("listen ") != std::string::npos)
				{
					//std::cout << "******IN LISTEN******" << std::endl;
					std::pair<std::string, unsigned int> tmpPairForListen = ft_creatPairForListen(tmpStr);
					tmpServerBlock.ft_addToListens(tmpPairForListen);
				}
				else if(tmpStr.find("server_name ") != std::string::npos)
				{
					std::string tmpServerName = ft_creatStringForServerName(tmpStr);
					tmpServerBlock.ft_setServerName(tmpServerName);
				}
				else if(tmpStr.find("error_page ") != std::string::npos)
				{
					std::pair<unsigned int, std::string> tmpPairForErrorPages = ft_creatPairForErrorPages(tmpStr);
					tmpServerBlock.ft_addToErrorPages(tmpPairForErrorPages);
				}
				else if(tmpStr.find("client_max_body_size ") != std::string::npos)
				{
					unsigned int tmpClientMaxBodySize = ft_creatUIntForClientMaxBodySize(tmpStr);
					tmpServerBlock.ft_setClientMaxBodySize(tmpClientMaxBodySize);
				}
				else if(tmpStr.find("location ") != std::string::npos)
				{
					//LocationBlock tmpLocation = ft_creatLocation(ifs, tmpStr);
					tmpServerBlock.ft_addToLocations(ft_creatLocation(ifs, tmpStr));
				}
				else if(tmpStr.find("}") != std::string::npos)
				{
					break;
				}
			}
			_serverBlocks.push_back(tmpServerBlock);
		}
	}
	return ;
}

std::pair<std::string, unsigned int> ConfigData::ft_creatPairForListen(std::string const & strListen)
{
	std::pair<std::string, unsigned int>	pair;
	std::string								str;
	std::size_t								pos;
	std::string								ip;
	int										port;
	
	str = strListen;
	pos = str.find("listen ");
	str.erase(0, pos + 7);
	
	pos = str.find(":");
	if(pos == std::string::npos)
	{
		ip = "localhost";
		port = atoi(str.c_str());
	}
	else
	{
		ip = str.substr(0, pos);
		if(ip == "127.0.0.1")
			ip = "localhost";
		port = atoi(str.substr(pos + 1).c_str());
	}
    pair = std::make_pair (ip, port);
	return (pair);
}

std::string ConfigData::ft_creatStringForServerName(std::string const & strServerName)
{
	std::string	str;
	std::size_t	pos;
	
	str = strServerName;
	pos = str.find("server_name ");
	str.erase(0, pos + 12);
	
	return (str);
}

std::pair<unsigned int, std::string> ConfigData::ft_creatPairForErrorPages(std::string const & strErrorPage)
{
	std::pair<unsigned int, std::string>	pair;
	std::string								str;
	std::size_t								pos;
	unsigned int							num;
	std::string								path;
	
	str = strErrorPage;
	pos = str.find("error_page ");
	str.erase(0, pos + 11);
	
	pos = str.find(" ");
	num = atoi(str.substr(0, pos).c_str());
	path = str.substr(pos + 1);
	
    pair = std::make_pair (num, path);
	return (pair);
}

unsigned int ConfigData::ft_creatUIntForClientMaxBodySize(std::string const & strClientMaxBodySize)
{
	std::string		str;
	unsigned int	size;
	std::size_t		pos;
	
	str = strClientMaxBodySize;
	pos = str.find("client_max_body_size ");
	size = atoi(str.substr(pos + 21).c_str());
	
	return (size);
}

LocationBlock	ConfigData::ft_creatLocation(std::ifstream & ifs, std::string const & strLocation)
{
	std::string		tmpStr;
	std::size_t		pos;
	LocationBlock	location;
	
	tmpStr = strLocation;
	pos = tmpStr.find("location ");
	tmpStr.erase(0, pos + 9);
	//std::cout << "(" << tmpStr << ")" << std::endl;
	location.ft_setPath(tmpStr);
	
	while(std::getline(ifs, tmpStr, '\n'))
	{
		//std::cout << "(" << tmpStr << ")" << std::endl;
		if(tmpStr.find("allow_methods ") != std::string::npos)
		{
			//std::cout << "(" << "ALLOW" << ")" << std::endl;
			std::vector<std::string> tmpAllowMethods = ft_creatAllowMethods(tmpStr);
			/*std::cout << "(" << tmpAllowMethods[0] << ")" << std::endl;
			std::cout << "(" << tmpAllowMethods[1] << ")" << std::endl;
			std::cout << "(" << tmpAllowMethods[2] << ")" << std::endl;*/
			location.ft_setAllowMethods(tmpAllowMethods);
		}
		else if(tmpStr.find("return ") != std::string::npos)
		{
			std::pair<unsigned int, std::string> tmpPairForReturn = ft_creatPairForReturn(tmpStr);
			location.ft_addToReturn(tmpPairForReturn);
		}
		else if(tmpStr.find("root ") != std::string::npos)
		{
			std::string tmpRoot = ft_creatStringForRoot(tmpStr);
			location.ft_setRoot(tmpRoot);
		}
		else if(tmpStr.find("autoindex ") != std::string::npos)
		{
			std::string tmpAutoindex = ft_creatStringForAutoindex(tmpStr);
			location.ft_setAutoindex(tmpAutoindex);
		}
		else if(tmpStr.find("index ") != std::string::npos)
		{
			std::string tmpIndex = ft_creatStringForIndex(tmpStr);
			location.ft_setIndex(tmpIndex);
		}
		else if(tmpStr.find("}") != std::string::npos)
		{
			break;
		}
	}
	//std::cout << "(" << location.getAutoindex() << ")" << std::endl;
	return (location);
}

std::vector<std::string> ConfigData::ft_creatAllowMethods(std::string const & strAllowMethods)
{
	//std::string	str;
	//std::size_t	pos;
	
	//str = strallowMethods;
	//pos = str.find("allow_methods ");
	//str.erase(0, pos + 14);
	
	std::vector<std::string> allowMethods;
	
	if(strAllowMethods.find("GET ") != std::string::npos)
	{
		allowMethods.push_back("GET");
	}
	if(strAllowMethods.find("POST ") != std::string::npos)
	{
		allowMethods.push_back("POST");
	}
	if(strAllowMethods.find("DELETE") != std::string::npos)
	{
		allowMethods.push_back("DELETE");
	}
	return (allowMethods);
}

std::pair<unsigned int, std::string> ConfigData::ft_creatPairForReturn(std::string const & strReturn)
{
	std::pair<unsigned int, std::string>	pair;
	std::string								str;
	std::size_t								pos;
	unsigned int							num;
	std::string								site;
	
	str = strReturn;
	pos = str.find("return ");
	str.erase(0, pos + 7);
	
	pos = str.find(" ");
	num = atoi(str.substr(0, pos).c_str());
	site = str.substr(pos + 1);
	
    pair = std::make_pair (num, site);
	return (pair);
}

std::string ConfigData::ft_creatStringForRoot(std::string const & strRoot)
{
	std::string	str;
	std::size_t	pos;
	
	str = strRoot;
	pos = str.find("root ");
	str.erase(0, pos + 5);
	
	return (str);
}

std::string ConfigData::ft_creatStringForAutoindex(std::string const & strAutoindex)
{
	std::string	str;
	std::size_t	pos;
	
	str = strAutoindex;
	pos = str.find("autoindex ");
	str.erase(0, pos + 10);
	
	return (str);
}

std::string ConfigData::ft_creatStringForIndex(std::string const & strIndex)
{
	std::string	str;
	std::size_t	pos;
	
	str = strIndex;
	pos = str.find("index ");
	str.erase(0, pos + 6);
	
	return (str);
}

std::vector<ServerBlock> & ConfigData::getServerBlocks(void)
{
    return (this->_serverBlocks);
}

void ConfigData::ft_printConfigData(void)
{
	std::cout << "File name(" << _configFileName << ")" << std::endl;

    for(std::vector<ServerBlock>::iterator itServerBlock = _serverBlocks.begin();
        itServerBlock != _serverBlocks.end();
        itServerBlock++)
    {
		//std::map<std::string, unsigned int>	_listen;
		std::cout << "******Listen******" << std::endl;
		for(std::map<std::string, unsigned int>::iterator itListen = itServerBlock->getListen().begin();
        	itListen != itServerBlock->getListen().end();
        	itListen++)
		{
			std::cout << "IP(" << itListen->first << ")" << " Port(" << itListen->second << ")" << std::endl;
		}
        //std::string							_serverName;
		std::cout << "******ServerName******" << std::endl;
		std::cout << "ServerName(" << itServerBlock->getServerName() << ")" << std::endl;
        //std::map<unsigned int, std::string>	_errorPages;
		std::cout << "******ErrorPages******" << std::endl;
		for(std::map<unsigned int, std::string>::iterator itErrorPages = itServerBlock->getErrorPages().begin();
        	itErrorPages != itServerBlock->getErrorPages().end();
        	itErrorPages++)
		{
			std::cout << "Num(" << itErrorPages->first << ")" << " Path(" << itErrorPages->second << ")" << std::endl;
		}
        //unsigned int						_clientMaxBodySize;
		std::cout << "******ClientMaxBodySize******" << std::endl;
		std::cout << "ClientMaxBodySize(" << itServerBlock->getClientMaxBodySize() << ")" << std::endl;
		//std::vector<Location>						_locations;
		std::cout << "******Locations******" << std::endl;
		ft_printLocation(*itServerBlock);
	}
	return;
}

void ConfigData::ft_printLocation(ServerBlock & serverBlock)
{
	for(std::vector<LocationBlock>::iterator itLocationBlock = serverBlock.getLocations().begin();
    	itLocationBlock != serverBlock.getLocations().end();
        itLocationBlock++)
    {
    	//std::string							_path;
    	std::cout << "******Path******" << std::endl;
		std::cout << "Path(" << itLocationBlock->getPath() << ")" << std::endl;
		//std::vector<std::string>				_allowMethods;
		std::cout << "******AllowMethods******" << std::endl;
		for(std::vector<std::string>::iterator itAllowMethods = itLocationBlock->getAllowMethods().begin();
        	itAllowMethods != itLocationBlock->getAllowMethods().end();
        	itAllowMethods++)
		{
			std::cout << "Method(" << *itAllowMethods << ")" << std::endl;
		}
		//std::map<unsigned int, std::string>	_return;
		std::cout << "******Return******" << std::endl;
		for(std::map<unsigned int, std::string>::iterator itReturn = itLocationBlock->getReturn().begin();
        	itReturn != itLocationBlock->getReturn().end();
        	itReturn++)
		{
			std::cout << "Num(" << itReturn->first << ")" << " Site(" << itReturn->second << ")" << std::endl;
		}
		//std::string							_root;
		std::cout << "******Root******" << std::endl;
		std::cout << "Root(" << itLocationBlock->getRoot() << ")" << std::endl;
		//std::string							_autoindex;
		std::cout << "******Autoindex******" << std::endl;
		std::cout << "Autoindex(" << itLocationBlock->getAutoindex() << ")" << std::endl;
	}
	return ;
}
