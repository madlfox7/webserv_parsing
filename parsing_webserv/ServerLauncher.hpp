#ifndef SERVERLAUNCHER_HPP
# define SERVERLAUNCHER_HPP

#include "ConfigData.hpp"
#include "ServerBlock.hpp"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <unistd.h>
#include <string>
#include <algorithm>
#include <map>
#include <vector>
#include <cstring>

class ServerLauncher
{
    public:
        ServerLauncher(ConfigData &configData);
        //ServerLauncher(ServerLauncher const & obj);
        //ServerLauncher & operator=(ServerLauncher const & rhs);
        ~ServerLauncher(void);
        
        void										ft_creatSockets(void);
        void										ft_launch(void);

    private:
        ConfigData                  				_configData;
        std::vector<struct pollfd>  				_pollDescriptors;
        unsigned int                				_numberOfListeningSockets;
        std::map<int, std::vector<ServerBlock *> >	_serversSortedBySocketDescriptors;
        
        uint32_t									ft_stringToIPAddress(std::string const & ipAddress);
        void										ft_closeSockets(void);
        void										ft_addToPollDescriptors(int sockDescriptor, short events);
        void										ft_acceptAndAddNewClientToPollDescriptors(unsigned long i);
        //void										ft_readRequest();
};

#endif
