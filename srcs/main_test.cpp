/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchampda <dchampda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/21 14:39:55 by dchampda          #+#    #+#             */
/*   Updated: 2021/06/21 14:39:55 by dchampda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <vector>

int main()
{
  int host;
  inet_pton(AF_INET, "127.0.0.1", &host);

  std::vector<int> ports;
  ports.push_back(7994);
  ports.push_back(7995);

  Server s1(ports, host);

  return 0;
}
