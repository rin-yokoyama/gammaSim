#ifndef __LINUX_DIRECTORY_FUNCTIONS__
#define __LINUX_DIRECTORY_FUNCTIONS__

#include <iostream>
#include <sys/stat.h> // For mkdir()
#include <sys/types.h>
#include <dirent.h> // For directory traversal
#include <unistd.h> // For unlink()
#include <cstring>  // For strerror()
#include <cstdio>   // For remove()

// Function to check if a directory exists
bool directoryExists(const char *path)
{
    struct stat info;
    if (stat(path, &info) != 0)
    {
        return false; // Cannot access the directory
    }
    else if (info.st_mode & S_IFDIR)
    {
        return true; // It's a directory
    }
    return false;
}

// Function to create a directory
bool createDirectory(const char *path)
{
    if (mkdir(path, 0777) == -1)
    {
        if (errno != EEXIST)
        { // Directory already exists is not an error
            std::cerr << "Error creating directory: " << strerror(errno) << std::endl;
            return false;
        }
    }
    return true;
}

// Function to remove all files in a directory
bool removeFilesInDirectory(const char *path)
{
    DIR *dir;
    struct dirent *entry;
    char filepath[1024];

    if ((dir = opendir(path)) != NULL)
    {
        while ((entry = readdir(dir)) != NULL)
        {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
            {
                snprintf(filepath, sizeof(filepath), "%s/%s", path, entry->d_name);
                if (remove(filepath) != 0)
                {
                    std::cerr << "Error removing file: " << strerror(errno) << std::endl;
                    closedir(dir);
                    return false;
                }
            }
        }
        closedir(dir);
    }
    else
    {
        std::cerr << "Error opening directory: " << strerror(errno) << std::endl;
        return false;
    }
    return true;
}

bool createOrCleanDirectory(std::string dirPath)
{
    if (!directoryExists(dirPath.c_str()))
    {
        // Directory does not exist, create it
        if (createDirectory(dirPath.c_str()))
        {
            std::cout << "Directory created: " << dirPath << std::endl;
            return true;
        }
    }
    else
    {
        // Directory exists, remove all files inside it
        if (removeFilesInDirectory(dirPath.c_str()))
        {
            std::cout << "All files removed from directory: " << dirPath << std::endl;
            return true;
        }
    }
    std::cout << "Failed to create or clean directory: " << dirPath << std::endl;
    return false;
}

#endif