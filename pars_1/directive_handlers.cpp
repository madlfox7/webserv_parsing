#include "Parser.hpp"

///location_handlers
bool isValidMethod(const string& method) 
{
    return method == "GET" || method == "POST" || method == "DELETE";
}

void handleAllowMethods(const string& line) 
{
    cout << "Handling 'allow_methods': " << line << endl;
    std::vector<string> words = splitLine(line);
    if (words.size() < 2 || words.size() > 4)
        throw std::runtime_error("⚠ Error: Invalid number of arguments for 'allow_methods'.");
    for (size_t i = 1; i < words.size(); ++i) 
    {
        if (!isValidMethod(words[i])) 
            throw std::runtime_error("⚠ Error: Invalid HTTP method. Allowed methods are 'GET', 'POST', 'DELETE'.");
    }
}

void handleReturn(const string& line)  
{
    cout << "Handling 'return': " << line << endl;
    std::vector<string> words = splitLine(line);
    if (words.size() != 3)
        throw std::runtime_error("⚠ Error: Invalid 'return' directive format. Expect two arguments.");
    if (!valid_code(words[1]))
        throw std::runtime_error("⚠ Error: Invalid 'return' http status code");
}

void handleRoot(const string& line)
 {
    cout << "Handling 'root': " << line << endl;
    std::vector<string> words = splitLine(line);
    if (words.size() != 2)
        throw std::runtime_error("⚠ Error: Invalid number of arguments for 'root'. Expected exactly one argument.");
   //arg validation check dir?
}

void handleAutoindex(const string& line) 
{
    cout << "Handling 'autoindex': " << line << endl;
    std::vector<string> words = splitLine(line);
    if (words.size() != 2)
        throw std::runtime_error("⚠ Error: Invalid number of arguments for 'autoindex'. Expected exactly one argument.");
    if (words[1] != "on" && words[1] != "off")  
        throw std::runtime_error("⚠ Error: Invalid argument for 'autoindex'. Allowed values are 'on' or 'off'.");
}

void handleIndex(const string& line) 
{
    cout << "Handling 'index': " << line << endl;
    std::vector<string> words = splitLine(line);
    if (words.size() != 2)
        throw std::runtime_error("⚠ Error: Invalid number of arguments for 'index' directive");
    const char* extensions[] = {".php", ".html"}; 
    check_extension(words[1], extensions, 2);
    return ;
}

void handleCgi(const string & line)
{
    cout << "Handling 'cgi': " << line << endl;
    std::vector<string> words = splitLine(line);
    if (words.size() != 3)
        throw std::runtime_error("⚠ Error: Invalid number of arguments for 'cgi_extension'. Expected exactly two arguments.");
    if (words[1] != ".py" && words[1] != ".sh")   ///  what about "./py"???
        throw std::runtime_error("⚠ Error: Invalid argument for 'autoindex'. Allowed values are 'on' or 'off'."); ////
}

void handleLocation(std::ifstream& file, const string& locationArg, int& serverBlockDepth) 
{
    cout << "Handling 'location': " << locationArg << endl;
    const string locationDirectives[] = 
    {
        "allow_methods", "return", "root", "autoindex", "index", "cgi_extension"
    };
    std::map<string, int> directiveCounts;
    for (size_t i = 0; i < sizeof(locationDirectives) / sizeof(locationDirectives[0]); ++i) 
        directiveCounts[locationDirectives[i]] = 0;
    std::map<string, void(*)(const string&)> locationDirectiveHandlers;
    locationDirectiveHandlers["allow_methods"] = &handleAllowMethods;
    locationDirectiveHandlers["return"] = &handleReturn;
    locationDirectiveHandlers["root"] = &handleRoot;
    locationDirectiveHandlers["autoindex"] = &handleAutoindex;
    locationDirectiveHandlers["index"] = &handleIndex;
    locationDirectiveHandlers["cgi_extension"] = &handleCgi;
    string innerLine;
    while (std::getline(file, innerLine)) 
    {
        if (innerLine == "}") 
        {
            --serverBlockDepth;
             checkDirectiveCount(directiveCounts, "LocationBlock");
            cout << "End of 'location' block." << endl;
            return;
        }
        std::vector<string> words = splitLine(innerLine);
        if (words.empty())
            continue;
        const string& directive = words[0];
        if (!isAllowedDirective(directive, locationDirectives, sizeof(locationDirectives) / sizeof(locationDirectives[0]))) 
            throw std::runtime_error("⚠ Error: Unknown directive in 'location': " + directive);
        std::map<string, int>::iterator countIt = directiveCounts.find(directive);
        if (countIt != directiveCounts.end()) 
            countIt->second++;
        std::map<string, void(*)(const string&)>::iterator it = locationDirectiveHandlers.find(directive);
        if (it != locationDirectiveHandlers.end()) 
            it->second(innerLine); 
    }
    throw std::runtime_error("⚠ Error: Expected '}' to close 'location' block.");
}

