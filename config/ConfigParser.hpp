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

# include "../srcs/Config.hpp"
# include "locationConfig.hpp"
# include "../includes/Formatting.hpp"


class ConfigParser
{
    public:
        ConfigParser( char *filepath );
        //ConfigParser( ConfigParser const & src );
        //ConfigParser & operator=( ConfigParser const & rhs );
        ~ConfigParser();

        void parser( std::string const & file );
        bool treatServerBlock();
        bool treatLocationBlock( std::vector<std::string> line );

        bool closeServerBlock( std::vector<std::string> line, Config * server );
        bool closeLocationBlock( std::vector<std::string> line, Location * location );

        bool isServerProperty( std::string line );
        bool isLocationProperty( std::string line );
        bool isLocation( std::vector<std::string> line );

        bool addServerProperty( std::vector<std::string> line, Config * server );
        bool addLocationProperty( std::vector<std::string> line, Location * location );

        std::vector<Config>   getServer();
        Config                getOneServer( size_t i );

        void openConfigFile( std::string const & file );

        std::vector<std::string> FormattingLine( std::ifstream & file );


    private:
        ConfigParser();

        int                     _serverNb;
        int                     _locationNb;
        std::ifstream           _configFile;

        std::vector<Config>     _server;
        std::vector<Location>   _location;

};

#endif
