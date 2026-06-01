#ifndef FOLDER_H
#define FOLDER_H

#include <string>
#include <vector>
#include "file.h"

class Folder
{
private:
    std::string name;
    Folder* parent;
    std::vector<File> files;
    std::vector<Folder*> subfolders;

public:
    Folder(std::string folderName, Folder* parentFolder);
    ~Folder();

    std::string getName() const;
    Folder* getParent() const;
    std::string getPath() const;

    Folder* findSubfolder(std::string folderName) const;
    bool hasFile(std::string fileName) const;

    Folder* addFolder(std::string folderName);
    void addFile(std::string fileName, std::string extension);

    void displayCurrentFolder() const;
    void displayTree(int level) const;
    int searchFile(std::string fileName) const;

    void deleteFile(std::string fileName);
    void deleteFolder(std::string folderName);
};

#endif
