#ifndef METHODUPLOAD_HPP
# define METHODUPLOAD_HPP


#include "AMethod.hpp"

enum CurrentStatus {header_received = 1, header_not_received};
class ChunkedRequest;
class MethodUpload: public AMethod
{
    protected :
        std::string _body_received;
        int         _byte_received;
        bool        _file_received;
        int         _byte_send;
        int         _error;

        ChunkedRequest *_chunked_request;

        std::string _tmp_path;

        CurrentStatus _status;
        std::string extractBodyRequest();
        void        receiveData();
        void        writeFile();
        void        writePreparation();
        void        removeBeginDecorations();
        void        removeEndDecorations();
        void        removeAllDecorations();

        void        setHeader();
        void        setChunkedRequest(ChunkedRequest *);
        void        writeProcessedDataChunked();
        bool        maxBodySizeIsReached();

        std::string       createRandomNameWithExtensionAndCheckAvailability(std::string extension, std::string path_test);

    public :
        MethodUpload(int fd, std::string path, std::string request_received, AField &field);
        virtual ~MethodUpload();

    void init();
    void exec();
};

#endif