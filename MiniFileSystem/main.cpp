#include "FileSystem.h"
#include <iostream>
#include <stdexcept>

using namespace std;

int main()
{
    FileSystem system;

    try
    {
        system.loadFromFile("filesystem.txt");
        cout << "Initial file structure loaded from filesystem.txt." << endl;
    }
    catch (exception& error)
    {
        cout << "Warning: " << error.what() << endl;
        cout << "The program will still start with an empty Root folder." << endl;
    }

    system.run();

    return 0;
}
