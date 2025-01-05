#include "ServerLauncher.hpp"

ServerLauncher::ServerLauncher(ConfigData &configData) : _configData(configData)
{
    return ;
}

/*ServerLauncher::ServerLauncher(ServerLauncher const & obj)
{
    *this = obj;
    return ;
}*/

/*ServerLauncher & ServerLauncher::operator=(ServerLauncher const & rhs)
{
	if(this != &rhs)
    {
        _configData = rhs._configData;
		return (*this);
    }
	return (*this);
}*/

ServerLauncher::~ServerLauncher(void)
{
    return ;
}

void ServerLauncher::ft_creatSockets(void)
{
    for(std::vector<ServerBlock>::iterator itServerBlock = _configData.getServerBlocks().begin();
        itServerBlock != _configData.getServerBlocks().end();
        itServerBlock++)
    {
        for(std::multimap<std::string, unsigned int>::iterator itListen = itServerBlock->getListen().begin();
        itListen != itServerBlock->getListen().end();
        itListen++)
        {
        	// Set IP:PORT in struct sockaddr_in
        	struct	sockaddr_in sockAddrIn;
            int		addrLen;
            addrLen = sizeof(sockAddrIn);
            memset(&sockAddrIn, 0, addrLen);
            sockAddrIn.sin_family = AF_INET;
            if(itListen->first == "localhost")
            {
                sockAddrIn.sin_addr.s_addr = INADDR_LOOPBACK; // set IP //INADDR_ANY
            }
            else
            {
                sockAddrIn.sin_addr.s_addr = htonl(ft_stringToIPAddress(itListen->first)); // set IP
            }
            sockAddrIn.sin_port = htons(itListen->second); // set PORT
        	
            // Create a socket (IPv4, TCP)
            int	sockDescriptor;
            sockDescriptor = socket(AF_INET, SOCK_STREAM, 0);
            if(sockDescriptor == -1)
            {
            	ft_closeSockets();
            	throw std::runtime_error("Error in socket()");
            }
            
			// Set socket options
            /*int optVal;
            int sockOpt;
            optVal = 1;
            sockOpt = setsockopt(sockDescriptor, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal));
            if(sockOpt == -1)
            {
                exit(EXIT_FAILURE);
            }*/
            
            // Bind socket to the IP:PORT
            int	bindRes;
            bindRes = bind(sockDescriptor, (struct sockaddr*)&sockAddrIn, addrLen);
            if (bindRes == -1)
            {
            	ft_closeSockets();
				throw std::runtime_error("Error in bind()");
            }
            
            // Listen for connections
            int	listenRes;
            listenRes = listen(sockDescriptor, SOMAXCONN);
            if(listenRes == -1)
            {
				ft_closeSockets();
                throw std::runtime_error("Error in listen()");
            }
            
            // Initialize poll structure
            ft_addToPollDescriptors(sockDescriptor, POLLIN);
        }
    }
    _numberOfListeningSockets = _pollDescriptors.size();
    return ;
}

uint32_t ServerLauncher::ft_stringToIPAddress(std::string const & ipAddress)
{
	// TODO
    (void)ipAddress;

    uint32_t res;
    res = 0;
    return (res);
}

void ServerLauncher::ft_closeSockets(void)
{
	for(std::vector<struct pollfd>::iterator itPollDescriptors = _pollDescriptors.begin();
        itPollDescriptors != _pollDescriptors.end();
        itPollDescriptors++)
	{
		close(itPollDescriptors->fd);
	}
	return ;
}

void ServerLauncher::ft_addToPollDescriptors(int sockDescriptor, short events)
{
    struct pollfd pollDescriptor;

    pollDescriptor.fd = sockDescriptor; // Monitor the listening socket
    pollDescriptor.events = events;
    _pollDescriptors.push_back(pollDescriptor);
}

