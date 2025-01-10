#include "Parser.hpp"

//return directive count in 1 server block  = 1 
//client_max_body_size count = 1
//others ???

//TODO:check count of directives 
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
        throw std::runtime_error("⚠ Error: Invalid number of arguments for 'allow_methods'.");
    for (size_t i = 1; i < words.size(); ++i) 
    {
        if (!isValidMethod(words[i])) 
            throw std::runtime_error("⚠ Error: Invalid HTTP method. Allowed methods are 'GET', 'POST', 'DELETE'.");
    }
}

//////////////////////////////////////////////////////////////

void handleReturn(const string& line)  //arg count - ? 
{
    cout << "Handling 'return': " << line << endl;
      std::vector<string> words = splitLine(line);
     if (words.size() != 3)
        throw std::runtime_error("⚠ Error: Invalid 'error_page' directive format. Expect two arguments.");

    // two or one arg is ok? 
    //examples return 403 (Forbidden) || return status_code Uri
    //..........
    //TODO: check first arg numeric and is http response code, ex: 404 ... 
    //return 301 http://example.com ??
    //return 301??
    //return 404 'Not found' ??
    // ...
}

void handleRoot(const string& line)
 {
    cout << "Handling 'root': " << line << endl;
    std::vector<string> words = splitLine(line);
    if (words.size() != 2)
        throw std::runtime_error("⚠ Error: Invalid number of arguments for 'root'. Expected exactly one argument.");
   // .. root arg validation ? 
}


////////AUTOINDEX/////////////////////////////////////////
void handleAutoindex(const string& line) 
{
    cout << "Handling 'autoindex': " << line << endl;
    std::vector<string> words = splitLine(line);
    if (words.size() != 2)
        throw std::runtime_error("⚠ Error: Invalid number of arguments for 'autoindex'. Expected exactly one argument.");
    if (words[1] != "on" && words[1] != "off")  
        throw std::runtime_error("⚠ Error: Invalid argument for 'autoindex'. Allowed values are 'on' or 'off'.");
}
///////////////////////////////////////////////////////


void handleIndex(const string& line) 
{
    cout << "Handling 'index': " << line << endl;
    std::vector<string> words = splitLine(line);
    if (words.size() < 2)
        throw std::runtime_error("⚠ Error: Invalid number of arguments for 'index' directive");
   const char* extensions[] = {".php", ".html"}; // index.php || index.html // index.php.hp case -> normal name = ????
    check_extension(words[1], extensions, 2);
    // arg num - ? "index index.php index.html index.php" ok????
    return ;
}


//////////////////////////////////////////////////////////////

void handleCgi(const string & line)
{
     cout << "Handling 'cgi': " << line << endl;
    std::vector<string> words = splitLine(line);
    if (words.size() != 3)
        throw std::runtime_error("⚠ Error: Invalid number of arguments for 'cgi_extension'. Expected exactly two arguments.");
    if (words[1] != ".py" && words[1] != ".sh")   /// ".py" or "py"? what about "./py" / symbol???
        throw std::runtime_error("⚠ Error: Invalid argument for 'autoindex'. Allowed values are 'on' or 'off'.");
}


void handleLocation(std::ifstream& file, const string& locationArg, int& serverBlockDepth) 
{
    cout << "Handling 'location': " << locationArg << endl;
    const string locationDirectives[] = 
    {
        "allow_methods", "return", "root", "autoindex", "index", "cgi_extension"
    };
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
            cout << "End of 'location' block." << endl;
            return;
        }
        std::vector<string> words = splitLine(innerLine);
        if (words.empty())
            continue;
        const string& directive = words[0];
        if (!isAllowedDirective(directive, locationDirectives, sizeof(locationDirectives) / sizeof(locationDirectives[0]))) 
            throw std::runtime_error("⚠ Error: Unknown directive in 'location': " + directive);
        std::map<string, void(*)(const string&)>::iterator it = locationDirectiveHandlers.find(directive);
        if (it != locationDirectiveHandlers.end()) 
            it->second(innerLine);
        //else 
          //  cout << "Generic handling for directive '" << directive << "' in 'location' block." << endl;
    }
    throw std::runtime_error("⚠ Error: Expected '}' to close 'location' block.");
}

//////server_handlers
///handling listen ///////////////////////////////////////////////////////////////

// bool isValidPort(const string& port) 
// {
//     for (size_t i = 0; i < port.size(); ++i) 
//     {
//         if (!std::isdigit(port[i])) 
//             return false;
//     }
//     int portNum = std::atoi(port.c_str());
//     return portNum > 0 && portNum <= 65535; 
// }

bool isValidIP(const string& ip)
 {
     if (ip == "localhost")  //127.0.0.1:3434 <=> localhost:3434, listen localhost (port 80 by default).. 
        return true;   // syntax listen ip:port
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

//bool isValidNumber(const string& str, int min, int max) 
void handleListenDirective(const string& line, std::ifstream&, int&) 
{    
    //in class we must have variable and function to check listen_count < 0 || > (max_listen_size?) throw err
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
        if (!isValidIP(ip) || !isValidNumber(arg, 0,  65535))
            throw std::runtime_error("⚠ Error: Invalid IP address or port number.");
    } 
    else
    {
          if (!isValidNumber(arg, 0,  65535))
            throw std::runtime_error("⚠ Error: Invalid port number or address.");
    }
}

////////////////////////////////////////////////////////////////////////////////

void handleServerNameDirective(const string& line, std::ifstream&, int&) //num  1 < args < ? server_name google.com google.org ... ok???
{
       cout << "Handling 'server_name': " << line << endl;
        std::vector<string> words = splitLine(line);
     if (words.size() != 2)
        throw std::runtime_error("⚠ Error: Invalid 'server_name' directive format. Expect exactly one argument.");
    //.........
    //check .com .org extensions ... 
    // server name.com  localhost ? ??
    //server name defined by ip?? server_name 127.0.0.1;
    //can have many args or 1?
}


// void handleReturnDirective(const string& line, std::ifstream&, int&) //arg num = ?
// {
//     cout << "Handling 'return': " << line << endl;
//      std::vector<string> words = splitLine(line);
//      if (words.size() != 3)
//         throw std::runtime_error("⚠ Error: Invalid 'error_page' directive format. Expect two arguments.");

//     // two or one arg is ok? 
//     //examples return 403 (Forbidden) || return status_code Uri
//     //..........
// }

void handleErrorPageDirective(const string& line, std::ifstream&, int&)//arg num =?
{
    cout << "Handling 'error_page': " << line << endl;
    // at least 2 arg, more???
    //error_page code uri
    //error_page code1 code2 code3 same_uri???
    //error_page 404 /404.html;
    //error_page 500 502 503 /50x.html; ????
    //......
}

/////////////////////////////////////////////////////////////////////
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
    if (words.size() < 2)
        throw std::runtime_error("⚠ Error: Directive 'location' must have an argument.");
    string nextLine;
    if (!std::getline(file, nextLine) || nextLine != "{")
        throw std::runtime_error("⚠ Error: Expected '{' after 'location' directive.");
    ++serverBlockDepth;
    handleLocation(file, words[1], serverBlockDepth);
}