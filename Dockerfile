# [Download image]
FROM ubuntu:18.04
MAINTAINER Loic Noaille & Diane Champdavoine

# [Download packages]
RUN apt-get update && apt-get install -y \
	apt-utils \
	clang \
	make \
	vim \
	php7.2-cgi \
	curl \
	&& rm -rf /var/lib/apt/lists/*

# [Copy files]
RUN mkdir /srcs/
COPY . /srcs/

CMD bash srcs/Docker_launch.sh
