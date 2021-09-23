#include "MethodCgi.hpp"

MethodCgi::MethodCgi(int fd, std::string path, std::string header, Config config, Location location, std::string body, std::string method) 
    : AMethod(fd, path, header), _config(config), _location(location), _body(body), _method(method), _header_cgi(""), _body_cgi(""), _sent(0)
{
    _tmpOut = "";
}

MethodCgi::~MethodCgi()
{

}

void MethodCgi::init()
{
    this->_tmpOut = createTmpFile();
    setEnv();
    processCGI();
}

void MethodCgi::exec()
{
    if (getHeaderSent() == FALSE)
    {
        if (this->_tmpOut != "")
            readCgiFile();
        if (this->_tmpOut == "")
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
    std::string binary_path = construct_path(_location.getCgiBinary(), _location);
    const char *args[3] = {binary_path.c_str(), this->getPath().c_str(), NULL};

    //Transformer les variables d'env en char**
    char **env;
    env = convertEnv();

    //Gestion de l'execution du cgi
    if (execCGI(args, env) == -1)
    {
        std::cerr << "Error during CGI execution" << std::endl;
        return ;
    }
    freeEnv(env);
}

int MethodCgi::execCGI( const char ** args, char ** env )
{
    pid_t   pid;
    int     fd[2];
    int     tmp = open(this->_tmpOut.c_str(), 
                    O_CREAT | O_RDWR | O_TRUNC | O_NONBLOCK, 
                    S_IRUSR | S_IWUSR);

    //Creation des pipes
    if ((pipe(fd) < 0) || (tmp < 0))
    {
        std::cerr << "Pipe or tmp file opening failed" << std::endl;
        return -1;
    }

    //Ecriture du body dans le cas de POST
    write(fd[0], _body.c_str(), _body.size());
    lseek(fd[0], 0, SEEK_SET);

    //Creation du child process
    pid = fork();
    if (pid == -1)          //Erreur
    {
        std::cerr << "Error with fork()" << std::endl;
        close(fd[0]);
        close(fd[1]);
        return -1;
    }
    else if (pid == 0)       //Child
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
            return -1;
        }
    }
    else  //Parent
    {
        close(fd[0]);
    }
    return 1;
}

void MethodCgi::readCgiFile()
{
    int ret;
    char buffer[50 + 1] = {0};

    int fd = open(this->_tmpOut.c_str(), O_RDONLY);
    if (fd < 0)
    {
        //std::cout << strerror(errno) << std::endl;
        std::cerr << "Error opening tmp" << std::endl;
        exit(0);
    }

    while ((ret = read(fd, buffer, 50)) > 0)
    {
        _body_cgi += buffer;
        memset(buffer, 0, 50);
    }

    close(fd);

    if (ret < 0)
    {
        std::cerr << "Error reading tmp" << std::endl;
        exit(0);
    }
    if (ret == 0 && _body_cgi.size() == 0)
    {
        return;
    }

    remove(this->_tmpOut.c_str());
    _tmpOut = "";

    extractHeader();
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
    this->_env["AUTH_TYPE="] = "Basic";            //A remplacer ?
        //Chemin vers le binaire. 
    this->_env["PATH_INFO="] = construct_path(_location.getCgiBinary(), _location);
    this->_env["PATH_TRANSLATED="] = construct_path(_location.getCgiBinary(), _location);

        //Fichier à ouvrir avec le binaire
    this->_env["SCRIPT_FILENAME="] = this->getPath();
    this->_env["SCRIPT_NAME="] = this->getPath();
        //Chemin vers le binaire + requete query. Facultatif ?
    this->_env["REQUEST_URI="] = construct_path(_location.getCgiBinary(), _location);
    this->_env["REMOTE_HOST="] = "";               //Nom d'hote du client. Vide si pas connu
    this->_env["REMOTE_ADDR="] = "127.0.0.1";      //IP du client
    this->_env["QUERY_STRING="] = "";              //A remplacer en fonction du contenu recu

    this->_env["CONTENT_LENGTH="] = int_to_string(this->_body.size());
    this->_env["CONTENT_TYPE="] = "text/plain"; //Dans header ?
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

void MethodCgi::extractHeader()
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