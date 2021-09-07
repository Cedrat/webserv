#include <unistd.h>
#include <iostream>
#include <sys/wait.h>
#include <string>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <cstdlib>

std::string generateTempFileName()
{
    std::ostringstream ss;
    ss << "/tmp/" << "00";  //Besoin d'une fonction pour donner nom aleatoire
    return ss.str();    
}



std::string ft_cgi(const char ** args, const char ** env)
{
    std::string header;

    pid_t   pid;
    int     status;


//Creation de fichiers temporaires
//Recup de leurs fd pour le fork
    FILE* fIn = tmpfile();
    //FILE* fOut = tmpfile();
    FILE* fOut = fopen("test2.txt", "w");
    int fdIn = fileno(fIn);
    int fdOut = fileno(fOut);

    //int fd = open("test.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

    //Write le body dans fdin ?
    write(fdIn, "test", 4);

    pid = fork();
    if (pid == 0)       //Child
    {
        //Probleme ICI
        //dup2(fdIn, STDIN_FILENO);
        dup2(fdOut, STDOUT_FILENO);

        //arg[0] = script path  ||  arg[1] = requested file path  ||  arg[3] = NULL
        if ((execve(args[0], const_cast<char *const *>(args), const_cast<char**>(env))) < 0)
        {
            std::cerr << "Execve failed" << std::endl;
            close(fdIn);
            close(fdOut);
            exit(0);
        }
    }
    else if (pid > 0)   //Parent
    {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            std::cout << "\nExit status : " << WEXITSTATUS(status) << std::endl;

        //Recuperation du fichier dans std::string header
        int ret = 1;
        char buffer[50] = {0};
        freopen("test2.txt", "r", fOut);
        while (ret > 0)
        { 
            ret = read(fdOut, buffer, 49);
            buffer[ret] = '\0';
            header += buffer;
        }
    }
    else
    {
        std::cerr << "Error with fork()" << std::endl;
        close(fdIn);
        close(fdOut);
        remove("test2.txt");
        exit(0);
    }

    //Fermeture des fds et suppression du fichier temporaire
    close(fdIn);
    close(fdOut);
    fclose(fOut);
    remove("test2.txt");

    return header;
}


int main(void)
{
    std::string header;

    const char *args[3] = {"./../../../../.brew/bin/php-cgi", "/test.php", NULL};
    const char *env[21] = {"SERVER_SOFTWARE=Webserv/1.0", "SERVER_NAME=127.0.0.1", "GATEWAY_INTERFACE=CGI/1.1",
                "REDIRECT_STATUS=200", "SERVER_PROTOCOL=HTTP/1.1", "SERVER_PORT=7995", "REQUEST_METHOD=GET",
                "PATH_INFO=/php-cgi", "PATH_TRANSLATED=/php-cgi", "SCRIPT_FILENAME=test.php",
                "AUTH_TYPE=Basic", "REMOTE_HOST=", "REMOTE_ADDR=127.0.0.1", "CONTENT_LENGTH=0",
                "CONTENT_TYPE=text/plain", "QUERY_STRING=", "HTTP_ACCEPT=text/*", "HTTP_HOST=localhost",
                "HTTP_USER_AGENT=Mozilla/91.0.1", "HTTP_COOKIE=", NULL};

/***
    const char *args[3] = {"./cgi_tester", "/youpi.bla", NULL};
    const char *env[21] = {"SERVER_SOFTWARE=Webserv/1.0", "SERVER_NAME=127.0.0.1", "GATEWAY_INTERFACE=CGI/1.1",
                "REDIRECT_STATUS=200", "SERVER_PROTOCOL=HTTP/1.1", "SERVER_PORT=7995", "REQUEST_METHOD=GET",
                "PATH_INFO=/cgi_tester", "PATH_TRANSLATED=/cgi_tester", "SCRIPT_FILENAME=youpi.bla",
                "AUTH_TYPE=Basic", "REMOTE_HOST=", "REMOTE_ADDR=127.0.0.1", "CONTENT_LENGTH=0",
                "CONTENT_TYPE=text/plain", "QUERY_STRING=", "HTTP_ACCEPT=text/*", "HTTP_HOST=localhost",
                "HTTP_USER_AGENT=Mozilla/91.0.1", "HTTP_COOKIE=", NULL};
***/

    //Gestion de l'execution du cgi
    header = ft_cgi(args, env);
    std::cout << "Returned :\n" << std::endl;
    std::cout << header << std::endl;


    return 0;
}


// /usr/bin/php-cgi

//Get = QUERY_STRING
//Post = pipe un body sur stdin


//Ouvrir fichier = fd
//Rediriger fd pour recup data dans child
