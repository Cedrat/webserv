#include "MethodPostCgi.hpp"

MethodPostCgi::MethodPostCgi( int fd, std::string path, std::string request_received, 
                    AField &field, Config config, Location location, std::string content_type )
	: AMethod(fd, path, request_received, field), _config(config), _location(location), _body(""),
	_content_type(content_type), _cgi_init(FALSE), _byte_received(0), _file_received(FALSE),
    _byte_send(0), _error(NO_CONTENT)
{
	_path_file = construct_path_post(_fields.getPath(), location);
}

MethodPostCgi::~MethodPostCgi() {
	delete _cgi;
}



void MethodPostCgi::init()
{
    _fields.setPollin();
    _body_received = extractBodyRequest();
    if (_fields.getTransfertEncoding() == "chunked")
    {
        _body_received = "\r\n" + _body_received;
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
            _file_received = TRUE;
            _fields.setPollout();
        }
    }
}

void MethodPostCgi::writeFile()
{
    std::fstream file;

    file.open(_path.c_str(), std::fstream::out | std::fstream::binary | std::fstream::app);

    file.write(_body_received.c_str(),_body_received.size());

    file.close();
}

void MethodPostCgi::writeProcessedDataChunked()
{
    int fd = open(getPath().c_str(),  O_APPEND| O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    
    _chunked_request->writeProcessedData(fd); 

    close(fd); 
}

void MethodPostCgi::writePreparation()
{
    _byte_received += _body_received.size();
    if ((_byte_received) > _fields.getContentLength())
    {
        int nb_chars_to_erase = (_byte_received - _fields.getContentLength());
        _body_received.erase(_body_received.end() - nb_chars_to_erase, _body_received.end());
    }
}

void MethodPostCgi::exec()
{
	if (_file_received == FALSE)
	{
		receiveData();
		if (_fields.getTransfertEncoding() == "chunked")
		{
			Info data;
			_chunked_request->addData(_body_received);
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
				_file_received = TRUE;
				_fields.setPollout();
			}
		}
	}
	else if (_cgi_init == FALSE)
	{
		_cgi = new MethodCgi(_fd, _path_file, "", _config, _location, _path, "POST", _fields, _content_type);
		_cgi->init();
		_cgi_init = TRUE;
	}
	else
	{
		_cgi->exec();
		if (_cgi->getStatus() == TRUE)
		{
			setIsFinished(TRUE);
		}	
	}
}


void MethodPostCgi::setChunkedRequest(ChunkedRequest *chunked_request)
{
    _chunked_request = chunked_request;
}

bool MethodPostCgi::maxBodySizeIsReached()
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

std::string MethodPostCgi::extractBodyRequest()
{
    std::string copy_request = _header;

    copy_request.erase(0, _header.find("\r\n\r\n") + 4);

    return (copy_request);
}

void MethodPostCgi::receiveData()
{
    char buffer[BUFFER_SIZE + 1];
    int ret;
    
    ret = read(_fd, buffer, BUFFER_SIZE);
    buffer[ret] = 0;
    _body_received.append(buffer, ret);
}