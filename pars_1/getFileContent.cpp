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

void checkListenAndLocationArgs(const std::map<string, std::vector<string> > & directiveArgs)
{
    if (directiveArgs.find("listen") != directiveArgs.end())
    {
        const std::vector<string>& listenArgs = directiveArgs.at("listen");
        std::set<string> uniqueListenArgs;
        for (size_t i = 0; i < listenArgs.size(); ++i)
        {
            if (!uniqueListenArgs.insert(listenArgs[i]).second) 
                throw std::runtime_error("⚠ Error: `listen` directive has duplicate argument: " + listenArgs[i]);
        }
    }
    if (directiveArgs.find("location") != directiveArgs.end())
    {
        const std::vector<string>& locationArgs = directiveArgs.at("location");
        std::set<string> uniqueLocationArgs;
        for (size_t i = 0; i < locationArgs.size(); ++i)
        {
            if (!uniqueLocationArgs.insert(locationArgs[i]).second) 
            {
                throw std::runtime_error("⚠ Error: `location` directive has duplicate argument: " + locationArgs[i]);
            }
        }
    }

    // if (directiveArgs.find("listen") != directiveArgs.end() &&
    //     directiveArgs.find("location") != directiveArgs.end())
    // {
    //     const std::vector<string>& listenArgs = directiveArgs.at("listen");
    //     const std::vector<string>& locationArgs = directiveArgs.at("location");

    //     for (size_t i = 0; i < listenArgs.size(); ++i)
    //     {
    //         for (size_t j = 0; j < locationArgs.size(); ++j)
    //         {
    //             if (listenArgs[i] == locationArgs[j])
    //             {
    //                 throw std::runtime_error("⚠ Error: `listen` and `location` directives have the same argument: " + listenArgs[i]);
    //             }
    //         }
    //     }
    // }
}




// void handleServerBlock(std::ifstream& file, const std::map<string, DirectiveHandler>& directiveHandlers) 
// {
//     std::map<string, int> directiveCounts;
//     const string serverDirectives[] = { "listen", "server_name", "location", "error_page", "client_max_body_size" };
//     for (size_t i = 0; i < sizeof(serverDirectives) / sizeof(serverDirectives[0]); ++i) 
//         directiveCounts[serverDirectives[i]] = 0;
//     string line;
//    std::map<string, std::vector<string> > directiveArgs; 

//     int serverBlockDepth = 1;
//     while (std::getline(file, line))
//     {
//         std::vector<string> words = splitLine(line);
//         if (words.empty())
//             continue;
//         const string& directive = words[0];
//         if (directive == "}")
//         {
//             if (--serverBlockDepth == 0)
//             {
//                 cout << "Server block closed successfully!!!!" << endl;
//                 checkDirectiveCount(directiveCounts, "ServerBlock");
//                 checkListenAndLocationArgs(directiveArgs);
//                 // add a case when neccessery directive is present but has same arg location and listen dir
//                 break; // Server block ends
//             }
//             continue; // Handle }
//         }
//         if (!isAllowedDirective(directive, serverDirectives, sizeof(serverDirectives) / sizeof(serverDirectives[0]))) 
//             throw std::runtime_error("⚠ Error: Unknown directive: " + directive);
//         std::map<string, int>::iterator countIt = directiveCounts.find(directive);
//         if (countIt != directiveCounts.end()) 
//             countIt->second++;
//         std::map<string, DirectiveHandler>::const_iterator handlerIt = directiveHandlers.find(directive);
//         if (handlerIt != directiveHandlers.end()) 
//             handlerIt->second(line, file, serverBlockDepth);
//     }
//     if (serverBlockDepth > 0) 
//         throw std::runtime_error("⚠ Error: Expected '}' to close 'server' block.");
// } //old


