#include "Folder.h"
#include <iostream>
#include <stdexcept>

using namespace std;

Folder::Folder(string folderName, Folder* parentFolder)
{
    name = folderName;
    parent = parentFolder;
}

Folder::~Folder()
{
    // This loop is recursive because deleting a child folder calls its destructor too.
    for (size_t i = 0; i < subfolders.size(); i++)
    {
        delete subfolders[i];
    }

    subfolders.clear();
}

string Folder::getName() const
{
    return name;
}

Folder* Folder::getParent() const
{
    return parent;
}

string Folder::getPath() const
{
    if (parent == NULL)
    {
        return name;
    }

    return parent->getPath() + "/" + name;
}

Folder* Folder::findSubfolder(string folderName) const
{
    for (size_t i = 0; i < subfolders.size(); i++)
    {
        if (subfolders[i]->getName() == folderName)
        {
            return subfolders[i];
        }
    }

    return NULL;
}

bool Folder::hasFile(string fileName) const
{
    for (size_t i = 0; i < files.size(); i++)
    {
        if (files[i].getName() == fileName || files[i].getFullName() == fileName)
        {
            return true;
        }
    }

    return false;
}

Folder* Folder::addFolder(string folderName)
{
    if (folderName == "")
    {
        throw invalid_argument("Folder name cannot be empty.");
    }

    if (findSubfolder(folderName) != NULL)
    {
        throw runtime_error("A folder with this name already exists here.");
    }

    Folder* newFolder = new Folder(folderName, this);
    subfolders.push_back(newFolder);

    return newFolder;
}

void Folder::addFile(string fileName, string extension)
{
    if (fileName == "")
    {
        throw invalid_argument("File name cannot be empty.");
    }

    if (extension == "")
    {
        throw invalid_argument("File extension cannot be empty.");
    }

    if (hasFile(fileName) || hasFile(fileName + "." + extension))
    {
        throw runtime_error("A file with this name already exists here.");
    }

    File newFile(fileName, extension);
    files.push_back(newFile);
}

void Folder::displayCurrentFolder() const
{
    cout << "\nCurrent folder: " << name << endl;
    cout << "Path: " << getPath() << endl;

    cout << "\nFiles:" << endl;
    if (files.size() == 0)
    {
        cout << "  No files" << endl;
    }
    else
    {
        for (size_t i = 0; i < files.size(); i++)
        {
            cout << "  " << files[i].getFullName() << endl;
        }
    }

    cout << "\nSubfolders:" << endl;
    if (subfolders.size() == 0)
    {
        cout << "  No subfolders" << endl;
    }
    else
    {
        for (size_t i = 0; i < subfolders.size(); i++)
        {
            cout << "  " << subfolders[i]->getName() << endl;
        }
    }
}

void Folder::displayTree(int level) const
{
    for (int i = 0; i < level; i++)
    {
        cout << "  ";
    }
    cout << "[" << name << "]" << endl;

    for (size_t i = 0; i < files.size(); i++)
    {
        for (int j = 0; j < level + 1; j++)
        {
            cout << "  ";
        }
        cout << files[i].getFullName() << endl;
    }

    for (size_t i = 0; i < subfolders.size(); i++)
    {
        subfolders[i]->displayTree(level + 1);
    }
}

int Folder::searchFile(string fileName) const
{
    int matches = 0;

    for (size_t i = 0; i < files.size(); i++)
    {
        if (files[i].getName() == fileName || files[i].getFullName() == fileName)
        {
            cout << "Found: " << getPath() << "/" << files[i].getFullName() << endl;
            matches++;
        }
    }

    for (size_t i = 0; i < subfolders.size(); i++)
    {
        matches += subfolders[i]->searchFile(fileName);
    }

    return matches;
}

void Folder::deleteFile(string fileName)
{
    for (size_t i = 0; i < files.size(); i++)
    {
        if (files[i].getName() == fileName || files[i].getFullName() == fileName)
        {
            files.erase(files.begin() + i);
            return;
        }
    }

    throw runtime_error("File was not found in the current folder.");
}

void Folder::deleteFolder(string folderName)
{
    for (size_t i = 0; i < subfolders.size(); i++)
    {
        if (subfolders[i]->getName() == folderName)
        {
            delete subfolders[i];
            subfolders.erase(subfolders.begin() + i);
            return;
        }
    }

    throw runtime_error("Folder was not found in the current folder.");
}
