#include "data_file.h"

#include <iostream>

DataFile::DataFile()
{
}

bool DataFile::Open(std::string path)
{
    filepath = path;
    file.open(filepath);

    if (!file.is_open())
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool DataFile::MoreKeys()
{
    return !file.eof();
}

DataKey DataFile::GetKey()
{
    std::string line("");

    while (!file.eof() && line == "")
    {
        std::getline(file, line);
    }

    if (file.eof() && line == "")
    {
        return DataKey{"", std::shared_ptr<std::stringstream>()};
    }

    auto sit = line.begin();
    std::string key;
    while (sit != line.end() && *sit != ':')
    {
        key += *sit;
        ++sit;
    }

    while (sit != line.end() && (*sit == ' ' || *sit == ':'))
    {
        ++sit;
    }

    if (sit == line.end())
    {
        std::cerr << "Exodus: Malformed data file: " << filepath << std::endl;
        return DataKey{"", std::shared_ptr<std::stringstream>()};
    }

    DataKey data_key;
    data_key.key = key;
    data_key.ss = std::shared_ptr<std::stringstream>(new std::stringstream(std::string(sit, line.end())));
    return data_key;
}
