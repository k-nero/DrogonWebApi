#pragma once

#ifdef INFRASTRUCTURE_EXPORTS
#define INFRASTRUCTURE_API __declspec(dllexport)
#else
#define INFRASTRUCTURE_API __declspec(dllimport)
#endif

#include <openssl/evp.h>
#include <string>
#include <openssl/rand.h>



class INFRASTRUCTURE_API Encrypt
{
public:
	Encrypt();
	~Encrypt();
	static std::string HashPassword(const std::string& password, const std::string& salt);
	static bool VerifyPassword(std::string password, std::string hash);
};

