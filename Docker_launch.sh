#! /bin/bash

cp /usr/bin/php-cgi /srcs/www/test_cgi/
cp /usr/bin/perl /srcs/www/test_cgi/

cd srcs/

make

/bin/bash
