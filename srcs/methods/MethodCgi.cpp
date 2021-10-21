#include "MethodCgi.hpp"
#include "../fields/AField.hpp"
#include "../includes/define.hpp"

MethodCgi::MethodCgi(int fd, std::string path, std::string header, 
					Config config, Location location, std::string body, 
					std::string method, AField & field, std::string content_type) 
	: AMethod(fd, path, header, field), _config(config), _location(location), 
	_body(body), _method(method), _tmp_out(""), _tmp_in(""), _header_cgi(""), 
	_body_cgi(""), _sent(0), _read_ended(FALSE), _pid_ended(FALSE), _readed(0), _content_type(content_type)
{

}

MethodCgi::~MethodCgi()
{
	delete &_fields;
}

void MethodCgi::init()
{
	_fields.setPollout();
	this->_tmp_out = createTmpFile();
	this->_tmp_in = createTmpFile();
	setEnv();
	processCGI();
}

void MethodCgi::exec()
{
	if (getHeaderSent() == FALSE)
	{
		if (waitpid(_pid, NULL, WNOHANG) != 0 && _pid_ended == FALSE)
		{
			_pid_ended = TRUE;
		}
		if (_pid_ended == TRUE && _read_ended == FALSE)
			readCgiFile();
		else if (_pid_ended == TRUE && _read_ended == TRUE)
		{
			send(getFd(), _header_cgi.c_str(), _header_cgi.size(), 0);
			setHeaderSent(TRUE);
		}
	}
	else	
	{
		sendBody();
	}
}



/**************************************************************
Main CGI process
**************************************************************/
void MethodCgi::processCGI()
{
	//Transformer les variables d'env en char**
	char	**env;
	char	**args;
	env = convertEnv();
	args = setArgs();

	execCGI(args, env);
	freeEnv(env);
	freeArgs(args);
}

void MethodCgi::execCGI( char ** args, char ** env )
{
	int     tmp_out = open(this->_tmp_out.c_str(), 
					O_CREAT | O_RDWR | O_TRUNC | O_NONBLOCK, 
					S_IRUSR | S_IWUSR | std::ios::binary);
	int     tmp_in;

	if (_body.empty() == TRUE)
	{
		tmp_in = open(this->_tmp_in.c_str(), 
					O_CREAT | O_RDWR | O_TRUNC | O_NONBLOCK, 
					S_IRUSR | S_IWUSR);
	}
	else
	{
		tmp_in = open(this->_body.c_str(), 
					O_RDWR | O_NONBLOCK, 
					S_IRUSR | S_IWUSR | std::ios::binary);
	}

	if ((tmp_out < 0) || (tmp_in < 0))
	{
		setErrorResponse(SERVER_ERROR);
		_pid_ended = TRUE;
		return ;
	}

	lseek(tmp_in, 0, SEEK_SET);
	_pid = fork();
	if (_pid == -1)     //Error
	{
		remove(this->_tmp_in.c_str());
		return ;
	}
	else if (_pid == 0)       //Child
	{
		dup2(tmp_in, STDIN_FILENO);
		dup2(tmp_out, STDOUT_FILENO);
		if ((execve(args[0], args, env)) < 0)
		{
			close(tmp_in);
			close(tmp_out);
			remove(this->_tmp_out.c_str());
			remove(this->_tmp_in.c_str());
			exit(-1);
		}
	}
	else  //Parent
	{
		close(tmp_in);
		close(tmp_out);
		remove(this->_tmp_in.c_str());
	}
}

void MethodCgi::readCgiFile()
{
	FILE* f = fopen(this->_tmp_out.c_str(), "r");
	if (f == NULL)
	{
		setErrorResponse(BAD_REQUEST);
		return ;
	}
	size_t  ret;
	char    buffer[BUFFER_CGI + 1] = {0};

	fseek(f, _readed, SEEK_SET);
	ret = fread(buffer, sizeof(*buffer), BUFFER_CGI, f);
	_readed += ret;
	_body_cgi += buffer;

	if (ferror(f))
	{
		if (_tmp_out != "")
		{
			fclose(f);
			remove(this->_tmp_out.c_str());
			this->_tmp_out = "";
			setErrorResponse(BAD_REQUEST);
			return ;
		}
	}
	else if (feof(f))
	{
		_read_ended = TRUE;
		remove(this->_tmp_out.c_str());
		this->_tmp_out = "";
	}
	fclose(f);
	if (ret == BUFFER_CGI)  //Lecture pas finie
		return ;
	extractHeader();  //Recup et completion du header
}




/**************************************************************
Setters
**************************************************************/
void MethodCgi::setEnv()
{
	// Serveur
	this->_env["REDIRECT_STATUS="] = "200";
	this->_env["SERVER_SOFTWARE="] = "Webserv/1.0";
	this->_env["SERVER_NAME="] = _config.getIP();
	this->_env["GATEWAY_INTERFACE="] = "CGI/1.1";
	// Requete
	this->_env["SERVER_PROTOCOL="] = "HTTP/1.1";
	this->_env["SERVER_PORT="] = int_to_string(_config.getPort());
	this->_env["REQUEST_METHOD="] = this->_method;
	// Paths and stuff
	this->_env["PATH_INFO="] = getPathInfo();
	this->_env["PATH_TRANSLATED="] = getPathInfo();
	this->_env["QUERY_STRING="] = _fields.getQuery();
	this->_env["REQUEST_URI="] = this->_path + _fields.getQuery();
	this->_env["SCRIPT_FILENAME="] = this->_path;
	this->_env["REMOTE_HOST="] = _fields.getHostName();
	this->_env["CONTENT_LENGTH="] = getFileSize();
	this->_env["CONTENT_TYPE="] = this->_content_type;
}




