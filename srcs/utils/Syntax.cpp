/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Syntax.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchampda <dchampda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/26 15:09:37 by dchampda          #+#    #+#             */
/*   Updated: 2021/07/26 15:09:38 by dchampda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../includes/Syntax.hpp"

bool isIP( std::string line, char c, int host )
{
    size_t start = 0;
    size_t dot_pos;
    size_t dot_nb = 0;

    if ((dot_pos = line.find("localhost")) != std::string::npos)
    {
        if (line.at(dot_pos + 9) != ':')
            return false;
        inet_pton(AF_INET, "127.0.0.1", &host);
        return true;
    }
    for (int i = 0; line[i] != c; i++)
    {
        if (!isdigit(line[i]) && line[i] != '.')
            return false;
    }
    dot_pos = line.find(".");
    if (dot_pos == std::string::npos)
        return false;
    while (dot_pos != std::string::npos)
    {
        if (dot_pos - start <= 0 || dot_pos - start > 3)
            return false;
        dot_nb++;
        start = dot_pos + 1;
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