#include "../includes/utils.hpp"



std::string create_tmp_file_with_extension(std::string extension)
{
    std::string str_tmp_file_name("tmpXXXXXX");
    
    if (extension != "")
        str_tmp_file_name = str_tmp_file_name +  "." + extension ;
    
    char *_tmpFileName;

    _tmpFileName = const_cast<char *>(str_tmp_file_name.c_str());
    
    mkstemps(_tmpFileName, extension.size() + 1);

    return _tmpFileName;
}

std::string create_tmp_file()
{
    char _tmpFileName[] = "tmpXXXXXX";
    mkstemp(_tmpFileName);

    return _tmpFileName;
}