//////server_handlers

bool isValidIP(const string& ip)
 {
     if (ip == "localhost")  
        return true;   
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
} //private brodcast ip's //TODO

void handleListenDirective(const string& line, std::ifstream&, int&) 
{    
    cout << "Handling 'listen': " << line << endl;
    std::vector<string> words = splitLine(line);
     if (words.size() != 2)
        throw std::runtime_error("⚠ Error: Invalid 'listen' directive format. Expect exactly one argument.");
    const string& arg = words[1];
    size_t colonPos = arg.find(':');
    if (colonPos != string::npos)
    {
        string ip = arg.substr(0, colonPos);
        string port = arg.substr(colonPos + 1);
        if (!isValidIP(ip) || !isValidNumber(port, 0,  65535))
            throw std::runtime_error("⚠ Error: Invalid IP address or port number.");
    } 
    else
    {
          if (!isValidNumber(arg))
            throw std::runtime_error("⚠ Error: Invalid port number or address.");
    }
}

void handleServerNameDirective(const string& line, std::ifstream&, int&) 
{
    cout << "Handling 'server_name': " << line << endl;
    std::vector<string> words = splitLine(line);
     if (words.size() != 2)
        throw std::runtime_error("⚠ Error: Invalid 'server_name' directive format. Expect exactly one argument.");
}

void handleErrorPageDirective(const string& line, std::ifstream&, int&)
{
    cout << "Handling 'error_page': " << line << endl;
    std::vector<string> words = splitLine(line);
    if (words.size() != 3)
        throw std::runtime_error("⚠ Error: Invalid 'error_page' directive format. Expect two arguments.");
   //  std::vector<std::string> strList = {"404", "508", "200"}; 
   // if (!valid_code(return_code, strList))
    if (!valid_code(words[1]))
        throw std::runtime_error("⚠ Error: Invalid 'error_page' http status code");
}

void handleClientMaxBodySizeDirective(const string& line, std::ifstream&, int&)
{
    cout << "Handling 'client_max_body_size': " << line << endl;
    std::vector<string> words = splitLine(line);
    if (words.size() != 2)
        throw std::runtime_error("⚠ Error: Invalid 'client_max_body_size' directive format. Expect exactly one argument.");
    if (!isValidNumber(words[1]))
         throw std::runtime_error("⚠ Error: Invalid 'client_max_body_size' argument");
}

void handleLocationDirective(const string& line, std::ifstream& file, int& serverBlockDepth) 
{
    std::vector<string> words = splitLine(line);
    if (words.size() != 2)
        throw std::runtime_error("⚠ Error: Directive 'location' must have an argument.");
    //TODO: check location path???
    string nextLine;
    if (!std::getline(file, nextLine) || nextLine != "{")
        throw std::runtime_error("⚠ Error: Expected '{' after 'location' directive.");
    ++serverBlockDepth;
    handleLocation(file, words[1], serverBlockDepth);
}