#include "../includes/utils.hpp"
#include "../includes/define.hpp"

bool check_timeout(int timeout)
{
    if (timeout == -1)
        return (FALSE); 
    if ((std::time(0) - timeout) > TIMEOUT)
    {
        return (TRUE);
    }
    return (FALSE);
}