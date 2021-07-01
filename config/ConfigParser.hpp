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

# include "serverConfig"
# include "locationConfig"


class ConfigParser
{
    public:
        ConfigParser( char *filepath );
        //ConfigParser( ConfigParser const & src );
        //ConfigParser & operator=( ConfigParser const & rhs );
        ~ConfigParser();

        void parser( std::string const & file );
        void checkIfIsServerBlock();
        void parseServerName();

        int treatServerBlock();

        std::ifstream openConfigFile( std::string const & file );

        std::vector<std::string> FormattingLine( std::ifstream & file );
        std::vector<std::string> splitLine( std::string line );

        std::string trimComment( std::string line );
        std::string trimStartAndEndWhitespaces( std::string line );


    private:
        ConfigParser();

        std::ifstream           _configFile;

        std::vector<serverConfig>   _server;
        std::vector<locationConfig> _location;

};

//class Location {}

#endif
