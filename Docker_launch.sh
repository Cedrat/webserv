#! /bin/bash

cp /usr/bin/php-cgi /srcs/www/cgi_bin/
cp /usr/bin/perl /srcs/www/cgi_bin/

cd srcs/

make

/bin/bash
