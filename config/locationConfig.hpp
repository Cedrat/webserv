#ifndef LOCATIONCONFIG_HPP
# define LOCATIONCONFIG_HPP

# include <vector>
# include <map>
# include <string>

class locationConfig
{
    public:
        locationConfig();
        //locationConfig( locationConfig const & src );
        //locationConfig & operator=( lcoationConfig const & rhs );
        ~locationConfig();

        /*
        void setLocation();
        void setRoot();
        void setMethods();
        void setAutoindex();
        void setDefaultFile();
        void setUploadFolder();
        void setCgi();

        std::string getLocation() const;
        std::string getRoot() const;
        std::vector<std::string> getMethods() const;
        bool getAutoindex() const;
        std::string getDefaultFile();
        std::string getUploadFolder();
        std::map<std::string, std::string> getCgi() const;
        */

    private:
        std::string                 _location;
        std::string                 _root;
        std::vector<std::string>    _methods;
        bool                        _autoindex;
        std::string                 _default_file;
        std::string                 _upload_folder;

        std::map<std::string, std::string>  _cgi;

};

#endif