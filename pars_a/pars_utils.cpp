#include "Parser.hpp"

bool valid_code(const string& return_code)
{
    return  (isValidNumber(return_code, 100, 599));//change range or hardcode vals???
}

// bool valid_code(const string& return_code, const std::vector<string>& strList) 
//  {
//     for (size_t i = 0; i < strList.size(); ++i)
//     {
//         if (return_code == strList[i]) 
//             return true;
//     }
//     return false;
// } //exact match case


bool checkPrefix(const string &line, const string &prefix)
{
    if (line.size() < prefix.size())
        return false;
    for (size_t i = 0; i < prefix.size(); i++)
    {
        if (line[i] != prefix[i])
            return false;
    }
    return true;
}

void checkDirectiveCount(const std::map<string, int>& directiveCounts, const string& blockType)
{
    const string necessaryDirectives_ServerBlock[] = {"listen", "location"};
    const string necessaryDirectives_LocationBlock[] = {"root"};

    const string optionalDirectives_ServerBlock[] = {"server_name", "error_page", "client_max_body_size"};
    const string optionalDirectives_LocationBlock[] = {
        "allow_methods", "return", "autoindex", "upload_dir", "index", "cgi_extension", "upload_dir"};

    const string* necessaryDirectives = NULL;
    const string* optionalDirectives = NULL;
    size_t necessaryCount = 0, optionalCount = 0;

    if (blockType == "ServerBlock")
    {
        necessaryDirectives = necessaryDirectives_ServerBlock;
        optionalDirectives = optionalDirectives_ServerBlock;
        necessaryCount = sizeof(necessaryDirectives_ServerBlock) / sizeof(string);
        optionalCount = sizeof(optionalDirectives_ServerBlock) / sizeof(string);
    }
    else if (blockType == "LocationBlock")
    {
        necessaryDirectives = necessaryDirectives_LocationBlock;
        optionalDirectives = optionalDirectives_LocationBlock;
        necessaryCount = sizeof(necessaryDirectives_LocationBlock) / sizeof(string);
        optionalCount = sizeof(optionalDirectives_LocationBlock) / sizeof(string);
    }
    for (size_t i = 0; i < necessaryCount; ++i)
    {
        const string& directive = necessaryDirectives[i];
        std::map<string, int>::const_iterator it = directiveCounts.find(directive);
        if (it == directiveCounts.end() || it->second < 1)
            throw std::runtime_error("⚠ Error: Necessary directive '" + directive + "' is missing in " + blockType + ".");
    }
    for (size_t i = 0; i < optionalCount; ++i)
    {
        const string& directive = optionalDirectives[i];
        std::map<string, int>::const_iterator it = directiveCounts.find(directive);
        if (it != directiveCounts.end() && it->second > 1)
            throw std::runtime_error("⚠ Error: Optional directive '" + directive + "' appears more than once in " + blockType + ".");
    }
    for (std::map<string, int>::const_iterator it = directiveCounts.begin(); it != directiveCounts.end(); ++it) //DEBUG
        cout << "Directive: " << it->first << " Count: " << it->second << endl;
}

bool isValidNumber(const string& str, int min, int max) 
{
    if (str.empty())
        return false;
    for (size_t i = 0; i < str.size(); i++) 
    {
        if (!std::isdigit(str[i]))
            return false;
    }
    long num = std::atol(str.c_str());
    return num >= min && num <= max;
}

void check_extension(const string& word, const char* extensions[], int num_extensions)
{
    for (int i = 0; i < num_extensions; ++i)
    {
        const char* ext = extensions[i];
        if (word.size() >= strlen(ext) && word.substr(word.size() - strlen(ext)) == ext)
            return; 
    }
    throw std::runtime_error("⚠ Error: expected one of the extensions.");
}

void checkLineFormat(const string& line, const string& expectedString) 
{
    if (line != expectedString) 
        throw std::runtime_error("⚠ Error: Line does not match the expected format: '" + expectedString + "\\n'");
}

bool isAllowedDirective(const string& directive, const string* allowedDirectives, size_t directiveCount) 
{
    for (size_t i = 0; i < directiveCount; ++i) 
    {
        if (directive == allowedDirectives[i]) 
            return true;
    }
    return false;
}

std::vector<string> splitLine(const string& line) 
{
    std::vector<string> words;
    std::istringstream stream(line);
    string word;
    while (stream >> word)
        words.push_back(word);
    return words;
}

void normalizeFileContent(const string &filename)
{
    std::ifstream file(filename.c_str());
    if (!file) 
    {
        cerr << "Failed to open file: " << filename << endl;
        return;
    }
    string line;
    std::ostringstream buffer;
    bool firstLine = true;
    while (std::getline(file, line)) 
    {
        string::size_type hashPos = line.find('#');
        if (hashPos != string::npos)
            line.erase(hashPos);  
        string normalizedLine;
        bool inWord = false;
        for (size_t i = 0; i < line.length(); ++i) 
        {
            if (isspace(line[i]))
             {
                if (inWord) 
                {
                    normalizedLine += ' ';
                    inWord = false;
                }
            } 
            else 
            {
                normalizedLine += line[i];
                inWord = true;
            }
        }
        if (!normalizedLine.empty() && normalizedLine[normalizedLine.length() - 1] == ' ') 
            normalizedLine.erase(normalizedLine.length() - 1);
        if (!normalizedLine.empty()) 
        {
            if (!firstLine) 
                buffer << '\n';  
            else 
                firstLine = false;
            buffer << normalizedLine;
        }
    }
    file.close();
    string tempFilename = filename + ".tmp";
    std::ofstream tempFile(tempFilename.c_str());
    if (!tempFile)
    {
        cerr << "Failed to open temporary file: " << tempFilename << endl;
        return;
    }
    tempFile << buffer.str();
    tempFile.close();
    if (remove(filename.c_str()) != 0) 
    {
        cerr << "Failed to remove original file: " << filename << endl;
        return;
    }
    if (rename(tempFilename.c_str(), filename.c_str()) != 0)
    {
        cerr << "Failed to rename temporary file: " << filename << endl;
        return;
    }
}

// void normalizeFileContent(const string &filename) 
// {
//     std::ifstream file(filename.c_str());
//     if (!file) 
//     {
//         cout << "Failed to open file: " << filename << endl;
//         return;
//     }
//     string line;
//     std::ostringstream buffer;
//     bool firstLine = true;
//     while (std::getline(file, line)) 
//     {
//         string::size_type hashPos = line.find('#');
//         if (hashPos != string::npos) 
//             line.erase(hashPos);
//         string normalizedLine;
//         bool inWord = false;
//         for (size_t i = 0; i < line.length(); ++i) 
//         {
//             if (isspace(line[i])) 
//             {
//                 if (inWord) 
//                 {
//                     normalizedLine += ' ';
//                     inWord = false;
//                 }
//             } 
//             else 
//             {
//                 normalizedLine += line[i];
//                 inWord = true;
//             }
//         }
//         if (!normalizedLine.empty() && normalizedLine[normalizedLine.length() - 1] == ' ') 
//             normalizedLine.erase(normalizedLine.length() - 1);
//         if (!normalizedLine.empty()) 
//         {
//             if (!firstLine) 
//                 buffer << '\n';
//             else 
//                 firstLine = false;
//             buffer << normalizedLine;
//         }
//     }
//     file.close();
//     std::ofstream outFile(filename.c_str());
//     if (!outFile) 
//     {
//         cout << "Failed to open file for writing: " << filename << endl;
//         return;
//     }
//     outFile << buffer.str();
//     outFile.close();
// } // old function ovewrite file 