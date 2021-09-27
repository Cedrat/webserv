#include "../includes/utils.hpp"

bool check_if_request_is_in_progress(int request_status)
{
    if (request_status != ZERO_REQUEST && request_status != REQUEST_FINISHED)
        return (TRUE);
    return (FALSE);
}
