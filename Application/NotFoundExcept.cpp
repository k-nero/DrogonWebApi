#include "pch.h"
#include "NotFoundExcept.h"

NotFoundExcept::NotFoundExcept()
{
	this->message = "Not found";
}

NotFoundExcept::NotFoundExcept(std::string message)
{
	this->message = message;
}

const char* NotFoundExcept::what() const throw()
{
	return this->message.c_str();
}
