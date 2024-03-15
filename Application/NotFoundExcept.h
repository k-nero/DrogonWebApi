#pragma once
#include <exception>
#include <string> 

class NotFoundExcept : public std::exception
{
public:
	NotFoundExcept();
	NotFoundExcept(std::string message);
	~NotFoundExcept() = default;
	const char* what() const throw();
private:
	std::string message;
};

