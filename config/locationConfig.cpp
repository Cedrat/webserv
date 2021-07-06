#include "locationConfig.hpp"

locationConfig::locationConfig()
{

}

locationConfig::~locationConfig() { }





/*************************************************************
Config - setting
*************************************************************/
void locationConfig::setLocationDirective( std::vector<std::string> line )
{
    this->_location = line[1];
}






/*************************************************************
Config - checking
*************************************************************/
bool locationConfig::checkLocationData()
{

    return true;
}



/*************************************************************
Getters
*************************************************************/
std::string locationConfig::getLocation() const
{
    return this->_location;
}

