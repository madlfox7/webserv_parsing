#include "Parser.hpp"

string get_filename(int argc, char **argv)
{
    if (argc > 2) 
        throw std::runtime_error("⚠ Error: Too many arguments");
    if (argc < 2)
    {
        string defaultPath = resolvePath(DEFAULT_FILENAME);
        if (checkFileType(defaultPath.c_str()) != FileType_RegularFile) 
            throw std::runtime_error("⚠ Error: Default file is not a regular file");
        return defaultPath;
    }
    string filePath = resolvePath(argv[1]);
    if (checkFileType(filePath.c_str()) != FileType_RegularFile)
        throw std::runtime_error("⚠ Error: Provided path does not point to a regular file");
    return filePath;
}

void pars_server(const string& filename) 
{
    try 
    {
        normalizeFileContent(filename);
        std::ifstream file(filename.c_str());
        if (!file.is_open())
            throw std::runtime_error("⚠ Error: Failed to open file after normalization: " + filename);

        const string serverDirectives[] = 
        {
            "listen", "server_name", "location",  "error_page", "client_max_body_size"
        };

        std::map<string, DirectiveHandler> directiveHandlers;
        directiveHandlers["listen"] = &handleListenDirective;
        directiveHandlers["server_name"] = &handleServerNameDirective;
        directiveHandlers["location"] = &handleLocationDirective;
      //  directiveHandlers["return"] = &handleReturnDirective;
        directiveHandlers["error_page"] = &handleErrorPageDirective;
        directiveHandlers["client_max_body_size"] = &handleClientMaxBodySizeDirective;

        string line;
        while (std::getline(file, line)) 
        {
            checkLineFormat(line, "server");
            if (!std::getline(file, line))
                throw std::runtime_error("⚠ Error: Expected '{' line after 'server'.");
            checkLineFormat(line, "{");
            int serverBlockDepth = 1;
            while (std::getline(file, line)) 
            {
                std::vector<string> words = splitLine(line);
                if (words.empty())
                    continue;
                const string& directive = words[0];
                if (directive == "}")
                 {
                    if (--serverBlockDepth == 0) 
                    {
                        cout << "✅✅✅✅✅✅✅✅Server block closed successfully.✅✅✅✅✅✅✅" << std::endl;
                        break; // server block end
                    }
                    continue; // handle }
                }
                if (!isAllowedDirective(directive, serverDirectives, sizeof(serverDirectives) / sizeof(serverDirectives[0])))
                    throw std::runtime_error("⚠ Error: Unknown directive: " + directive);

                std::map<string, DirectiveHandler>::iterator it = directiveHandlers.find(directive);
                if (it != directiveHandlers.end())
                    it->second(line, file, serverBlockDepth);
                else
                    cout << "Handling generic directive '" << directive << "': " << line << std::endl;
            }
            if (serverBlockDepth > 0) 
            {
                file.close();
                throw std::runtime_error("⚠ Error: Expected '}' to close 'server' block.");
            }
        }
        file.close();
    } 
    catch (const std::runtime_error& e)
    {
        cout << "⚠ Configuration error: " << e.what() << std::endl;
        throw;
    } 
    catch (const std::exception& e) 
    {
        cout << "⚠ Unexpected error: " << e.what() << std::endl;
        throw;
    }
}

// void pars_config(const string& filename)
// {
//     // try
//     // {
//     //     // string filename = get_filename(argc, argv); 
//     //     pars_server(filename.c_str());
//     // }
//     // catch (const std::runtime_error &e)
//     // {
//     //     cout << e.what() << endl;
//     //     throw; 
//     // }
//     // catch (const std::exception &e)
//     // {
//     //     cout << "Error: An unexpected exception occurred." << endl;
//     //     throw; 
//     // }
// }