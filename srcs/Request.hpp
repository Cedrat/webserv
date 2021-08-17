#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "../includes/utils.hpp"
#include <string>

# define OK 200
# define BAD_REQUEST 400
# define NOT_SUPPORTED 505
# define METHOD_NOT_ALLOWED 405
# define BAD_HOST 666

# define ZERO_REQUEST 0
# define METHOD_LINE 1
# define HOST_LINE 2
# define REQUEST_FINISHED 3

class Config;
class Location;
class Request
{
    private : 
        std::string _method_line;
        std::string _method;
        std::string _path_file_request;
        std::string _host_name;
        int _error; 
        int _where_is_request;
        

    public :
        Request();
        ~Request();

        void        setMethod(std::string method);
        void        setError(int error);
        void        setPathFileRequest(std::string path_file_request);
        void        setWhereIsRequest(int where_is_request);

        std::string getMethod() const;
        std::string getPathFileRequest(void) const;
        int         getError() const;
        std::string getHostName() const;
        int         getWhereIsRequest() const;

        
        int         isAValidMethodLine(std::string method_line);
        
        void        addToRequestHeader(std::string request_linei);
        
        void        verifyMethod(Config config);
        void        verifyHostName(Config config);
        
        Location    findBestLocation(Config config);

        void        checkDuplicate(std::string request);
        void        checkSyntaxRequest(std::string request);
        void        checkAndAddMethod(std::string request);
        void        checkAndAddHostName(std::string request);
};

bool check_if_request_is_in_progress(int request_status);

#endif