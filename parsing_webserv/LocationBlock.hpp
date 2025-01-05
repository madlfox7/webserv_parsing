#ifndef LOCATIONBLOCK_HPP
# define LOCATIONBLOCK_HPP

#include <string>
#include <algorithm>
#include <map>
#include <vector>

class LocationBlock
{
    public:
        LocationBlock(void);
        //LocationBlock(LocationBlock const & obj);
        //LocationBlock & operator=(LocationBlock const & rhs);
        ~LocationBlock(void);
        
        std::string &							getPath(void);
        std::vector<std::string> &				getAllowMethods(void);
        std::map<unsigned int, std::string> &	getReturn(void);
        std::string &							getRoot(void);
        std::string &							getAutoindex(void);
        std::string &							getIndex(void);
        void									ft_setPath(std::string & path);
        void									ft_setAllowMethods(std::vector<std::string> allowMethods);
        void									ft_addToReturn(std::pair<unsigned int, std::string> pairForReturn);
        void									ft_setRoot(std::string & root);
        void									ft_setAutoindex(std::string & autoindex);
        void									ft_setIndex(std::string & index);

	private:
		std::string							_path;
		std::vector<std::string>			_allowMethods;
		std::map<unsigned int, std::string>	_return;
		std::string							_root;
		std::string							_autoindex;
        std::string							_index;
};

#endif
