#include <ctime>
#include <string>
#include <iostream>

#define DATE_FORMAT "%a, %d %b %Y %X %Z"

std::string date_string()
{
    time_t time = std::time(0);
    tm actual_time = *localtime(&time);
    char buffer[80];

    strftime(buffer, 80, DATE_FORMAT, &actual_time);

    std::string date(buffer);
    date = "Date: " + date;
    return(date);
}