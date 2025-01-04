#include "Parser.hpp"

///location_handlers

//////ALLOW METHODS/////////////////////
bool isValidMethod(const string& method) 
{
    return method == "GET" || method == "POST" || method == "DELETE";
}

void handleAllowMethods(const string& line) 
{
    cout << "Handling 'allow_methods': " << line << endl;
    std::vector<string> words = splitLine(line);
    if (words.size() < 2 || words.size() > 4)
        throw std::runtime_error("Error: Invalid number of arguments for 'allow_methods'.");
    for (size_t i = 1; i < words.size(); ++i) 
    {
        if (!isValidMethod(words[i])) 
            throw std::runtime_error("Error: Invalid HTTP method. Allowed methods are 'GET', 'POST', 'DELETE'.");
    }
}

//////////////////////////////////////////////////////////////

void handleReturn(const string& line) 
{
    cout << "Handling 'return': " << line << endl;
}

void handleRoot(const string& line)
 {
    cout << "Handling 'root': " << line << endl;
}


////////AUTOINDEX/////////////////////////////////////////
void handleAutoindex(const string& line) 
{
    cout << "Handling 'autoindex': " << line << endl;
    std::vector<string> words = splitLine(line);
    if (words.size() != 2)
        throw std::runtime_error("Error: Invalid number of arguments for 'autoindex'. Expected exactly one argument.");
    if (words[1] != "on" && words[1] != "off")  
        throw std::runtime_error("Error: Invalid argument for 'autoindex'. Allowed values are 'on' or 'off'.");
}
///////////////////////////////////////////////////////

void handleIndex(const string& line) 
{
    cout << "Handling 'index': " << line << endl;
}

void handleLocation(std::ifstream& file, const string& locationArg, int& serverBlockDepth) 
{
    cout << "Handling 'location': " << locationArg << endl;
    const string locationDirectives[] = 
    {
        "allow_methods", "return", "root", "autoindex", "index"
    };

    std::map<string, void(*)(const string&)> locationDirectiveHandlers;
    locationDirectiveHandlers["allow_methods"] = &handleAllowMethods;
    locationDirectiveHandlers["return"] = &handleReturn;
    locationDirectiveHandlers["root"] = &handleRoot;
    locationDirectiveHandlers["autoindex"] = &handleAutoindex;
    locationDirectiveHandlers["index"] = &handleIndex;

    string innerLine;
    while (std::getline(file, innerLine)) 
    {
        if (innerLine == "}") 
        {
            --serverBlockDepth;
            cout << "End of 'location' block." << endl;
            return;
        }
        std::vector<string> words = splitLine(innerLine);
        if (words.empty())
            continue;
        const string& directive = words[0];
        if (!isAllowedDirective(directive, locationDirectives, sizeof(locationDirectives) / sizeof(locationDirectives[0]))) 
            throw std::runtime_error("Unknown directive in 'location': " + directive);
        std::map<string, void(*)(const string&)>::iterator it = locationDirectiveHandlers.find(directive);
        if (it != locationDirectiveHandlers.end()) 
            it->second(innerLine);
        else 
            cout << "Generic handling for directive '" << directive << "' in 'location' block." << endl;
    }
    throw std::runtime_error("Expected '}' to close 'location' block.");
}

//////server_handlers
///handling listen ///////////////////////////////////////////////////////////////

bool isValidPort(const string& port) 
{
    for (size_t i = 0; i < port.size(); ++i) 
    {
        if (!std::isdigit(port[i])) 
            return false;
    }
    int portNum = std::atoi(port.c_str());
    return portNum > 0 && portNum <= 65535;
}

bool isValidIP(const string& ip)
 {
    std::istringstream iss(ip);
    string byte;
    int byteCount = 0;
    while (std::getline(iss, byte, '.')) 
    {
        if (byte.empty() || byte.size() > 3)
            return false;
        for (string::const_iterator it = byte.begin(); it != byte.end(); ++it) 
        {
            if (!std::isdigit(*it))
                return false;
        }
        int num = std::atoi(byte.c_str());
        if (num < 0 || num > 255)
            return false;
        ++byteCount;
    }
    return byteCount == 4; 
}


void handleListenDirective(const string& line, std::ifstream&, int&) 
{    
    //review and implement later
    //in class we must have variable and function to check listen_count < 0 || > (max_listen_size?) throw err
    cout << "Handling 'listen': " << line << endl;
    std::vector<string> words = splitLine(line);
     if (words.size() != 2)
        throw std::runtime_error("Error: Invalid 'listen' directive format. Expect exactly one argument.");
    const string& arg = words[1];
    size_t colonPos = arg.find(':');
    if (colonPos != string::npos)
    {
        string ip = arg.substr(0, colonPos);
        string port = arg.substr(colonPos + 1);
        if (!isValidIP(ip) || !isValidPort(port)) 
            throw std::runtime_error("Error: Invalid IP address or port number.");
    } 
    else
    {
        if (!isValidPort(arg))
            throw std::runtime_error("Error: Invalid port number.");
    }
}

////////////////////////////////////////////////////////////////////////////////

void handleServerNameDirective(const string& line, std::ifstream&, int&) 
{
       cout << "Handling 'server_name': " << line << endl;
}


void handleReturnDirective(const string& line, std::ifstream&, int&) 
{
    cout << "Handling 'return': " << line << endl;
}

void handleErrorPageDirective(const string& line, std::ifstream&, int&)
{
    cout << "Handling 'error_page': " << line << endl;
}

void handleClientMaxBodySizeDirective(const string& line, std::ifstream&, int&)
{
    cout << "Handling 'client_max_body_size': " << line << endl;
}

void handleLocationDirective(const string& line, std::ifstream& file, int& serverBlockDepth) 
{
    std::vector<string> words = splitLine(line);
    if (words.size() < 2)
        throw std::runtime_error("Directive 'location' must have an argument.");
    string nextLine;
    if (!std::getline(file, nextLine) || nextLine != "{")
        throw std::runtime_error("Expected '{' after 'location' directive.");
    ++serverBlockDepth;
    handleLocation(file, words[1], serverBlockDepth);
}