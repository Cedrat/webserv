#ifndef FORMATTING_HPP
# define FORMATTING_HPP


#include <string>

std::string trimComment( std::string line );
std::string trimStartAndEndWhitespaces( std::string line );
std::string trimDotComa( std::string line );
std::vector<std::string> splitLine( std::string line );

#endif
