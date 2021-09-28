#include "MethodCgi.hpp"
#include "AField.hpp"
#include "define.hpp"

MethodCgi::MethodCgi(int fd, std::string path, std::string header, Config config, Location location, std::string body, std::string method, AField & field) 
    : AMethod(fd, path, header, field), _config(config), _location(location), _body(body), _method(method), _header_cgi(""), _body_cgi(""), _sent(0)
{
    _tmpOut = "";
    _read_ended = FALSE;
    _pid_ended = FALSE;
    _readed = 0;
}

MethodCgi::~MethodCgi()
{

}

void MethodCgi::init()
{
    _fields.setPollout();
    this->_tmpOut = createTmpFile();
    setEnv();
    processCGI();
}

void MethodCgi::exec()
{
    if (getHeaderSent() == FALSE)
    {
        if (waitpid(_pid, NULL, WNOHANG) > 0)
            _pid_ended = TRUE;      //CGI process terminé
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
    std::string     binary_path = construct_path(_location.getCgiBinary(), _location);
    const char      *args[3] = {binary_path.c_str(), this->getPath().c_str(), NULL};

    //Transformer les variables d'env en char**
    char **env;
    env = convertEnv();

    //Gestion de l'execution du cgi
    execCGI(args, env);
    freeEnv(env);
}

void MethodCgi::execCGI( const char ** args, char ** env )
{
    int     fd[2];
    int     tmp = open(this->_tmpOut.c_str(), 
                    O_CREAT | O_RDWR | O_TRUNC | O_NONBLOCK, 
                    S_IRUSR | S_IWUSR);

    //Creation des pipes
    if ((pipe(fd) < 0) || (tmp < 0))
    {
        std::cerr << "Pipe or tmp file opening failed" << std::endl;
        return ;
    }

    //Ecriture du body dans le cas de POST
    write(fd[0], _body.c_str(), _body.size());
    lseek(fd[0], 0, SEEK_SET);

    _pid = fork();
    if (_pid == -1)     //Error
    {
        std::cerr << "Error with fork()" << std::endl;
        remove(this->_tmpOut.c_str());
        return ;
    }
    else if (_pid == 0)       //Child
    {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        dup2(tmp, STDOUT_FILENO);

        if ((execve(args[0], const_cast<char *const *>(args), env)) < 0)
        {
            std::cerr << "Execve failed" << std::endl;
            close(fd[0]);
            close(tmp);
            remove(this->_tmpOut.c_str());
            exit(-1);
        }
    }
    else  //Parent
    {
        close(fd[0]);
        close(tmp);
    }
}

void MethodCgi::readCgiFile()
{
    FILE* f = fopen(this->_tmpOut.c_str(), "r");
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
        if (_tmpOut != "")
        {
            fclose(f);
            remove(this->_tmpOut.c_str());
            setErrorResponse();
            return ;
        }
    }
    else if (feof(f))
    {
        _read_ended = TRUE;
        remove(this->_tmpOut.c_str());
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
    this->_env["PATH_TRANSLATED="] = this->getPath();   //path sans la partie www/, juste fin du chemin vers fichier ?
    this->_env["QUERY_STRING="] = _fields.getQuery();

    this->_env["REQUEST_URI="] = this->getPath() + _fields.getQuery();   //Chemin vers le fichier + requete query. Facultatif ?

    this->_env["REMOTE_HOST="] = _fields.getHostName();
    this->_env["CONTENT_LENGTH="] = int_to_string(this->_body.size());
}



/**************************************************************
Utils
**************************************************************/
std::string MethodCgi::createTmpFile()
{
    char _tmpFileName[] = "tmpXXXXXX";
    mkstemp(_tmpFileName);

    return _tmpFileName;
}

void MethodCgi::freeEnv(char ** env)
{
    for(int i = 0; env[i]; i++)
        delete [] env[i];
    delete [] env;
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



/**************************************************************
Headers
**************************************************************/
void    MethodCgi::extractHeader()
{
    size_t  end_header;

    if((end_header = _body_cgi.find("\r\n\r\n")) != std::string::npos)
    {
        _header_cgi = _body_cgi.substr(0, end_header + 4);
        _body_cgi = _body_cgi.substr(end_header + 4);
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
    //signal(SIGPIPE, SIG_IGN);

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