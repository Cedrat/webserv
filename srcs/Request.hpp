#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <string>
#include <vector>

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
class ResponseHTTP;
class Request
{
    private :
        std::string _method;
        std::string _request;
        std::string _host_name;
        std::string _method_line;
        std::string _path_file_request;
        int _error;
        int _where_is_request;
        bool _sending_data;
        ResponseHTTP _data_to_send;


    public :
        Request();
        ~Request();

        void        setError(int error);
        void        setSendingData(bool);
        void        setMethod(std::string method);
        void        setResponseHTTP(ResponseHTTP  rep);
        void        setPathFileAnswer(const char* path);
        void        setWhereIsRequest(int where_is_request);
        void        setPathFileRequest(std::string path_file_request);

        int             getError() const;
        std::string     getMethod() const;
        std::string     getRequest() const;
        std::string     getHostName() const;
        bool            getSendingData() const;
        ResponseHTTP    getResponseHTTP() const;
        int             getWhereIsRequest() const;
        std::string     getPathFileRequest(void) const;

        int         isAValidMethodLine(std::string method_line);

        void        resetRequest();
        void        addToRequest(std::string request);
        void        addToRequestHeader(std::string request_linei);


        void        verifyMethod(Config config);
        void        verifyHostName(Config config);

        Location    findBestLocation(Config config);

        void        checkSyntaxRequest();
        void        checkDuplicate(std::string request);
        void        checkAndAddMethod(std::string request);
        void        checkSyntaxRequest(std::string request);
        void        checkAndAddHostName(std::string request);
        void        checkDuplicate(std::vector<std::string> all_lines);

        void        setFdAnswer(int);

        void        send();

        void        checkPath();

        void        resetByteSend();
        void        setFinished(bool);
};

bool check_if_request_is_in_progress(int request_status);

#endif
