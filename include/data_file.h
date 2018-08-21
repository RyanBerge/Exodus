#ifndef DATA_FILE_H
#define DATA_FILE_H

#include <sstream>
#include <fstream>
#include <string>
#include <memory>

struct DataKey
{
    std::string key;
    std::shared_ptr<std::stringstream> ss;
};

class DataFile
{
public:
    DataFile();

    bool Open(std::string path);
    bool MoreKeys();
    DataKey GetKey();

private:
    std::fstream file;
    std::string filepath;
};

#endif // DATA_FILE_H