void ServerLauncher::ft_launch(void)
{
    while (true)
    {
        int pollRes;
        pollRes = poll(_pollDescriptors.data(), _pollDescriptors.size(), -1);
        // TODO ???: zero indicates that the system call timed out 
        if (pollRes == -1)
        {
            ft_closeSockets();
            throw std::runtime_error("Error in poll()");
        }

        for (unsigned long i = 0; i < _pollDescriptors.size(); ++i)
        {
            // Check for new connections
            if (_pollDescriptors[i].revents & POLLIN)
			{
	            if (i < _numberOfListeningSockets) //_pollDescriptors[i].fd == server_fd
	            {
	            	ft_acceptAndAddNewClientToPollDescriptors(i);	
	            }
	            else // Check for data from clients
	            {
	            	//ft_readRequest();
				}
	    	}
	    	else if (_pollDescriptors[i].revents & POLLOUT)
			{
				//TODO: send response to the client
			}
	    	else if (_pollDescriptors[i].revents & POLLERR || _pollDescriptors[i].revents & POLLHUP)
			{
				//TODO: erase Descriptor flom vector
			}
        }
    }
    return ;
}

void ServerLauncher::ft_acceptAndAddNewClientToPollDescriptors(unsigned long i)
{
	struct	sockaddr_in clientAddr;
	socklen_t clientAddrLen;
	
	clientAddrLen = sizeof(clientAddr);
	
	int	clientDescriptor = accept(_pollDescriptors[i].fd, (struct sockaddr *)&clientAddr, &clientAddrLen);
	if (clientDescriptor == -1)
    {
        ft_closeSockets();
        throw std::runtime_error("Error in accept()");
    }
    
	// Add new client to _pollDescriptors
	struct pollfd	clientPollFd;
	clientPollFd.fd = clientDescriptor;
	clientPollFd.events = POLLIN | POLLOUT;
	_pollDescriptors.push_back(clientPollFd);
	
	return ;
}

/*void ServerLauncher::ft_creatSockets() // test delete
{
            // Create a socket (IPv4, TCP)
            int sockDescriptor;
            sockDescriptor = socket(AF_INET, SOCK_STREAM, 0);
            if(sockDescriptor == -1)
            {
                exit(EXIT_FAILURE);
            }

            int optVal;
            int sockOpt;
            optVal = 1;
            sockOpt = setsockopt(sockDescriptor, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal));
            if(sockOpt == -1)
            {
                exit(EXIT_FAILURE);
            }

            struct sockaddr_in sockAddrIn;
            int addrLen;
            addrLen = sizeof(sockAddrIn);
            memset(&sockAddrIn, 0, addrLen);
            sockAddrIn.sin_family = AF_INET;
            sockAddrIn.sin_addr.s_addr = INADDR_ANY;
            sockAddrIn.sin_port = htons(3434);
            
            // Bind to a port
            int bindRes;
            bindRes = bind(sockDescriptor, (struct sockaddr*)&sockAddrIn, addrLen);
            if (bindRes == -1)
            {
                close(sockDescriptor);
                exit(EXIT_FAILURE);
            }
            
            // Listen for connections
            int listenRes;
            listenRes = listen(sockDescriptor, SOMAXCONN);
            if(listenRes == -1)
            {
                close(sockDescriptor);
                exit(EXIT_FAILURE);
            }


	bool active = true;
	int connection;
	while (active)
	{
		std::cout << "Listening on Port: " << 3434 << std::endl;
		connection = accept(sockDescriptor, (struct sockaddr *)(&sockAddrIn), (socklen_t *)&addrLen);

		char buffer[100000];
		ssize_t bytesRead = read(connection, buffer, 100000);
		std::cout << "Le message fait: " << bytesRead << " characteres" << std::endl;
		std::cout << "*****BUFFER START*****" << std::endl;
		std::cout << buffer << std::endl;
		std::cout << "*****BUFFER END*****" << std::endl;
        std::cout << std::endl;

		std::string content = "<h1>Bonjour, je suis un serveur HTTP tout simple!</h1>";
		std::string response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(content.length()) + "\n\n" + content;
		send(connection, response.c_str(), response.size(), 0);
		close(connection);
	}



    return ;
}*/
