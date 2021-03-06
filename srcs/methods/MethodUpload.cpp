#include "MethodUpload.hpp"
#include "AMethod.hpp"
#include "../fields/AField.hpp"
#include "../fields/FieldPost.hpp"
#include "../includes/utils.hpp"
#include "../parsing/ChunkedRequest.hpp"




MethodUpload::MethodUpload(int fd, std::string path, std::string request_received, AField &field) :
AMethod(fd, path, request_received, field), _byte_received(0), _file_received(FALSE), _byte_send(0), _error(CREATED), _chunked_request(NULL), _status(header_not_received)
{
}


MethodUpload::~MethodUpload()
{
    if (_chunked_request != NULL)
        delete _chunked_request;
    delete &_fields;
}

void MethodUpload::init()
{
    _fields.setPollin();
    _body_received = extractBodyRequest();
    
    std::string extension;
    extension = extract_extension(_header);
    _path += createRandomNameWithExtensionAndCheckAvailability(extension, _path);
    if (_body_received.find("\r\n\r\n") != std::string::npos)
    {
        writePreparation();
        try {
            if (_byte_received >= _fields.getContentLength())
                removeAllDecorations();
            else
                removeBeginDecorations();
        }
        catch (BadRequestException const & e)
        {
            _error = BAD_REQUEST;
            _file_received = TRUE;
            _fields.setPollout();
        }
        writeFile();
        _body_received = "";
        if (_byte_received >= _fields.getContentLength())
        {
            _fields.setPollin();
            _file_received = TRUE;
            _fields.setPollout();
        }
        _status = header_received;
    }
}
    // check length body_received and content-Length
    // If content-Length inferior to size body
    // pass pollfd to pollout and prepare header request and body request kiss.


void MethodUpload::exec()
{
    if (_file_received == FALSE)
    {
        receiveData();
        writePreparation();
        try {
            if (_status == header_not_received)
            {
                removeBeginDecorations();
                _status = header_received;    
            }
            if (_byte_received >= _fields.getContentLength())
            {
                removeEndDecorations();
            }
        }
        catch (BadRequestException const & e)
        {
            _error = BAD_REQUEST;
            _file_received = TRUE;
            _fields.setPollout();
        }
        writeFile();
        _body_received = "";
        if (_byte_received >= _fields.getContentLength())
        {
            _fields.setPollin();
            _file_received = TRUE;
            _fields.setPollout();
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

std::string MethodUpload::extractBodyRequest()
{
    std::string copy_request = _header;

    copy_request.erase(0, _header.find("\r\n\r\n") + 4);
    
    return (copy_request);
}


void MethodUpload::writeProcessedDataChunked()
{
    int fd = open(getPath().c_str(),  O_APPEND| O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    
    _chunked_request->writeProcessedData(fd); 

    close(fd); 
}

void MethodUpload::writePreparation()
{
    _byte_received += _body_received.size();
    if ((_byte_received) > _fields.getContentLength())
    {
        int nb_chars_to_erase = (_byte_received - _fields.getContentLength());
        _body_received.erase(_body_received.end() - nb_chars_to_erase, _body_received.end());
    }
}


void MethodUpload::setHeader()
{

   std::vector<Config> configs = _fields.getDataRequest().getConfigs();
   Config best_config = configs[find_index_best_config(configs, _fields.getHostName(), _fields.getDataRequest().getPort(), _fields.getDataRequest().getHost())];

   std::string path_error = best_config.getPathError(_error);
    
    _header = "HTTP/1.1 " + get_string_error(_error);
    _header += "\nContent-Length: " + int_to_string(get_file_size(path_error)) + "\n";
    _header +=  date_string() + "\n\n";
    setPath(path_error); 
}

void MethodUpload::setChunkedRequest(ChunkedRequest *chunked_request)
{
    _chunked_request = chunked_request;
}

bool MethodUpload::maxBodySizeIsReached()
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

void MethodUpload::removeBeginDecorations()
{
    int nb_lines_to_erase = 4;

    for (int i = 0; i < nb_lines_to_erase; i++)
    {
        if (_body_received.find("\r\n") != std::string::npos)
            _body_received.erase(0, _body_received.find("\r\n")+ 2);
        else
            throw BadRequestException();
    }
    
}

void MethodUpload::removeEndDecorations()
{
    if (_body_received.find("-----------------------------") != std::string::npos)
        _body_received.erase(_body_received.find("-----------------------------"), _body_received.size());
     else
        throw BadRequestException();
    if (_body_received.find("\r\n") != std::string::npos)
        _body_received.erase(_body_received.size() - 2, 2);
     else
        throw BadRequestException();
}

std::string    MethodUpload::createRandomNameWithExtensionAndCheckAvailability(std::string extension, std::string path_test)
{
    char alphanum[] = "1234567890qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";

    std::srand(time(0));
    bool is_not_available = TRUE;
    int size_name = 10;
    std::string name_file;

    while (is_not_available)
    {
        name_file.resize(size_name);
        for (int i = 0; i < size_name; i++)
        {
            name_file[i] = alphanum[std::rand()%62];
        }
        name_file += "." + extension;
        if (check_if_file_exist(path_test + name_file) == FALSE)
            is_not_available = FALSE;
    }
    return (name_file);
}


void MethodUpload::removeAllDecorations()
{

    removeBeginDecorations();
    removeEndDecorations();
}