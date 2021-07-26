#ifndef LOCATIONCONFIG_HPP
# define LOCATIONCONFIG_HPP

# include <vector>
# include <map>
# include <string>
# include <iostream>
# include <string>
# include <sys/stat.h>
# include <fstream>

class locationConfig
{
    typedef bool(locationConfig::*checks)();

    public:
        locationConfig();
        //locationConfig( locationConfig const & src );
        //locationConfig & operator=( lcoationConfig const & rhs );
        ~locationConfig();


        void setLocationDirective( std::vector<std::string> line );
        void setRoot( std::vector<std::string> line );
        void setAutoindex( std::vector<std::string> line );
        void setMethods( std::vector<std::string> line );
        void setIndex( std::vector<std::string> line );
        void setUploadFolder( std::vector<std::string> line );
        void setCgi( std::vector<std::string> line );

        void setUncalledDirectives();

        bool checkLocationData();
        bool checkLocation();
        bool checkRoot();
        bool checkIndex();
        bool checkMethods();
        bool checkUploadFolder();
        bool checkCgi();
        bool isExtension( std::string ext );


        std::string getLocation() const;
        std::string getRoot() const;
        bool getAutoindex() const;
        std::vector<std::string> getMethods() const;
        std::vector<std::string> getIndex() const;
        std::string getUploadFolder() const;
        std::map<std::string, std::string> getCgi() const;

        void initArrayChecks();
        

    private:
        std::string                         _location;
        std::string                         _root;
        bool                                _autoindex;
        std::vector<std::string>            _methods;
        std::vector<std::string>            _index;
        std::string                         _upload_folder;
        std::map<std::string, std::string>  _cgi;

        checks _checks[6] = {&locationConfig::checkLocation, &locationConfig::checkRoot, 
            &locationConfig::checkIndex, &locationConfig::checkMethods, &locationConfig::checkUploadFolder, 
            &locationConfig::checkCgi};

};

#endif