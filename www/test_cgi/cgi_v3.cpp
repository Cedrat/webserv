#include <unistd.h>
#include <iostream>
#include <sys/wait.h>
#include <string>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <cstdlib>
#include <fcntl.h>
#include <cstdio>
#include <cstring>
#include <map>

std::string create_tmp_file_name()
{
    //char _tmpFileName[] = "tmpXXXXXX";
    //mkstemp(_tmpFileName);

    //return _tmpFileName;
    return "";
}

std::string ft_read_cgi_file(int fd)
{
    char buffer[50 + 1] = {0};
    int ret;
    std::string new_str;

    if (fd < 0)
    {
        std::cerr << "Error opening tmp" << std::endl;
        exit(0);
    }

    while ((ret = read(fd, buffer, 50)) > 0)
    {
        std::cout << buffer << std::endl;
        new_str += buffer;
        memset(buffer, 0, 50);
    }
    //Check ret < 0 ?
    close(fd);

    return new_str;
}



std::string ft_cgi(std::string fileName, const char ** args, char ** env)
{
    int inpipefd[2];
    int outpipefd[2];
    pipe(inpipefd);
    pipe(outpipefd);


    std::string header;
    pid_t   pid;
    int     fd[2];

    if (pipe(fd) < 0)
    {
        std::cerr << "Pipe or tmp file opening failed" << std::endl;
        exit(0);
    }

    pid = fork();
    if (pid == -1)
    {
        std::cerr << "Error with fork()" << std::endl;
        //close(fd[0]);
        //close(fd[1]);
        exit(0);
    }
    else if (pid == 0)       //Child
    {
        dup2(outpipefd[0], STDIN_FILENO);
        dup2(inpipefd[1], STDOUT_FILENO);
        dup2(inpipefd[1], STDERR_FILENO);

        //arg[0] = script path  ||  arg[1] = requested file path  ||  arg[3] = NULL
        execve(args[0], const_cast<char *const *>(args), env);
            std::cerr << "Execve failed" << std::endl;
            exit(0);
    }
    else  //Parent
    {
        close(outpipefd[0]);
        close(inpipefd[1]);
    }

    char buffer[51];
    int ret;
    while((ret = read(inpipefd[0], buffer, 50)) > 0)
    {
        buffer[ret] = 0;
        header += buffer;
    }


    //header = ft_read_cgi_file(outpipefd[1]);
    return header;
}

void set_env_vars(std::map<std::string, std::string> & env)
{
    //Serveur
    env["REDIRECT_STATUS="] = "200";
    env["SERVER_SOFTWARE="] = "Webserv/1.0";
    env["SERVER_NAME="] = "127.0.0.1";      //A remplacer par server.getHost();
    env["GATEWAY_INTERFACE="] = "CGI/1.1";
    //Requete
    env["SERVER_PROTOCOL="] = "HTTP/1.1";
    env["SERVER_PORT="] = "7995";           //A remplacer par server.getPort();
    env["REQUEST_METHOD="] = "GET";         //A remplacer
    env["AUTH_TYPE="] = "Basic";            //A remplacer ?

    env["PATH_INFO="] = "/cgi.pl";         //A remplacer. Chemin vers le binaire. 
    env["PATH_TRANSLATED="] = "/cgi.pl";
    //env["PATH_INFO="] = "/php-cgi";         //A remplacer. Chemin vers le binaire. 
    //env["PATH_TRANSLATED="] = "/php-cgi";   //A remplacer. Chemin vers le binaire.
    env["SCRIPT_FILENAME="] = "test.php";   //A remplacer. Fichier à ouvrir avec le binaire
    env["SCRIPT_NAME="] = "test.php";       //Pareil que FILENAME. Facultatif ?

    //env["REQUEST_URI="] = "/php-cgi";       //A remplacer. Chemin vers le binaire + requete query. Facultatif ?
    env["REMOTE_HOST="] = "";               //Nom d'hote du client. Vide si pas connu
    env["REMOTE_ADDR="] = "127.0.0.1";      //IP du client
    env["QUERY_STRING="] = "";              //A remplacer

    env["CONTENT_LENGTH="] = "0";           //Taille du body ?
    env["CONTENT_TYPE="] = "text/plain";    //Recuperer ça dans le header de la requete ?

    env["HTTP_ACCEPT="] = "text/*";         //Type de donnees MIME acceptees par le client
    env["HTTP_USER_AGENT="] = "Mozilla/91.0.1"; //Le navigateur utilise par le client
}

char ** set_env_as_char(std::map<std::string, std::string> & env)
{
    std::map<std::string, std::string>::const_iterator it = env.begin();
    char **new_env = new char*[env.size() + 1];
    int i = 0;

    while (it != env.end())
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

void free_env(char ** env)
{
    for(int i = 0; env[i]; i++)
        delete [] env[i];
    delete [] env;
}

int main(void)
{
    std::string header;

    //const char *args[3] = {"./../../../../.brew/bin/php-cgi", "/test.php", NULL};
    //const char *args[3] = {"./../../../../../../../usr/bin/php-cgi", "/test.php", NULL};
    const char *args[3] = {"./cgi.pl", "/test.php", NULL};

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

    //Init les variables d'env et les transformer en char**
    std::map<std::string, std::string> env_map;
    set_env_vars(env_map);
    char **env2;
    env2 = set_env_as_char(env_map);

    //Gestion de l'execution du cgi
    header = ft_cgi(create_tmp_file_name(), args, env2);
    std::cout << "Returned :\n" << std::endl;
    std::cout << header << std::endl;

    free_env(env2);

    return 0;
}


// /usr/bin/php-cgi

//Get = QUERY_STRING     - 1024 characters max
//Post = pipe un body sur stdin