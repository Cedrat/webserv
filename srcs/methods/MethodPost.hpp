#ifndef METHODPOST_HPP
# define METHODPOST_HPP


#include "AMethod.hpp"
class ChunkedRequest;
class MethodPost: public AMethod
{
    protected :
        int         _byte_received;
        bool        _file_received;
        int         _error;

        ChunkedRequest *_chunked_request;

        std::string _tmp_path;

        std::string extractBodyRequest();
        void        writePreparation();

        void        setHeader();
        void        setChunkedRequest(ChunkedRequest *);
        void        writeProcessedDataChunked();
        bool        maxBodySizeIsReached();

    public :
        MethodPost(int fd, std::string path, std::string request_received, AField &field);
        virtual ~MethodPost();

    void init();
    void exec();
};

#endif