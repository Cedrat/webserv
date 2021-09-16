#ifndef AMETHOD_HPP
# define AMETHOD_HPP

# include "../includes/utils.hpp"

class AMethod 
{
	private: 
		int _fd;

		bool _header_sent;
		bool _request_sent;

		bool _is_finished;
		std::string _path;
		std::string _header;
		AField * 	_fields;

	public :
		AMethod(int fd, std::string path, std::string header);
		virtual ~AMethod() {};
		virtual void init() = 0;
		virtual void exec() = 0;

		void setHeaderSent(bool const);
		void setRequestSent(bool const);
		void setIsFinished(bool const);
		void setPath(std::string const);
		void setHeader(std::string const);

		bool const & getHeaderSent();
		bool const & getRequestSent();
		bool const & getIsFinished();
		std::string const & getPath();
		std::string const & getHeader();
		int const & getFd();
};

#endif