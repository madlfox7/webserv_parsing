#include "LocationBlock.hpp"

LocationBlock::LocationBlock(void)
{
  return ;
}

/*LocationBlock::LocationBlock(LocationBlock const & obj)
{
    *this = obj;
}*/

/*LocationBlock & LocationBlock::operator=(LocationBlock const & rhs)
{
  if(this != &rhs)
  {
	_path = rhs._path;
    return (*this);
  }
	return (*this);
}*/

LocationBlock::~LocationBlock(void)
{
  return ;
}

std::string & LocationBlock::getPath(void)
{
	return (this->_path);
}

std::vector<std::string> & LocationBlock::getAllowMethods(void)
{
	return (this->_allowMethods);
}

std::map<unsigned int, std::string> & LocationBlock::getReturn(void)
{
	return (this->_return);
}

std::string & LocationBlock::getRoot(void)
{
	return (this->_root);
}

std::string & LocationBlock::getAutoindex(void)
{
	return (this->_autoindex);
}

std::string & LocationBlock::getIndex(void)
{
	return (this->_index);
}

void LocationBlock::ft_setPath(std::string & path)
{
	_path = path;
	return ;
}

void LocationBlock::ft_setAllowMethods(std::vector<std::string> allowMethods)
{
	_allowMethods = allowMethods;
	return ;
}

void LocationBlock::ft_addToReturn(std::pair<unsigned int, std::string> pairForReturn)
{
	_return.insert(pairForReturn);
	return ;
}

void LocationBlock::ft_setRoot(std::string & root)
{
	_root = root;
	return ;
}

void LocationBlock::ft_setAutoindex(std::string & autoindex)
{
	_autoindex = autoindex;
	return ;
}

void LocationBlock::ft_setIndex(std::string & index)
{
	_index = index;
	return ;
}
