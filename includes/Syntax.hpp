/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Syntax.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnoaille <lnoaille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/26 15:09:26 by dchampda          #+#    #+#             */
/*   Updated: 2021/09/07 16:44:40 by lnoaille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYNTAX_HPP
# define SYNTAX_HPP

#include <string>

    bool isIP( std::string line, char c );
    bool isPort( std::string line );
    bool isExtension( std::string ext );

    bool isAcceptableName( std::string line );
    bool isAcceptableURI( std::string line );

#endif
