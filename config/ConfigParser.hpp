/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchampda <dchampda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/28 15:09:26 by dchampda          #+#    #+#             */
/*   Updated: 2021/06/28 21:26:17 by dchampda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

# include <string>
# include <cstring>
# include <vector>
# include <map>
# include <list>
# include <fstream>
# include <iostream>

# include "serverConfig.hpp"
# include "locationConfig.hpp"


class ConfigParser
{
    public:
        ConfigParser( char *filepath );
        //ConfigParser( ConfigParser const & src );
        //ConfigParser & operator=( ConfigParser const & rhs );
        ~ConfigParser();

        void parser( std::string const & file );
        void parseServerName();
        int treatServerBlock();

        void initServerProperties();
        void initLocationProperties();
        bool isServerProperty( std::string line );
        bool isLocationProperty( std::string line );
        bool addServerProperty( std::vector<std::string> line, serverConfig * server );
        bool addLocationProperty( std::vector<std::string> line, serverConfig server );

        std::ifstream openConfigFile( std::string const & file );

        std::vector<std::string> FormattingLine( std::ifstream & file );
        std::vector<std::string> splitLine( std::string line );
        std::string trimComment( std::string line );
        std::string trimStartAndEndWhitespaces( std::string line );
        std::string trimDotComa( std::string line );


    private:
        ConfigParser();

        int                     _serverNb;
        std::ifstream           _configFile;

        std::vector<serverConfig>   _server;
        std::vector<locationConfig> _location;

        std::vector<std::string>    _serverProperties;
        std::vector<std::string>    _locationProperties;

};

//class Location {}

#endif
