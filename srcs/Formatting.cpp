#include "../includes/Formatting.hpp"
#include "../includes/utils.hpp"

std::string trimComment( std::string line )
{
    std::string newLine;
    size_t      commentPos;

    commentPos = line.find("#");
    if (commentPos == std::string::npos)
        return line;
    else
        newLine = line.substr(0, commentPos);

    return newLine;
}

std::string trimStartAndEndWhitespaces( std::string line )
{
    std::string newLine;
    size_t      whitePos;

    whitePos = line.find_first_not_of(" \n\t\r\f\v");
    if (whitePos != std::string::npos)
        newLine = line.substr(whitePos);

    whitePos = newLine.find_last_not_of(" \n\t\r\f\v");
    if (whitePos != std::string::npos)
        newLine = newLine.substr(0, whitePos + 1);

    return newLine;
}

std::string trimDotComa( std::string line )
{
    std::string newLine;
    size_t      signPos;
    size_t      accoladePos;

    if ((accoladePos = line.find("{")) != std::string::npos || (accoladePos = line.find("}")) != std::string::npos)
        return line;

    signPos = line.find(";");
    if (!line.empty() && signPos == std::string::npos)
        throw ("Error : Line need to be terminated with a single ';'");
    else if (line[signPos + 1])
        throw ("Error : Line need to be terminated with a single ';'");
    else
        newLine = trimStartAndEndWhitespaces(line.substr(0, signPos));
    
    return newLine;
}

std::vector<std::string> splitLine( std::string line )
{
    //Renvoie vector<string> avec les éléments découpés selon les espaces
    std::vector<std::string>    formattedLine; 
    size_t                      start = 0, end = 0;

    end = line.find_first_of(" \n\t\r\f\v");
    if (end == std::string::npos)
    {
        formattedLine.push_back(line);
        return formattedLine;
    }
    while (end != std::string::npos)
    {
        formattedLine.push_back(line.substr(start, (end - start)));
        if (line[end + 1] == ' ')
        {
            start = line.find_first_not_of(" \n\t\r\f\v", end + 1);
            end = line.find_first_of(" \n\t\r\f\v", start + 1);
        }
        else
        {        
            start = end + 1;
            end = line.find_first_of(" \n\t\r\f\v", end + 1);
        }
    }
    formattedLine.push_back(line.substr(start, line.size()));

    return formattedLine;
}