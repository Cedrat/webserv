#include "MethodCgi.hpp"
#include "AField.hpp"
#include "define.hpp"

MethodCgi::MethodCgi(int fd, std::string path, std::string header, Config config, Location location, std::string body, std::string method, AField & field, std::string post_path) 
	: AMethod(fd, path, header, field), _config(config), _location(location), _body(body), _method(method), _header_cgi(""), _body_cgi(""), _sent(0), _post_path(post_path)
{
	_tmp_out = "";
	_read_ended = FALSE;
	_pid_ended = FALSE;
	_readed = 0;
}

MethodCgi::~MethodCgi()
{

}

void MethodCgi::init()
{
	//Temporaire pour url cassées :<
	treatPath();
	
	_fields.setPollout();
	this->_tmp_out = createTmpFile();
	setEnv();
	processCGI();
}

void MethodCgi::exec()
{
	if (getHeaderSent() == FALSE)
	{
		if (waitpid(_pid, NULL, WNOHANG) != 0 && _pid_ended == FALSE)
		{
			_pid_ended = TRUE;      //CGI process terminé
		}
		if (_pid_ended == TRUE && _read_ended == FALSE)
			readCgiFile();          //read non bloquant
		else if (_pid_ended == TRUE && _read_ended == TRUE)
		{
			send(getFd(), _header_cgi.c_str(), _header_cgi.size(), 0);
			setHeaderSent(TRUE);
			std::cerr << "CGI header sent" << std::endl;
		}
	}
	else
	{
		sendBody();
		std::cout << "CGI body sent" << std::endl;
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
}

void MethodCgi::execCGI( char ** args, char ** env )
{
	int     fd[2];
	int     tmp = open(this->_tmp_out.c_str(), 
					O_CREAT | O_RDWR | O_TRUNC | O_NONBLOCK, 
					S_IRUSR | S_IWUSR);

	FILE *fin = tmpfile();
	long tmp_in = fileno(fin);

	//Creation des pipes
	if ((pipe(fd) < 0) || (tmp < 0) || (tmp_in < 0))
	{
		std::cerr << "Pipe or tmp file opening failed" << std::endl;
		return ;
	}

	//Ecriture du body dans le cas de POST
	write(tmp_in, _body.c_str(), _body.size());
	lseek(tmp_in, 0, SEEK_SET);


	_pid = fork();
	if (_pid == -1)     //Error
	{
		std::cerr << "Error with fork()" << std::endl;
		remove(this->_tmp_out.c_str());
		return ;
	}
	else if (_pid == 0)       //Child
	{
		dup2(tmp_in, STDIN_FILENO);
		dup2(tmp, STDOUT_FILENO);

		if ((execve(args[0], args, env)) < 0)
		{
			close(tmp_in);
			close(tmp);
			remove(this->_tmp_out.c_str());
			exit(-1);
		}
	}
	else  //Parent
	{
		close(tmp_in);
		close(tmp);
	}
}

void MethodCgi::readCgiFile()
{
	FILE* f = fopen(this->_tmp_out.c_str(), "r");
	if (f == NULL)
	{
		setErrorResponse();
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
		std::cerr << "Error reading tmp" << std::endl;
		if (_tmp_out != "")
		{
			fclose(f);
			remove(this->_tmp_out.c_str());
			this->_tmp_out = "";
			setErrorResponse();
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

		//Fichier à ouvrir avec le binaire 
	this->_env["PATH_INFO="] = this->getPath();     //A verifier
	//this->_env["PATH_TRANSLATED="] = "test_cgi/POST_test_02.php";   //path sans la partie www/, juste fin du chemin vers fichier ?
	this->_env["QUERY_STRING="] = _fields.getQuery();
	//Chemin vers le fichier + requete query.
	this->_env["REQUEST_URI="] = this->getPath() + _fields.getQuery();
	this->_env["SCRIPT_FILENAME="] = this->getPath();

	this->_env["REMOTE_HOST="] = _fields.getHostName();
	this->_env["CONTENT_LENGTH="] = int_to_string(this->_body.size());
}



/**************************************************************
Utils
**************************************************************/
std::string MethodCgi::createTmpFile()
{
	char _tmp_file_name[] = "tmpXXXXXX";
	int	fd;

	fd = mkstemp(_tmp_file_name);
	if (fd < 0)
		throw(EmergencyExit());
	else
		close(fd);

	return _tmp_file_name;
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
	std::string     binary_path = construct_path(_location.getCgiBinary(), _location);
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

void MethodCgi::treatPath()
{
	if (_path.find("./www") == std::string::npos)
		_path.insert(0, "./www");
	
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
		setErrorResponse();
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
void    MethodCgi::setErrorResponse()
{
	std::string path_error = _config.getPathError(BAD_REQUEST);

	_header_cgi = "HTTP/1.1 " + get_string_error(BAD_REQUEST);
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

	ss.seekg(_sent);
	ss.read(buffer, BUFFER_SIZE);
	buffer[ss.gcount()] = '\0';

	std::cout << "how much read? " << ss.gcount() << std::endl;
	ret = ::send(getFd(), buffer, ss.gcount(), 0);
	_sent += ret;

	if (ret == ss.gcount() && ss.eof())
	{
		setIsFinished(TRUE);
	}
	std::cout << ret << "BYTE SEND" << std::endl;
}
