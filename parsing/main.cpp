#include "Parser.hpp"



int main(int argc, char **argv)
{
    try
    {
        pars_config(argc, argv);
    }
    catch (const std::exception &e)
    {
        cerr << "Failed to parse configuration file." << endl;
        return 1; 
    }
    cout << "Program continues with parsed configuration." << endl;
    return 0; 
}