/**************************************************************
Utils
**************************************************************/
std::string MethodCgi::createTmpFile()
{
	char _tmp_file_name[] = "tmp/tmpXXXXXX";
	int	fd;

	fd = mkstemp(_tmp_file_name);
	if (fd < 0)
	{
		setErrorResponse(500);
		_pid_ended = TRUE;
		return ("");
	}
	else
		close(fd);

	return  _tmp_file_name;
}

void MethodCgi::freeEnv( char ** env )
{
	for(int i = 0; env[i]; i++)
		delete [] env[i];
	delete [] env;
}

void MethodCgi::freeArgs( char ** args )
{
	for(int i = 0; args[i]; i++)
		delete [] args[i];
	delete [] args;
}

char ** MethodCgi::convertEnv()
{
	std::map<std::string, std::string>::const_iterator it = this->_env.begin();
	char **new_env = new char*[this->_env.size() + 1];
	int i = 0;

	while (it != this->_env.end())
	{
		new_env[i] = new char[it->first.size() + it->second.size() + 1];
		strcpy(new_env[i], it->first.c_str());
		strcat(new_env[i], it->second.c_str());
		it++;
		i++;
	}
	new_env[i] = NULL;

	return new_env;
}

char ** MethodCgi::setArgs()
{
	std::string     binary_path = _location.getCgiBinary();
	char 			**args = new char*[3];

	args[0] = new char[binary_path.size() + 1];
	args[0] = strcpy(args[0], binary_path.c_str());

	args[1] = new char[this->getPath().size() + 1];
	args[1] = strcpy(args[1], this->getPath().c_str());

	args[2] = NULL;

	return args;
}

bool MethodCgi::getStatus()
{
	return this->_is_finished;
}

std::string MethodCgi::getPathInfo()
{
	std::string	file_path;
	size_t		pos;

	if ((pos =_path.rfind("/")) != std::string::npos)
	{
		file_path = _path.substr(0, pos);
		return file_path;
	}
	return _path;
}

std::string MethodCgi::getFileSize()
{
	if (_body.empty() == TRUE)
		return int_to_string(0);

	std::ifstream file;

    file.open(_body.c_str(), std::fstream::in | std::fstream::binary | std::fstream::app);
    if (file.is_open() == FALSE)
    {
        return int_to_string(0);
    }

    file.seekg (0, std::ios::end);
    unsigned long end = file.tellg();
	file.close();
	
	return int_to_string(end);
}



/**************************************************************
Headers
**************************************************************/
void    MethodCgi::extractHeader()
{
	size_t  end_header;
	if(((end_header = _body_cgi.find("\r\n\r\n")) != std::string::npos) 
		|| ((end_header = _body_cgi.find("\n\n")) != std::string::npos))
	{
		_header_cgi = _body_cgi.substr(0, end_header + 4);
		_body_cgi = _body_cgi.substr(end_header + 4);
	}
	else
	{
		setErrorResponse(BAD_REQUEST);
		return ;
	}
	adaptHeader();
}

void MethodCgi::adaptHeader()
{
	std::string new_header;

	new_header = "HTTP/1.1 200 OK\n";
	new_header += "Content-Length: " + int_to_string(_body_cgi.size()) + "\n";
	new_header += _header_cgi;
	_header_cgi = new_header;
}



/**************************************************************
Error management
**************************************************************/
void    MethodCgi::setErrorResponse(int error)
{
	std::string path_error = _config.getPathError(error);

	_header_cgi = "HTTP/1.1 " + get_string_error(error);
	_header_cgi += "\nContent-Length: " + int_to_string(get_file_size(path_error)) + "\n";
	_header_cgi += date_string() + " \n\n";

	std::fstream fs;
	std::stringstream buffer;

	fs.open(path_error.c_str(),  std::fstream::in); 
	buffer << fs.rdbuf();
	_body_cgi = buffer.str();
	_read_ended = TRUE;
	fs.close();
}


/**************************************************************
Body
**************************************************************/
void MethodCgi::sendBody()
{
	char                buffer[BUFFER_SIZE + 1];
	int                 ret = 0;
	std::stringstream   ss;

	ss.write(_body_cgi.c_str(), _body_cgi.size());

	if (ss.fail())
	{
		throw(CloseSocketException());
	}
	ss.seekg(_sent);
	ss.read(buffer, BUFFER_SIZE);
	buffer[ss.gcount()] = '\0';

	ret = ::send(getFd(), buffer, ss.gcount(), 0);
	if (ret == -1)
    {
         throw(UnableToSendException());
    }
	_sent += ret;
	if (ret == ss.gcount() && ss.eof())
	{
		setIsFinished(TRUE);
	}
}
