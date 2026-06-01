#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>
#include <vector>
#include "Folder.h"

class FileSystem
{
private:
    Folder* root;
    Folder* currentFolder;

    std::string trim(std::string text) const;
    std::vector<std::string> splitPath(std::string path) const;
    void splitFileName(std::string completeName, std::string& fileName, std::string& extension) const;

    Folder* getOrCreateFolder(std::vector<std::string> parts, int lastFolderIndex);
    void loadFolderPath(std::string path);
    void loadFilePath(std::string path, std::string extensionFromLine);

    void showMenu() const;
    int getMenuChoice() const;
    std::string askForText(std::string message) const;

public:
    FileSystem();
    ~FileSystem();

    void loadFromFile(std::string fileName);
    void run();
};

#endif
