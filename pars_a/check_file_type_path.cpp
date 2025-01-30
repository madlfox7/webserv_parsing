#include "Parser.hpp"


/////check path 
//[

bool isAbsolutePath(const char* path)
{
    if (path == NULL) return false;
    return path[0] == '/';
}

string resolvePath(const char* path)
{
    if (isAbsolutePath(path)) 
        return string(path);
    else
    {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL)
        {
            string fullPath(cwd);
            fullPath += "/";
            fullPath += path;
            return fullPath;
        } 
        else
        {
            cerr << "Error getting current working directory" << endl;
            exit(1);
        }
    }
}


FileType checkFileType(const char* path)
{
    struct stat statbuf;
    if (stat(path, &statbuf) != 0) 
        return FileType_Error;
    if (S_ISREG(statbuf.st_mode)) 
        return FileType_RegularFile;
    if (S_ISDIR(statbuf.st_mode))
        return FileType_Directory;
    if (S_ISCHR(statbuf.st_mode))
        return FileType_CharacterDevice;
    if (S_ISBLK(statbuf.st_mode))
        return FileType_BlockDevice;
    if (S_ISFIFO(statbuf.st_mode))
        return FileType_FIFO;
    if (S_ISLNK(statbuf.st_mode))
        return FileType_SymbolicLink;
    if (S_ISSOCK(statbuf.st_mode))
        return FileType_Socket;
    return FileType_Unknown;
}
