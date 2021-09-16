#include <regex.h>
#include <stdio.h>
#include <unistd.h>

int match_regex(char *request, char * motif)
{

    regex_t preg;

    if (regcomp(&preg, motif, REG_NOSUB | REG_EXTENDED) != 0)
    {
        return (0);
    }
    if (regexec(&preg, request, 0 , NULL , 0) == 0)
        return (1);
    return (-1);
}