#ifndef LOCATION_HPP
# define LOCATION_HPP

#include <iostream>
#include <vector>
#include <map>
#include "../includes/utils.hpp"

class Location
{
    private :
        std::string                         _location;
        std::string                         _root;
        std::vector<std::string>            _methods;
        bool                                _autoindex;
        std::string                         _default_file; 
        std::string                         _upload_folder;

        std::map<std::string, std::string>  _cgi; //???

    public : 
        Location();
        ~Location();

        void    setLocation(std::string location);
        void    addMethod(std::string method);
        void    setAutoIndex(bool _autoindex_on);
        void    setDefaultFile(std::string default_file);
        void    setUploadFolder(std::string upload_folder);
        void    setRoot(std::string root);

        bool                        getAutoIndex(void) const;
        std::string                 getRoot(void) const;
        std::string                 getLocation(void) const;
        std::string                 getDefaultFile(void) const;
        std::string                 getUploadFolder(void) const;
        std::vector<std::string>    getMethods(void) const;

        bool                        checkIfMethodIsPresent(std::string method) const;
};

#endif