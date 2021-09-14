


1er temps; 

on recoit une requete -> Verification syntaxe;

2eme temps;
requete finie "\r\n\r\n" -> Verification doublon;


Remplissage minimum des champs selon la méthode
GETFIELD DELETEFIELD POSTFIELD

GETVERIF DELETEVERIF POSTVERIF 

AMETHODS 
GET DELETE POST REDIRECTION                           ERREUR

class requestInProgress
{
    private:
    std::string _str_request;
    bool is_finished;

    public : 
        isFinished();
        AMethod *getField(); //Travail a l'interieur avec AField et AVerif
}
class GetField
{
    private :
        std::string _str_request;
        std::string _host;
}

class AVerif
{
    class Config;
    class Location;
    struct &pollfd;




}

class GetVerif : AVerif
{   
    private :
        
        int         _host;    //
        int         _port;    //créer une class SocketInfo;
        int         _fd;      //
        
        std::string _str_request;
        
        std::string _host_name;
        bool        _redirection;
        
        std::string _path_file; //construit avec alias + default_file si AI = OFF;

        bool        _cgi;
        bool        _auto_index;

    public : 
        GetVerif();
        ~GetVerif();

        AMethod *verif();
    
}


class AMethod
{
    private :
        class Config;   //
        class Location; // 
        struct &pollfd; // 
        int    _host;   //
        int    _port;   //

        bool _header_sent;
        bool _request_sent;
        
        std::string path;
    
    public :
        virtual ~AMethod() {};
        virtual init() = 0;
        virtual exec() = 0;
}

class Get
{   
    private :
        class Config;
        class Location;

        bool _header_sended;
        bool _request_sended;

    public : 
        Get();
        ~Get();

         void exec();
}

class Delete
{
    private :
        class Config;
        class Location;

        bool _header_sended;
        bool _request_sended;

    public : 
        Delete();
        ~Delete();

        void exec();
}

class Post : public IMethod
{
    private :
        bool _request_received;
        bool _chunked_request;
        int  _content_length;

    public :
        void exec();
}