#include "FileSystem.h"
#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>

using namespace std;

FileSystem::FileSystem()
{
    root = new Folder("Root", NULL);
    currentFolder = root;
}

FileSystem::~FileSystem()
{
    delete root;
}

string FileSystem::trim(string text) const
{
    int start = 0;
    int end = static_cast<int>(text.length()) - 1;

    while (start <= end && (text[start] == ' ' || text[start] == '\t'))
    {
        start++;
    }

    while (end >= start && (text[end] == ' ' || text[end] == '\t' || text[end] == '\r'))
    {
        end--;
    }

    if (start > end)
    {
        return "";
    }

    return text.substr(start, end - start + 1);
}

vector<string> FileSystem::splitPath(string path) const
{
    vector<string> parts;
    string currentPart = "";

    for (size_t i = 0; i < path.length(); i++)
    {
        if (path[i] == '/')
        {
            if (currentPart != "")
            {
                parts.push_back(currentPart);
                currentPart = "";
            }
        }
        else
        {
            currentPart += path[i];
        }
    }

    if (currentPart != "")
    {
        parts.push_back(currentPart);
    }

    return parts;
}

void FileSystem::splitFileName(string completeName, string& fileName, string& extension) const
{
    int dotPosition = -1;

    for (int i = static_cast<int>(completeName.length()) - 1; i >= 0; i--)
    {
        if (completeName[i] == '.')
        {
            dotPosition = i;
            break;
        }
    }

    if (dotPosition <= 0 || dotPosition == static_cast<int>(completeName.length()) - 1)
    {
        throw invalid_argument("File entry must include a name and an extension.");
    }

    fileName = completeName.substr(0, dotPosition);
    extension = completeName.substr(dotPosition + 1);
}

Folder* FileSystem::getOrCreateFolder(vector<string> parts, int lastFolderIndex)
{
    if (parts.size() == 0 || parts[0] != root->getName())
    {
        throw runtime_error("Every path in filesystem.txt must start with Root.");
    }

    Folder* folder = root;

    for (int i = 1; i <= lastFolderIndex; i++)
    {
        Folder* nextFolder = folder->findSubfolder(parts[i]);

        if (nextFolder == NULL)
        {
            nextFolder = folder->addFolder(parts[i]);
        }

        folder = nextFolder;
    }

    return folder;
}

void FileSystem::loadFolderPath(string path)
{
    vector<string> parts = splitPath(path);

    if (parts.size() == 0)
    {
        throw runtime_error("A folder path in filesystem.txt is empty.");
    }

    getOrCreateFolder(parts, static_cast<int>(parts.size()) - 1);
}

void FileSystem::loadFilePath(string path, string extensionFromLine)
{
    vector<string> parts = splitPath(path);

    if (parts.size() < 2)
    {
        throw runtime_error("A file path must include Root and a file name.");
    }

    Folder* folder = getOrCreateFolder(parts, static_cast<int>(parts.size()) - 2);
    string fileName = parts[parts.size() - 1];
    string extension = trim(extensionFromLine);

    if (extension == "")
    {
        splitFileName(fileName, fileName, extension);
    }
    else if (extension[0] == '.')
    {
        extension = extension.substr(1);
    }

    folder->addFile(fileName, extension);
}

void FileSystem::loadFromFile(string fileName)
{
    ifstream inputFile(fileName.c_str());

    if (!inputFile)
    {
        throw runtime_error("Could not open " + fileName + ".");
    }

    string type;
    string path;

    while (inputFile >> type)
    {
        if (type == "FOLDER")
        {
            inputFile >> path;
            loadFolderPath(path);
        }
        else if (type == "FILE")
        {
            string restOfLine;
            inputFile >> path;
            getline(inputFile, restOfLine);
            loadFilePath(path, trim(restOfLine));
        }
        else
        {
            string restOfLine;
            getline(inputFile, restOfLine);
        }
    }
}

void FileSystem::showMenu() const
{
    cout << "\n========== Mini File System Explorer ==========" << endl;
    cout << "1. Create Folder" << endl;
    cout << "2. Create File" << endl;
    cout << "3. Display Current Folder" << endl;
    cout << "4. Display Full Folder Tree" << endl;
    cout << "5. Search File" << endl;
    cout << "6. Enter Folder" << endl;
    cout << "7. Go Back to Parent Folder" << endl;
    cout << "8. Delete File" << endl;
    cout << "9. Delete Folder" << endl;
    cout << "10. Show Current Path" << endl;
    cout << "11. Exit" << endl;
    cout << "Choose an option: ";
}

int FileSystem::getMenuChoice() const
{
    int choice;
    cin >> choice;

    if (cin.fail())
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        throw invalid_argument("Please enter a number from 1 to 11.");
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return choice;
}

string FileSystem::askForText(string message) const
{
    string text;

    cout << message;
    getline(cin, text);

    return trim(text);
}

void FileSystem::run()
{
    int choice = 0;

    while (choice != 11)
    {
        try
        {
            showMenu();
            choice = getMenuChoice();

            if (choice == 1)
            {
                string folderName = askForText("Enter folder name: ");
                currentFolder->addFolder(folderName);
                cout << "Folder created." << endl;
            }
            else if (choice == 2)
            {
                string fileName = askForText("Enter file name without extension: ");
                string extension = askForText("Enter extension without dot: ");

                if (extension != "" && extension[0] == '.')
                {
                    extension = extension.substr(1);
                }

                currentFolder->addFile(fileName, extension);
                cout << "File created." << endl;
            }
            else if (choice == 3)
            {
                currentFolder->displayCurrentFolder();
            }
            else if (choice == 4)
            {
                cout << "\nFull Folder Tree:" << endl;
                root->displayTree(0);
            }
            else if (choice == 5)
            {
                string fileName = askForText("Enter file name to search: ");
                int matches = root->searchFile(fileName);

                if (matches == 0)
                {
                    cout << "File was not found." << endl;
                }
            }
            else if (choice == 6)
            {
                string folderName = askForText("Enter folder name to enter: ");
                Folder* nextFolder = currentFolder->findSubfolder(folderName);

                if (nextFolder == NULL)
                {
                    throw runtime_error("Folder was not found in the current folder.");
                }

                currentFolder = nextFolder;
                cout << "Moved to " << currentFolder->getPath() << endl;
            }
            else if (choice == 7)
            {
                if (currentFolder->getParent() == NULL)
                {
                    throw runtime_error("You are already at Root.");
                }

                currentFolder = currentFolder->getParent();
                cout << "Moved to " << currentFolder->getPath() << endl;
            }
            else if (choice == 8)
            {
                string fileName = askForText("Enter file name to delete: ");
                currentFolder->deleteFile(fileName);
                cout << "File deleted." << endl;
            }
            else if (choice == 9)
            {
                string folderName = askForText("Enter folder name to delete: ");
                currentFolder->deleteFolder(folderName);
                cout << "Folder and its contents deleted." << endl;
            }
            else if (choice == 10)
            {
                cout << "Current path: " << currentFolder->getPath() << endl;
            }
            else if (choice == 11)
            {
                cout << "Goodbye!" << endl;
            }
            else
            {
                throw invalid_argument("Please enter a number from 1 to 11.");
            }
        }
        catch (exception& error)
        {
            cout << "Error: " << error.what() << endl;
        }
    }
}
