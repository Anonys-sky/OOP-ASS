#ifndef FILE_H
#define FILE_H

#include <string>

class File
{
private:
    std::string name;
    std::string extension;

public:
    File();
    File(std::string fileName, std::string fileExtension);

    std::string getName() const;
    std::string getExtension() const;
    std::string getFullName() const;
};

#endif
