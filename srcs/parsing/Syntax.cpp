/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Syntax.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnoaille <lnoaille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/26 15:09:37 by dchampda          #+#    #+#             */
/*   Updated: 2021/09/07 16:44:55 by lnoaille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Syntax.hpp"
# include "../includes/utils.hpp"

bool isIP( std::string line, char c )
{
    size_t start = 0;
    size_t dot_pos;
    size_t dot_nb = 0;

    for (int i = 0; line[i] != c; i++)  //Verification que chiffres only OU . de separation
    {
        if (!isdigit(line[i]) && line[i] != '.')
            return false;
    }
    dot_pos = line.find(".");
    if (dot_pos == std::string::npos)
        return false;
    while (dot_pos != std::string::npos)
    {
        if (dot_pos - start <= 0 || dot_pos - start > 3 || dot_nb > 3)
            return false;
        dot_nb++;
        start = dot_pos + 1;
        if (!isdigit(line[start]))
            return false;
        dot_pos = line.find(".", start);
    }
    if ((line.find(":") != std::string::npos && line.find(":") - start > 3)
        || (line.find(":") == std::string::npos && line.size() - start > 3))
        return false;
    if (dot_nb == 3)
        return true;
    return false;
}

bool isPort( std::string line )
{
    for (int i = 0; line[i]; i++)
    {
        if (!isdigit(line[i]))
            return false;
    }
    return true;
}


bool isExtension( std::string ext )
{
    if (ext[0] != '.')
        return false;
    for (size_t i = 1; i < ext.size(); i++)
    {
        if (!isalpha(ext[i]))
            return false;
    }
    return true;
}

bool isAcceptableName( std::string line )
{
    for (int i = 0; line[i]; i++)
    {
        if (line[i] == '"' && line[i + 1] == '"')
        {
            if (line[i + 2])
                return false;
            return true;
        }
        if (!isalnum(line[i]) && line[i] != '.' && line[i] != '-')
            return false;
    }
    return true;
}

bool isAcceptableURI( std::string line )
{
    for (size_t i = 0; i < line.size(); i++)
    {
        if (!isalnum(line[i]) && line[i] != '/' && line[i] != '.' && line[i] != '+'
            && line[i] != '-' && line[i] != '_' && line[i] != '%' && line[i] != '$')
            return false;
    }
    return true;
}