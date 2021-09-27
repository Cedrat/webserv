#include <vector>
#include <string>
#include <iostream>

std::vector<std::string> split_string(std::string str, std::string splitter)
{
    std::vector<std::string> split_str;

    while (str.find(splitter) == 0)
        str.erase(0, splitter.size());
    while (str.size() > 0)
    {
        if (str.find(splitter) != std::string::npos)
        {
            split_str.push_back(str.substr(0, str.find(splitter)));
            str.erase(0, str.find(splitter) + splitter.size());
        }
        else
        {
            split_str.push_back(str.substr(0, str.size()));
            str.erase(0, str.size());
        }
        while (str.find(splitter) == 0)
            str.erase(0, splitter.size());        
    }
    return (split_str);
}

int main(int argc, char **argv)
{
    if (argc < 3)
        return 1;

    std::string str(argv[1]);
    std::string c (argv[2]);

    std::vector<std::string> splitted_string = split_string(str, c);

    for (size_t i = 0; i < splitted_string.size(); i++)
    {
        std::cout << "|" << splitted_string[i] << "|" << std::endl;
    }
}