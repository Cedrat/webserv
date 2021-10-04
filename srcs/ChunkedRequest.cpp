#include "../includes/utils.hpp"
#include "ChunkedRequest.hpp"


static bool is_hexa_base(char i)
{
    if (('0' <= i && i <= '9') || ('A' <= i && i <= 'F'))
        return (TRUE);
    return (FALSE);
}

ChunkedRequest::ChunkedRequest() : _nb_bytes_to_treat(0)
{

}

ChunkedRequest::~ChunkedRequest()
{

}

void ChunkedRequest::addData(std::string data)
{
    _data_not_processed+= data;
}

void ChunkedRequest::processData()
{
    Info data = good_data;

    while (_data_not_processed.size() != 0)
    {
        if (_nb_bytes_to_treat == 0)
        {
            data = checkNextDataIsByteToTreat();
            if (data == incorrect_data)
                break;
            else if (data == missing_data)
                break;
            else if (data == good_data)
            {
               extractNbBytesToRead();
                if (_nb_bytes_to_treat == 0)
                {
                    data = all_data_read;
                    break;
                }
               treatData();
            }
        }
    }
}

Info ChunkedRequest::checkNextDataIsByteToTreat()
{
    if (_data_not_processed[0] == '\0' || _data_not_processed[1] == '\0')
        return (missing_data);
    else if (_data_not_processed[0] != '\r' || _data_not_processed[1] != '\n')
        return (incorrect_data);

    int i = 2;
    while( _data_not_processed[i] != '\0' && _data_not_processed[i] != '\r')
    {
        if (is_hexa_base(_data_not_processed[i]) == FALSE)
            return (incorrect_data);
        i++;
    }
    if (_data_not_processed[i] == '\0' || _data_not_processed[i + 1] == '\0')
        return (missing_data);
    else if (_data_not_processed[i] == '\r' && _data_not_processed[i + 1] == '\n')
        return (good_data);
    return (incorrect_data);
}

void ChunkedRequest::extractNbBytesToRead()
{
    std::string str_nb_bytes_next;

    _data_not_processed.erase(0, 2);
    str_nb_bytes_next = _data_not_processed.substr(0,_data_not_processed.find("\r\n"));
    _data_not_processed.erase(0, _data_not_processed.find("\r\n") + 2);
        
    _nb_bytes_to_treat = atoi_hexa(str_nb_bytes_next);    
}

void ChunkedRequest::treatData()
{
    if (_nb_bytes_to_treat > _data_not_processed.size())
    {
        _processed_data += _data_not_processed;
        _nb_bytes_to_treat -= _data_not_processed.size();
        _data_not_processed = "";
    }
    else
    {
       _processed_data += _data_not_processed.substr(0, _nb_bytes_to_treat);
       _data_not_processed.erase(0, _nb_bytes_to_treat);
       _nb_bytes_to_treat = 0; 
    }
}

void ChunkedRequest::writeProcessedData(int fd)
{
    write(fd, _processed_data.c_str(), _processed_data.size());
};

// processed_chunk += request.substr(0, nb_bytes_next);
//         request.erase(0, nb_bytes_next);

int main()
{
    ChunkedRequest test;

    test.addData("\r\n26\r\n"
                "Voici les donnees \r\n2\r\n mier morceau\r\n\r\n"
                "1C\r\n"
                "et voici un second morceau\r\n\r\n"
                "20\r\n"
                "et voici deux derniers morceaux \r\n"
                "12\r\n"
                "sans saut de ligne\r\n"
                "0\r\n"
                "\r\n");
    test.processData();
    test.writeProcessedData(1);
}