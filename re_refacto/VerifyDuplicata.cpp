#include "VerifyDuplicata.hpp"

VerifyDuplicata::VerifyDuplicata(std::string request) : _nb_host(0), _nb_content_length(0)
{
    std::vector<std::string> lines;

    lines = split_string(request, "\n");

    for (size_t i = 1; i < lines.size(); i++)
    {
        if (lines[i].find("Host") == 0)
        {
            _nb_host++;
        }
        else if (lines[i].find("Content-Length") == 0)
        {
            _nb_content_length++;
        }
    }
}

VerifyDuplicata::~VerifyDuplicata()
{

}

int const & VerifyDuplicata::getNbHost() const
{
    return (_nb_host);
}

int const & VerifyDuplicata::getNbContentLength() const
{
    return (_nb_content_length);
}
bool const VerifyDuplicata::isDuplicata() const
{
    if (_nb_host > 1 || _nb_content_length > 1)
        return (TRUE);
    return (FALSE);
}

bool const VerifyDuplicata::isMissingData() const
{
    if (_nb_host < 1)
        return (TRUE);
    return (FALSE);
}