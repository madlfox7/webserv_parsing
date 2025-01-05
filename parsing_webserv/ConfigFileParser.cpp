#include "ConfigFileParser.hpp"
#include "Parser.hpp"

ConfigFileParser::ConfigFileParser(void)
{
    return ;
}

ConfigFileParser::ConfigFileParser(ConfigFileParser const & obj)
{
    *this = obj;
    return ;
}

ConfigFileParser & ConfigFileParser::operator=(ConfigFileParser const & rhs)
{
    if(this != &rhs)
    {
		return (*this);
    }
	return (*this);
}

ConfigFileParser::~ConfigFileParser(void)
{
    return ;
}

void ConfigFileParser::ft_parseConfigFile(std::string &configFileName)
{
   try
    {
        pars_server(configFileName.c_str());
    }
    catch (const std::runtime_error &e)
    {
        cout << e.what() << endl;
        throw; 
    }
    catch (const std::exception &e)
    {
        cout << "Error: An unexpected exception occurred." << endl;
        throw; 
    }

    return ;
}
