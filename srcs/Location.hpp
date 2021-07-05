#ifndef LOCATION_HPP
# define LOCATION_HPP

#include <iostream>
#include <vector>
#include <map>

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

        void setAutoIndex(bool auto_index);
};

#endif