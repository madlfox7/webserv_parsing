#include "Parser.hpp"



void check_extension(const std::string& word, const char* extensions[], int num_extensions)
{
    for (int i = 0; i < num_extensions; ++i)
    {
        const char* ext = extensions[i];
        // Check if the word ends with the current extension
        if (word.size() >= strlen(ext) && word.substr(word.size() - strlen(ext)) == ext)
        {
            return; // Valid extension found
        }
    }

    // If no valid extension was found, throw an error
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


void normalizeFileContent(const string& filename) 
{
    std::ifstream file(filename.c_str());
    if (!file)
    {
        cout << "Failed to open file: " << filename << std::endl;
        return;
    }
    string line;
    std::ostringstream buffer;
    bool firstLine = true;
    while (std::getline(file, line)) 
    {
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
    std::ofstream outFile(filename.c_str());
    if (!outFile)
    {
        cout << "Failed to open file for writing: " << filename << std::endl;
        return;
    }
    outFile << buffer.str();
    outFile.close();
}