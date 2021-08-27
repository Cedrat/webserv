/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Syntax.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchampda <dchampda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/26 15:09:26 by dchampda          #+#    #+#             */
/*   Updated: 2021/07/26 15:09:28 by dchampda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYNTAX_HPP
# define SYNTAX_HPP

# include "fonction.hpp"

    bool isIP( std::string line, char c, int host );
    bool isPort( std::string line );
    bool isExtension( std::string ext );

    bool isAcceptableName( std::string line );
    bool isAcceptableURI( std::string line );

#endif
