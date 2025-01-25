
#include "ConfigFileParser.hpp"


int ft_main(int argc, char **argv)
{
    // if(argc > 2)
    // {
    //     std::cerr << "Wrong number of arguments" << std::endl;
    //     exit(1);
    // }  
    // if(argc == 1)
    //     configFileName = "default.conf";
    // else
    //     configFileName = argv[1];
    
    // to avoid config file type and path errors , now using get_filename

    //config file parsing
    //////////////////////////////////////////////////////////////////////////
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
    cout << "✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅"<<endl;
    //////////////////////////////////////////////////////////////////
    // try
	// {
	// 	// TODO: set config info to object configData
    // 	ConfigData configData;
	// 	configData.ft_setConfigData(configFileName);
    //     configData.ft_printConfigData();
		
	// 	// TODO: make ServerLauncher implementation
	// 	//ServerLauncher serverLauncher(configData);
	// 	//serverLauncher.ft_creatSockets();
    // 	//serverLauncher.ft_launch();
	// }
	// catch(std::exception &e)
	// {
	// 	std::cerr << e.what() << std::endl;
	// }
	
    return (0);
}


int main(int argc, char **argv)
{
    ft_main(argc, argv);
   // system("leaks webserv");
  
    return (0);
}




// int main(int argc, char **argv)
// {
//     try
//     {
//         pars_config(argc, argv);
//     }
//     catch (const std::exception &e)
//     {
//         cerr << "Failed to parse configuration file." << endl;
//         return 1; 
//     }
//     cout << "Program continues with parsed configuration." << endl;
//     return 0; 
// }
