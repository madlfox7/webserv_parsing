# ifndef PARSER_HPP
# define PARSER_HPP

#include <vector>
#include <iostream>
#include <exception>
#include <fcntl.h>
#include <unistd.h>
#include <cctype>
#include <string>
#include <stdexcept>
#include <fstream>
#include <cstdlib> 
#include <cstring> 
#include <sys/stat.h> 
#include <map>
#include <sstream> 

#define BUFFER_SIZE 1024
#define DEFAULT_FILENAME "default_conf"

using namespace std;

enum FileType
{
    FileType_RegularFile,
    FileType_Directory,
    FileType_CharacterDevice,
    FileType_BlockDevice,
    FileType_FIFO,
    FileType_SymbolicLink,
    FileType_Socket,
    FileType_Unknown,
    FileType_Error
};

//getting and checking file path 
bool isAbsolutePath(const char* path);
string resolvePath(const char* path);
FileType checkFileType(const char* path);

//getting content and cleaning up
string get_filename(int argc, char **argv);
void pars_config(int argc, char **argv);
// string remove_comments(const string &content);

//pars_utils.cpp

void normalizeFileContent(const string& filename) ;
bool isAllowedDirective(const string& directive, const string* allowedDirectives, size_t directiveCount) ;
void checkLineFormat(const string& line, const string& expectedString) ;
std::vector<string> splitLine(const string& line) ;

//pars_handlers.cpp
typedef void (*DirectiveHandler)(const string&, std::ifstream&, int&);
void handleLocation(std::ifstream& file, const string& locationArg, int& serverBlockDepth);
void handleListenDirective(const string& line, std::ifstream&, int&) ;
void handleServerNameDirective(const string& line, std::ifstream&, int&) ;
void handleLocationDirective(const string& line, std::ifstream& file, int& serverBlockDepth) ;
void handleReturnDirective(const string& line, std::ifstream&, int&) ;
void handleErrorPageDirective(const string& line, std::ifstream&, int&);
void handleClientMaxBodySizeDirective(const string& line, std::ifstream&, int&);


#endif