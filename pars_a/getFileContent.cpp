#include "Parser.hpp"


static std::map<int, std::pair<std::vector<std::string>, std::vector<std::string> > > serverBlockData;


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
                throw std::runtime_error("⚠ Error: `location` directive has duplicate argument: " + locationArgs[i]);
        }
    }
}

void validateServerBlocks()
{
    for (std::map<int, std::pair<std::vector<std::string>, std::vector<std::string> > >::const_iterator it1 = serverBlockData.begin(); it1 != serverBlockData.end(); ++it1)
    {
        int id1 = it1->first;
        const std::vector<std::string> &listen1 = it1->second.first;
        const std::vector<std::string> &serverNames1 = it1->second.second;

        for (std::map<int, std::pair<std::vector<std::string>, std::vector<std::string> > >::const_iterator it2 = it1; ++it2 != serverBlockData.end();)
        {
            int id2 = it2->first;
            const std::vector<std::string> &listen2 = it2->second.first;
            const std::vector<std::string> &serverNames2 = it2->second.second;
            std::set<std::string> listenSet1(listen1.begin(), listen1.end());
            std::set<std::string> listenSet2(listen2.begin(), listen2.end());
            std::vector<std::string> commonListenPorts;
            for (std::set<std::string>::const_iterator it = listenSet1.begin(); it != listenSet1.end(); ++it)
            {
                if (listenSet2.find(*it) != listenSet2.end())
                    commonListenPorts.push_back(*it);
            }
            if (!commonListenPorts.empty()) //same listen ports 
            {
                std::set<std::string> serverNameSet1(serverNames1.begin(), serverNames1.end());
                std::set<std::string> serverNameSet2(serverNames2.begin(), serverNames2.end());

                std::vector<std::string> commonServerNames;
                for (std::set<std::string>::const_iterator it = serverNameSet1.begin(); it != serverNameSet1.end(); ++it)
                {
                    if (serverNameSet2.find(*it) != serverNameSet2.end())
                        commonServerNames.push_back(*it);
                }
                if (serverNames1.empty() && serverNames2.empty())
                {
                    cerr << "\033[31mError: Server Block " << id1 << " and Server Block " << id2
                              << " have the same listen port(s): ";
                    for (size_t i = 0; i < commonListenPorts.size(); ++i)
                        cerr << commonListenPorts[i] << " ";
                    cerr << "but neither has a server_name.\033[0m" << std::endl;
                    throw std::runtime_error("Conflicting server blocks detected.");
                }
                if (!commonServerNames.empty() && serverNameSet1 == serverNameSet2)
                {
                    cerr << "\033[31mError: Server Block " << id1 << " and Server Block " << id2
                              << " have the same listen port(s): ";
                    for (size_t i = 0; i < commonListenPorts.size(); ++i)
                        cerr << commonListenPorts[i] << " ";
                    cerr << "but also have identical server_name(s): ";
                    for (size_t i = 0; i < commonServerNames.size(); ++i)
                        cerr << commonServerNames[i] << " ";
                    cerr << "\033[0m" << std::endl;
                    throw std::runtime_error("Conflicting server blocks detected.");
                }
            }
        }
    }
}

void storeServerBlockData(int serverBlockId, const std::string &directive, const std::vector<std::string> &args)
{
    if (directive == "listen")
        serverBlockData[serverBlockId].first.insert(serverBlockData[serverBlockId].first.end(), args.begin(), args.end());
    else if (directive == "server_name")
        serverBlockData[serverBlockId].second.insert(serverBlockData[serverBlockId].second.end(), args.begin(), args.end());
    std::cout << "\033[35mStored in Server Block " << serverBlockId << " -> " << directive << ": ";
    for (size_t i = 0; i < args.size(); ++i)
        std::cout << args[i] << " ";
    std::cout << "\033[0m" << std::endl;
}

void handleServerBlock(std::ifstream& file, const std::map<std::string, DirectiveHandler>& directiveHandlers, int serverBlockId) 
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
            throw std::runtime_error("⚠ Error: Unknown directive: " + directive);
        std::map<string, int>::iterator countIt = directiveCounts.find(directive);
        if (countIt != directiveCounts.end()) 
            countIt->second++;
        if (directive == "listen" || directive == "location")
        {
            std::vector<string> args(words.begin() + 1, words.end());
            directiveArgs[directive].insert(directiveArgs[directive].end(), args.begin(), args.end());
        }
         if (directive == "listen" || directive == "server_name") 
        {
            std::vector<std::string> args(words.begin() + 1, words.end());
            storeServerBlockData(serverBlockId, directive, args);
        }
        std::map<string, DirectiveHandler>::const_iterator handlerIt = directiveHandlers.find(directive);
        if (handlerIt != directiveHandlers.end()) 
            handlerIt->second(line, file, serverBlockDepth);
    }
    if (serverBlockDepth > 0) 
        throw std::runtime_error("⚠ Error: Expected '}' to close 'server' block.");
}

void parseServerBlock(const string& filename, const std::map<string, DirectiveHandler>& directiveHandlers)
{
     int serverBlockId = 0;
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
         ++serverBlockId;
         std::cout << "\n--- Entering Server Block " << serverBlockId << " ---\n";
       // handleServerBlock(file, directiveHandlers);
        handleServerBlock(file, directiveHandlers, serverBlockId);
    }
    file.close();
    if (!serverBlockData.empty())
        validateServerBlocks();
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