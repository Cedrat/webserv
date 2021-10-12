#include "MethodPost.hpp"
#include "AMethod.hpp"
#include "AField.hpp"
#include "FieldPost.hpp"
#include "../includes/utils.hpp"
#include "ChunkedRequest.hpp"


MethodPost::MethodPost(int fd, std::string path, std::string request_received, AField &field) :
AMethod(fd, path, request_received, field), _byte_received(0), _file_received(FALSE), _byte_send(0), _error(NO_CONTENT), _chunked_request(NULL)
{
    std::cout << "Welcome to MethodPost" << std::endl;
}


MethodPost::~MethodPost()
{
    if (_chunked_request != NULL)
        delete _chunked_request;
    delete &_fields;
}

void MethodPost::init()
{
    _fields.setPollin();
    _body_received = extractBodyRequest();
    if (_fields.getTransfertEncoding() == "chunked")
    {
        _body_received = "\r\n" + _body_received;
        std::cout << "Chunked Request" << std::endl;
        Info data;
        setChunkedRequest(new ChunkedRequest);
        _chunked_request->addData(_body_received);
        _byte_received += _body_received.size();
        if (maxBodySizeIsReached())
            return ;
        data = _chunked_request->processData();
        writeProcessedDataChunked();
        _body_received = "";
        if (data == all_data_read)
        {
            _file_received = TRUE;
            _fields.setPollout();
        }
        else if (data == incorrect_data)
        {
            _file_received = TRUE;
            _error = BAD_REQUEST;
            _fields.setPollout();
        }
    }
    else
    {
        writePreparation();
        writeFile();
        _body_received = "";
        if (_byte_received >= _fields.getContentLength())
        {
            _fields.setPollin();
            std::cout << "File received entirely" << std::endl;
            _file_received = TRUE;
            _fields.setPollout();
        }
    }
}
    // check length body_received and content-Length
    // If content-Length inferior to size body
    // pass pollfd to pollout and prepare header request and body request kiss.


void MethodPost::exec()
{
    if (_file_received == FALSE)
    {
        receiveData();
        if (_fields.getTransfertEncoding() == "chunked")
        {
            Info data;
            _chunked_request->addData(_body_received);
            _byte_received += _body_received.size();
            if (maxBodySizeIsReached())
                return ;
            data = _chunked_request->processData();
            writeProcessedDataChunked();
            _body_received = "";
            if (data == all_data_read)
                _file_received = TRUE;
             else if (data == incorrect_data)
            {
                _file_received = TRUE;
                _error = BAD_REQUEST;
            }
        }
        else
        {
            writePreparation();
            writeFile();
            _body_received = "";
            if (_byte_received >= _fields.getContentLength())
            {
                _fields.setPollin();
                std::cout << "File received entirely" << std::endl;
                _file_received = TRUE;
                _fields.setPollout();
            }
        }
    }
    else
    {
        if (getHeaderSent() == FALSE)
        {
            setHeader();
            sendHeader();
            setHeaderSent(TRUE);
        }
        else
        {
            sendBody();
        }
    }
}

std::string MethodPost::extractBodyRequest()
{
    std::string copy_request = _header;

    copy_request.erase(0, _header.find("\r\n\r\n") + 4);
    
    return (copy_request);
}

void MethodPost::receiveData()
{
    char buffer[BUFFER_SIZE + 1];
    int ret;
    
    ret = read(_fd, buffer, BUFFER_SIZE);
    buffer[ret] = 0;
    _body_received.append(buffer, ret);

    std::cout << "size_Body "<<  _body_received.size() << "ret = " << ret << std::endl;
     
    std::cout << "_body_received :\n" << _body_received << std::endl;

    // check length body_received and content-Length
    // If content-Length inferior to size body
    // pass pollfd to pollout and prepare header request and body request kiss.

}


void MethodPost::writeFile()
{
    std::fstream file;

    file.open(_path.c_str(), std::fstream::out | std::fstream::binary | std::fstream::app);

    file.write(_body_received.c_str(),_body_received.size());

    file.close();
}

void MethodPost::writeProcessedDataChunked()
{
    int fd = open(getPath().c_str(),  O_APPEND| O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    
    _chunked_request->writeProcessedData(fd); 

    close(fd); 
}

void MethodPost::writePreparation()
{
    _byte_received += _body_received.size();
    if ((_byte_received) > _fields.getContentLength())
    {
        int nb_chars_to_erase = (_byte_received - _fields.getContentLength());
        _body_received.erase(_body_received.end() - nb_chars_to_erase, _body_received.end());
    }
}

void MethodPost::sendHeader()
{
   send(getFd(), getHeader().c_str(), getHeader().size(), 0); 
}

void MethodPost::sendBody()
{
    signal(SIGPIPE, SIG_IGN);
    std::fstream fs;
    char buffer[BUFFER_SIZE + 1];
    int ret = 0;
    fs.open(getPath().c_str(),  std::fstream::in | std::fstream::app); 
    fs.seekg(_byte_send);
    fs.read(buffer, BUFFER_SIZE);
    buffer[fs.gcount()] = '\0'; 
    ret = ::send(getFd(), buffer, fs.gcount(), 0);
    _byte_send += ret;
    if (ret == fs.gcount() && fs.eof())
    {
        setIsFinished(TRUE);
    }
    fs.close();
}

void MethodPost::setHeader()
{

   std::vector<Config> configs = _fields.getDataRequest().getConfigs();
   Config best_config = configs[find_index_best_config(configs, _fields.getHostName(), _fields.getDataRequest().getPort(), _fields.getDataRequest().getHost())];

   std::string path_error = best_config.getPathError(_error);
    
    _header = "HTTP/1.1 " + get_string_error(_error);
    _header += "\nContent-Length: " + int_to_string(get_file_size(path_error)) + "\n";
    _header +=  date_string() + "\n\n";
    setPath(path_error); 
}

void MethodPost::setChunkedRequest(ChunkedRequest *chunked_request)
{
    _chunked_request = chunked_request;
}

bool MethodPost::maxBodySizeIsReached()
{
    Config config = _fields.getDataRequest().getConfigs()[find_index_best_config(_fields.getDataRequest().getConfigs(), _fields.getHostName(), _fields.getDataRequest().getPort(), _fields.getDataRequest().getHost())];

    if (_byte_received > config.getMaxBodySize())
    {
        _error = ENTITY_TOO_LARGE;
        _file_received = TRUE;
        _fields.setPollout();
        return TRUE;
    }
    return FALSE;
}