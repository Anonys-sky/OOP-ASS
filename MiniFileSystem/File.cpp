#include "file.h"

using namespace std;

File::File()
{
    name = "";
    extension = "";
}

File::File(string fileName, string fileExtension)
{
    name = fileName;
    extension = fileExtension;
}

string File::getName() const
{
    return name;
}

string File::getExtension() const
{
    return extension;
}

string File::getFullName() const
{
    return name + "." + extension;
}
