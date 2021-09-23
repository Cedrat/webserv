#include <ctime>
#include <string>
#include <iostream>

std::string date_string()
{
    struct tm data_time;
    time_t time = std::time(0);
    tm actual_time = *localtime(&time);
    char buffer[80];

    strftime(buffer, 80, "%a, %d %b %Y %X %Z", &actual_time);

    std::string date(buffer);
    date = "Date: " + date;
    return(date);
}