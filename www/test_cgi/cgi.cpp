#include <unistd.h>
#include <iostream>
#include <sys/wait.h>
#include <string>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstdlib>

std::string ft_cgi(char ** args, char **env)
{
    std::string header;

    pid_t   pid;
    int     status;


//Creation de fichiers temporaires
//Recup de leurs fd pour le fork
    FILE* fIn = tmpfile();
    //FILE* fOut = tmpfile();
    FILE* fOut = fopen("test2.txt", "w"); // ->Ecrit bien dedans
    int fdIn = fileno(fIn);
    int fdOut = fileno(fOut);


    //Write le body dans fdin ?
    //write(fdOut, _body, strlen(_body));

    pid = fork();
    if (pid == 0)       //Child
    {
        std::cout << "\nEnter child process" << std::endl;

        //Probleme ICI
        //dup2(fdIn, 0);
        dup2(fdOut, 1);

        //arg[0] = script path  ||  arg[1] = requested file path  ||  arg[3] = NULL
        if ((execve(args[0], args, env)) < 0)
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
            std::cout << "Exit status : " << WEXITSTATUS(status) << std::endl;

        int ret = 1;
        char buffer[50] = {0};
        while (ret > 0)
        {
           ret = read(fdOut, buffer, 49);
           header += buffer;
        }
    }
    else
    {
        std::cerr << "Error with fork()" << std::endl;
        close(fdIn);
        close(fdOut);
        exit(0);
    }

    std::cout << "Closing fds" << std::endl;
    close(fdIn);
    close(fdOut);

    std::cout << "\nReturned :" << std::endl;
    std::cout << header << std::endl;

    return header;
}


int main(void)
{
    char *args[3] = {"./cgi_tester", "/youpi.bla", NULL};
    char *env[20] = {"SERVER_SOFTWARE=Webserv/1.0", "SERVER_NAME=127.0.0.1", "GATEWAY_INTERFACE=CGI/1.1",
                "REDIRECT_STATUS=200", "SERVER_PROTOCOL=HTTP/1.1", "SERVER_PORT=7995", "REQUEST_METHOD=GET",
                "PATH_INFO=/cgi_tester", "PATH_TRANSLATED=/youpi.bla", "SCRIPT_FILENAME=/youpi.bla",
                "AUTH_TYPE=Basic", "REMOTE_HOST=", "REMOTE_ADDR=127.0.0.1", "CONTENT_LENGTH=0",
                "CONTENT_TYPE=text/plain", "QUERY_STRING=", "HTTP_ACCEPT=text/*", "HTTP_HOST=localhost",
                "HTTP_USER_AGENT=Mozilla/91.0.1", NULL};




    //Gestion de l'execution du cgi
    ft_cgi(args, env);

    
    //Pas necessaire, mais pour nettoyage just in case

    return 0;
}


// /usr/bin/php-cgi

//Get = QUERY_STRING
//Post = pipe un body sur stdin


//Ouvrir fichier = fd
//Rediriger fd pour recup data dans child