void handleServerBlock(std::ifstream& file, const std::map<string, DirectiveHandler>& directiveHandlers) 
{
    std::map<string, int> directiveCounts;
    std::map<string, std::vector<string> > directiveArgs; 

    const string serverDirectives[] = {"listen", "server_name", "location", "error_page", "client_max_body_size"};
    for (size_t i = 0; i < sizeof(serverDirectives) / sizeof(serverDirectives[0]); ++i) 
        directiveCounts[serverDirectives[i]] = 0;

    string line;
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
                cout << "Server block closed successfully!!!!" << endl;
                checkDirectiveCount(directiveCounts, "ServerBlock");
                checkListenAndLocationArgs(directiveArgs);
                break; // Server block ends
            }
            continue;
        }
        if (!isAllowedDirective(directive, serverDirectives, sizeof(serverDirectives) / sizeof(serverDirectives[0]))) 
        {
            throw std::runtime_error("⚠ Error: Unknown directive: " + directive);
        }
        std::map<string, int>::iterator countIt = directiveCounts.find(directive);
        if (countIt != directiveCounts.end()) 
        {
            countIt->second++;
        }
        if (directive == "listen" || directive == "location")
        {
            std::vector<string> args(words.begin() + 1, words.end());
            directiveArgs[directive].insert(directiveArgs[directive].end(), args.begin(), args.end());
        }
        std::map<string, DirectiveHandler>::const_iterator handlerIt = directiveHandlers.find(directive);
        if (handlerIt != directiveHandlers.end()) 
            handlerIt->second(line, file, serverBlockDepth);
    }
    if (serverBlockDepth > 0) 
    {
        throw std::runtime_error("⚠ Error: Expected '}' to close 'server' block.");
    }
}


// void handleServerBlock(std::ifstream& file, const std::map<string, DirectiveHandler>& directiveHandlers) 
// {
//     (void)direct
//     std::map<string, int> directiveCounts;
//     std::map<string, std::vector<string> > directiveArgs; // To store arguments for each directive

//     const string serverDirectives[] = {"listen", "server_name", "location", "error_page", "client_max_body_size"};
//     for (size_t i = 0; i < sizeof(serverDirectives) / sizeof(serverDirectives[0]); ++i)
//         directiveCounts[serverDirectives[i]] = 0;

//     string line;
//     int serverBlockDepth = 1;

//     while (std::getline(file, line))
//     {
//         std::vector<string> words = splitLine(line);
//         if (words.empty())
//             continue;

//         const string& directive = words[0];

//         if (directive == "}")
//         {
//             if (--serverBlockDepth == 0)
//             {
//                 cout << "Server block closed successfully!!!!" << endl;
//                 checkDirectiveCount(directiveCounts, "ServerBlock");
//                 checkListenAndLocationArgs(directiveArgs);
//                 break; 
//             }
//         }
//         else if (directive == "{")
//         {
//             ++serverBlockDepth;
//         }
//         else
//         {
//             if (directiveCounts.find(directive) != directiveCounts.end())
//                 ++directiveCounts[directive];
//             if (directive == "listen" || directive == "location")
//             {
//                 std::vector<string> args(words.begin() + 1, words.end());
//                 directiveArgs[directive].insert(directiveArgs[directive].end(), args.begin(), args.end());
//             }
//         }
//     }
// }

void parseServerBlock(const string& filename, const std::map<string, DirectiveHandler>& directiveHandlers)
{
    std::ifstream file(filename.c_str());
    if (!file.is_open()) 
        throw std::runtime_error("⚠ Error: Failed to open file after normalization: " + filename);
    string line;
    while (std::getline(file, line))
    {
        checkLineFormat(line, "server");
        if (!std::getline(file, line)) 
            throw std::runtime_error("⚠ Error: Expected '{' line after 'server'.");
        checkLineFormat(line, "{");
        handleServerBlock(file, directiveHandlers);
    }
    file.close();
}

void pars_server(const string& filename)
{
    try 
    {
        normalizeFileContent(filename);
        std::map<string, DirectiveHandler> directiveHandlers;
        directiveHandlers["listen"] = &handleListenDirective;
        directiveHandlers["server_name"] = &handleServerNameDirective;
        directiveHandlers["location"] = &handleLocationDirective;
        directiveHandlers["error_page"] = &handleErrorPageDirective;
        directiveHandlers["client_max_body_size"] = &handleClientMaxBodySizeDirective;
        parseServerBlock(filename, directiveHandlers);
    } 
    catch (const std::runtime_error& e)
    {
        cout << "⚠ Configuration error: " << e.what() << endl;
        throw;
    } catch (const std::exception& e)
    {
        cout << "⚠ Unexpected error: " << e.what() << endl;
        throw;
    }
}