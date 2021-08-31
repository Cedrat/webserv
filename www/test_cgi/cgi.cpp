#include <unistd.h>
#include <iostream>
#include <sys/wait.h>
#include <string>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstdlib>

std::string ft_cgi(char ** args, char **env, std::ofstream & os)
{
    std::string header;

    pid_t   pid;
    int     status;


    //Write le body dans fdin ?

    pid = fork();
    if (pid == 0)       //Child
    {
        //dup2 stdin
        //dup2 stdout

        //arg[0] = script path  ||  arg[1] = requested file path  ||  arg[3] = NULL
        if(execve(args[0], args, env) < 0)
        {
            std::cerr << "Execve failed" << std::endl;
            //close(fdin);
            //close(fdout);
            exit(0);
        }
    }
    else if (pid > 0)   //Parent
    {
        waitpid(pid, &status, 0);

        //int ret = 1;
        //char buffer[50] = {0};
        //while (ret > 0)
        //{
        //   ret = read(fdout, buffer, 50 + 1);
        //   os << buffer;
        //   ou std::string body += buffer ?
        //}
    }
    else
    {
        std::cerr << "Error with fork()" << std::endl;
        //close(fdin);
        //close(fdout);
        exit(0);
    }

    //close(fdin);
    //close(fdout);
    //os.flush();
    //Besoin de vider ofstream ?

    /*     Si ofstream -> pour le recup dans une string
    std::stringstream ss;
    ss << os.rdbuf();
    header = ss.str();
    std::cout << header << std::endl;*/

    return header;
}


int main(void)
{
    char *args[3] = {"/cgi_tester", "/youpi.bla", NULL};
    char *env[] = {};


    //Créer fichier temporaire pour stocker au fur
    //et a mesure le contenu du cgi
    std::ofstream   os;
    os.open("test.txt", std::ifstream::in | std::ifstream::out | std::ifstream::app | std::ifstream::trunc); //Nom du fichier
    if (os.is_open())
    {
        std::cerr << "Error opening tmp" << std::endl;
        exit(0);
    }

    //Gestion de l'execution du cgi
    ft_cgi(args, env, os);

    


    return 0;
}


// /usr/bin/php-cgi

//Get = QUERY_STRING
//Post = pipe un body sur stdin
