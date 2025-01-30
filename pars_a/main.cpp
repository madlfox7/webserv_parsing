
#include "ConfigFileParser.hpp"


int ft_main(int argc, char **argv)
{
    std::string configFileName;
    try
    {
        configFileName = get_filename(argc, argv);
        ConfigFileParser::ft_parseConfigFile(configFileName);
    }
    catch (const std::exception &e)
    {
        cerr << "Failed to parse configuration file." << endl;
        return 1; 
    }
    cout << "✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅"<< endl;
    //////////////////////////////////////////////////////////////////
   // ...............................
    return (0);
}


int main(int argc, char **argv)
{
    ft_main(argc, argv);
   // system("leaks webserv");
  
    return (0);
}
