#ifndef FORMATTING_HPP
# define FORMATTING_HPP


# include <string>
# include <vector>

std::string trimComment( std::string line );
std::string trimStartAndEndWhitespaces( std::string line );
std::string trimDotComa( std::string line );
std::vector<std::string> splitLine( std::string line );
std::string createRandomFileName(std::string path);


#endif
