#include "../includes/utils.hpp"



class AField
{
    private : 
        std::string _path;
        std::string _header;
        std::string _host_name;
};

class GetField : public AField
{

};

class PostField : public AField
{
    private: 
        std::string _transfert_encoding;
        int         _content_length;
    
    public : 
        std::string const & getTransfertEncoding(){
            return (_transfert_encoding);
        }
        void setTransfertEncoding(std::string const transfert_encoding){
            _transfert_encoding = transfert_encoding;
        }
        int const & getContentLength(){
            return (_content_length);
        }
        void setContentLength(int const length){
            _content_length = length;
        }
};

class DeleteField : public AField
{

};

class RedirectField : public AField
{
    private :
        std::string _location; // Pas besoin? Si on prepare les headers a l'avance
};

class ErrorField : public AField
{

};
