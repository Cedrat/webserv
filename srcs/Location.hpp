#ifndef LOCATION_HPP
# define LOCATION_HPP

#include <iostream>
#include <vector>
#include <map>
#include "../includes/utils.hpp"
# include "../includes/Syntax.hpp"

class Location
{
    typedef bool(Location::*checks)();

    private :
        std::string                         _location;
        std::string                         _root;
        std::vector<std::string>            _methods;
        bool                                _autoindex;
        bool                                _set_autoindex;
        std::string                         _default_file; // = index
        std::string                         _upload_folder;
        std::string                         _redirect;
        std::map<std::string, std::string>  _cgi;

    public : 
        Location();
        ~Location();

        //Assignation des valeurs
        void setLocationDirective( std::vector<std::string> line );
        void setRoot( std::vector<std::string> line );
        void setAutoindex( std::vector<std::string> line );
        void setMethods( std::vector<std::string> line );
        void setDefaultFile( std::vector<std::string> line );
        void setUploadFolder( std::vector<std::string> line );
        void setCgi( std::vector<std::string> line );
        void setRedirect( std::vector<std::string> line );
        void setUncalledDirectives();

        //Verification des valeurs
        bool checkLocationData();
        bool checkLocation();
        bool checkRoot();
        bool checkDefaultFile();
        bool checkMethods();
        bool checkUploadFolder();
        bool checkCgi();
        bool checkRedirect();
        bool isEqual(const Location & rhs) const;

        //Recuperation des valeurs
        std::string                getLocation() const;
        std::string                getRoot() const;
        std::string                getUploadFolder() const;
        bool                       getAutoIndex() const;
        std::vector<std::string>   getMethods() const;
        std::string                getDefaultFile() const;
        std::map<std::string, std::string> getCgi() const;
        std::string                getCgiBinary() const;
        std::string                getCgiExtension() const;
        std::string                getRedirect() const;

        void                        debug();

        bool                        checkIfMethodIsPresent(std::string method) const;
};

#endif