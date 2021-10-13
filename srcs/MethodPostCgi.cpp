#include "MethodPostCgi.hpp"

MethodPostCgi::MethodPostCgi( int fd, std::string path, std::string request_received, AField &field,
		Config config, Location location )
	: MethodPost(fd, path, request_received, field), _config(config), _location(location), _body(""),
	_cgi_init(FALSE), _get_body(FALSE), _content_type("")
{
	_byte_received = 0;
	_file_received = FALSE;
	_byte_send = 0;
	_error = NO_CONTENT;
	_path_file = construct_path_post(_fields.getPath(), location);
}

MethodPostCgi::~MethodPostCgi() {
	delete _cgi;
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
				std::cout << "File received entirely" << std::endl;
				_file_received = TRUE;
				_fields.setPollout();
			}
		}
	}
	else if (this->_get_body == FALSE)
	{
		//Stockage du body dans une variable pour transfert au CGI
		getBody();
	}
	else if (_cgi_init == FALSE)
	{
		//_path pas nécessaire finalement. A enlever ?
		_cgi = new MethodCgi(_fd, _path_file, "", _config, _location, _body, "POST", _fields, _path);
		_cgi->init();
		_cgi_init = TRUE;
	}
	else
	{
		std::cout << "CGI exec" << std::endl;
		_cgi->exec();
		if (_cgi->getStatus() == TRUE)
		{
			remove(_path.c_str());
			setIsFinished(TRUE);
		}	
	}
}

void MethodPostCgi::getBody()
{
	std::fstream file;

    file.open(_path.c_str(), std::fstream::in | std::fstream::binary | std::fstream::app);

	file.seekg(0, file.end);
	int len = file.tellg();
	char buffer[len + 1];

	file.seekg(0, file.beg);
	file.read(buffer, len);
	buffer[file.gcount()] = '\0';
	_body = buffer;
	file.close();
	_get_body = TRUE;
}
