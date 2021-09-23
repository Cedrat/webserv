#include "CGI.hpp"
# include <fstream>

CGI::CGI()
{
    //Preparation de l'environnement
    this->_tmpOut = createTmpFile();
    setEnv();

    //Lancement du processus
    processCGI();
}

CGI::~CGI() { }



/**************************************************************
Main CGI process
**************************************************************/
void CGI::processCGI()
{
    std::string header;

    const char *args[3] = {"./cgi.pl", "/test.php", NULL};
    //const char *args[3] = {"./../../../../.brew/bin/php-cgi", "/test.php", NULL};
    //const char *args[3] = {"./../../../../../../../usr/bin/php-cgi", "/test.php", NULL};

/***    
*    const char *args[3] = {"./ubuntu_cgi_tester", "/youpi.bla", NULL};
*    const char *args[3] = {"./cgi_tester", "/youpi.bla", NULL};
*    const char *env[21] = {"SERVER_SOFTWARE=Webserv/1.0", "SERVER_NAME=127.0.0.1", "GATEWAY_INTERFACE=CGI/1.1",
*                "REDIRECT_STATUS=200", "SERVER_PROTOCOL=HTTP/1.1", "SERVER_PORT=7995", "REQUEST_METHOD=GET",
*                "PATH_INFO=/cgi_tester", "PATH_TRANSLATED=/cgi_tester", "SCRIPT_FILENAME=youpi.bla",
*                "AUTH_TYPE=Basic", "REMOTE_HOST=", "REMOTE_ADDR=127.0.0.1", "CONTENT_LENGTH=0",
*                "CONTENT_TYPE=text/plain", "QUERY_STRING=", "HTTP_ACCEPT=text/*", "HTTP_HOST=localhost",
*                "HTTP_USER_AGENT=Mozilla/91.0.1", "HTTP_COOKIE=", NULL};
***/

    //Transformer les variables d'env en char**
    char **env;
    env = convertEnv();

    //Gestion de l'execution du cgi
    if (execCGI(args, env) == -1)
    {
        std::cerr << "Error during CGI execution" << std::endl;
        return ;
    }

    header = readCgiFile();
    std::cout << "Returned :\n" << std::endl;
    std::cout << header << std::endl;

    freeEnv(env);
}

int CGI::execCGI( const char ** args, char ** env )
{
    std::string header;
    pid_t       pid;
    int         status;
    
    std::fstream file(this->_tmpOut.c_str(), std::ios_base::trunc 
            | std::ios_base::in | std::ios_base::out);
    file.close();
    //put data (body) dans le file
    int input_fd = open(this->_tmpOut.c_str(), O_RDWR);
    int output_fd = open(this->_tmpOut.c_str(), O_RDWR);

    //Creation du child process
    pid = fork();
    if (pid == -1)          //Erreur
    {
        std::cerr << "Error with fork()" << std::endl;
        close(input_fd);
        close(output_fd);
        return -1;
    }
    else if (pid == 0)       //Child
    {
        dup2(input_fd, STDIN_FILENO);
        dup2(output_fd, STDOUT_FILENO);

        //arg[0] = script path  ||  arg[1] = requested file path  ||  arg[3] = NULL
        if ((execve(args[0], const_cast<char *const *>(args), env)) < 0)
        {
            std::cerr << "Execve failed" << std::endl;
            close(input_fd);
            close(output_fd);
            remove(this->_tmpOut.c_str());
            return -1;
        }
    }
    else  //Parent
    {
        close(input_fd);
        close(output_fd);     
    }
        
    waitpid(pid, &status, 0);

    return 1;
}

std::string CGI::readCgiFile()
{
    int ret;
    char buffer[50 + 1] = {0};
    std::string new_str;

    int fd = open(this->_tmpOut.c_str(), O_RDONLY);
    if (fd < 0)
    {
        std::cerr << "Error opening tmp" << std::endl;
        exit(0);
    }

    while ((ret = read(fd, buffer, 50)) > 0)
    {
        new_str += buffer;
        memset(buffer, 0, 50);
    }
    //Check ret < 0 ?
    close(fd);
    remove(this->_tmpOut.c_str());

    return new_str;
}




/**************************************************************
Setters
**************************************************************/
void CGI::setEnv()
{
    // Serveur
    this->_env["REDIRECT_STATUS="] = "200";
    this->_env["SERVER_SOFTWARE="] = "Webserv/1.0";
    this->_env["SERVER_NAME="] = "127.0.0.1";      //A remplacer par server.getHost();
    this->_env["GATEWAY_INTERFACE="] = "CGI/1.1";
    // Requete
    this->_env["SERVER_PROTOCOL="] = "HTTP/1.1";
    this->_env["SERVER_PORT="] = "7995";           //A remplacer par server.getPort();
    this->_env["REQUEST_METHOD="] = "GET";         //A remplacer en fonction de requete recue
    this->_env["AUTH_TYPE="] = "Basic";            //A remplacer ?

    //this->_env["PATH_INFO="] = "/php-cgi";         //A remplacer. Chemin vers le binaire. 
    //this->_env["PATH_TRANSLATED="] = "/php-cgi";   //A remplacer. Chemin vers le binaire.
    this->_env["PATH_INFO="] = "/cgi.pl";
    this->_env["PATH_TRANSLATED="] = "/cgi.pl";

    this->_env["SCRIPT_FILENAME="] = "test.php";   //A remplacer. Fichier à ouvrir avec le binaire
    this->_env["SCRIPT_NAME="] = "test.php";       //Pareil que FILENAME. Facultatif ?

    this->_env["REQUEST_URI="] = "/php-cgi";       //A remplacer. Chemin vers le binaire + requete query. Facultatif ?
    this->_env["REMOTE_HOST="] = "";               //Nom d'hote du client. Vide si pas connu
    this->_env["REMOTE_ADDR="] = "127.0.0.1";      //IP du client
    this->_env["QUERY_STRING="] = "";              //A remplacer en fonction du contenu recu

    this->_env["CONTENT_LENGTH="] = "0";           //Taille du body ?
    this->_env["CONTENT_TYPE="] = "text/plain";    //Recuperer ça dans le header de la requete ?
    // Client
    this->_env["HTTP_ACCEPT="] = "text/*";         //Type de donnees MIME acceptees par le client
    this->_env["HTTP_USER_AGENT="] = "Mozilla/91.0.1"; //Le navigateur utilise par le client
}



/**************************************************************
Utils
**************************************************************/
std::string CGI::createTmpFile()
{
    char _tmpFileName[] = "tmpXXXXXX";
    mkstemp(_tmpFileName);

    return _tmpFileName;
}

void CGI::freeEnv(char ** env)
{
    for(int i = 0; env[i]; i++)
        delete [] env[i];
    delete [] env;
}

char ** CGI::convertEnv()
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
Getters
**************************************************************/
std::map<std::string, std::string> CGI::getEnv()
{
    return this->_env;
}














// /usr/bin/php-cgi

//Get = QUERY_STRING     - 1024 characters max
//Post = pipe un body sur stdin


int main()
{
    CGI test;

    return 0;
}