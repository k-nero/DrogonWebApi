#pragma once

#ifdef INFRASTRUCTURE_EXPORTS
#define INFRASTRUCTURE_API __declspec(dllexport)
#else
#define INFRASTRUCTURE_API __declspec(dllimport)
#endif

#include <string>
#include <bcrypt.h>
#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)



class INFRASTRUCTURE_API BcryptHash
{
public:
	BcryptHash();
	~BcryptHash();
	static std::string HashPassword(std::string password);
	static bool VerifyPassword(std::string password, std::string hash);
};

