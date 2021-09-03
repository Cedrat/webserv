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

std::string create_tmp_file_name()
{
    char _tmpFileName[] = "tmpXXXXXX";
    mkstemp(_tmpFileName);

    return _tmpFileName;
}

std::string ft_read_cgi_file(std::string file)
{
    int ret;
    char buffer[50 + 1] = {0};
    std::string new_str;

    int fd = open(file.c_str(), O_RDONLY);
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
    remove(file.c_str());

    return new_str;
}

std::string ft_cgi(std::string fileName, const char ** args, const char ** env)
{
    std::string header;

    pid_t   pid;
    int     status;

//Creation du fichier temporaire
//Creation des pipes
    int fd[2];
    int tmp = open(fileName.c_str(), O_CREAT | O_RDWR | O_TRUNC | O_NONBLOCK, S_IRUSR | S_IWUSR);

    if (pipe(fd) < 0 || tmp < 0)
    {
        std::cerr << "Pipe or tmp file opening failed" << std::endl;
        exit(0);
    }

    pid = fork();
    if (pid == 0)       //Child
    {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);

        dup2(tmp, STDOUT_FILENO);

        //arg[0] = script path  ||  arg[1] = requested file path  ||  arg[3] = NULL
        if ((execve(args[0], const_cast<char *const *>(args), const_cast<char**>(env))) < 0)
        {
            std::cerr << "Execve failed" << std::endl;
            close(STDIN_FILENO);
            close(fd[0]);
            close(tmp);
            remove(fileName.c_str());
            exit(0);
        }
    }
    else if (pid > 0)   //Parent
    {
        close(fd[0]);
        //Ecrire query dans fd[1]
        close(fd[1]);

        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            std::cout << "\nExit status : " << WEXITSTATUS(status) << std::endl;
    }
    else
    {
        std::cerr << "Error with fork()" << std::endl;
        close(fd[0]);
        close(fd[1]);
        exit(0);
    }
    header = ft_read_cgi_file(fileName.c_str());
    return header;
}


int main(void)
{
    std::string header;

    //const char *args[3] = {"./../../../../.brew/bin/php-cgi", "/test.php", NULL};
    const char *args[3] = {"./../../../../../../../usr/bin/php-cgi", "/test.php", NULL};
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
    header = ft_cgi(create_tmp_file_name(), args, env);
    std::cout << "Returned :\n" << std::endl;
    std::cout << header << std::endl;


    return 0;
}


// /usr/bin/php-cgi

//Get = QUERY_STRING
//Post = pipe un body sur stdin


//Ouvrir fichier = fd
//Rediriger fd pour recup data dans child
