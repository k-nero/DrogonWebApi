#pragma once
#include <string>
#include "json/json.h"

class __declspec(dllexport) ApplicationUser
{
public:
	ApplicationUser();

	//Copy constructor
	ApplicationUser(const ApplicationUser& applicationuser);

	//Move constructor
	ApplicationUser(ApplicationUser&& applicationuser) noexcept;

	//Copy assignment operator
	ApplicationUser& operator=(const ApplicationUser& applicationuser);

	//Move assignment operator
	ApplicationUser& operator=(ApplicationUser&& applicationuser) noexcept;

	ApplicationUser(const std::string& id, const std::string& username, const std::string& normalizedusername, const std::string& email, const bool& emailconfirmed, const std::string& normalizedemail, const std::string& passwordhash, const std::string& securitystamp, const std::string& concurrencystamp, const std::string& phonenumber, const bool& phonenumberconfirmed, const bool& twofactorenabled, const std::string& lockoutend, const bool& lockoutenabled, const int& accessfailedcount);

	virtual std::string ToString();

	virtual Json::Value ToJson();

	virtual std::string GetId()
	{
		return Id;
	}
	virtual std::string GetUserName()
	{
		return UserName;
	}
	virtual std::string GetNormalizedUserName()
	{
		return NormalizedUserName;
	}
	virtual std::string GetEmail()
	{
		return Email;
	}
	virtual std::string GetNormalizedEmail()
	{
		return NormalizedEmail;
	}
	virtual std::string GetPasswordHash()
	{
		return PasswordHash;
	}
	virtual std::string GetSecurityStamp()
	{
		return SecurityStamp;
	}
	virtual std::string GetConcurrencyStamp()
	{
		return ConcurrencyStamp;
	}
	virtual std::string GetPhoneNumber()
	{
		return PhoneNumber;
	}
	virtual std::string GetLockoutEnd()
	{
		return LockoutEnd;
	}
	virtual bool GetEmailConfirmed()
	{
		return EmailConfirmed;
	}
	virtual bool GetPhoneNumberConfirmed()
	{
		return PhoneNumberConfirmed;
	}
	virtual bool GetTwoFactorEnabled()
	{
		return TwoFactorEnabled;
	}
	virtual bool GetLockoutEnabled()
	{
		return LockoutEnabled;
	}
	virtual int GetAccessFailedCount()
	{
		return AccessFailedCount;
	}
	virtual void SetUserName(const std::string& username)
	{
		UserName = username;
	}
	virtual void SetNormalizedUserName(const std::string& normalizedusername)
	{
		NormalizedUserName = normalizedusername;
	}
	virtual void SetEmail(const std::string& email)
	{
		Email = email;
	}
	virtual void SetNormalizedEmail(const std::string& normalizedemail)
	{
		NormalizedEmail = normalizedemail;
	}
	virtual void SetPasswordHash(const std::string& passwordhash)
	{
		PasswordHash = passwordhash;
	}
	virtual void SetSecurityStamp(const std::string& securitystamp)
	{
		SecurityStamp = securitystamp;
	}
	virtual void SetConcurrencyStamp(const std::string& concurrencystamp)
	{
		ConcurrencyStamp = concurrencystamp;
	}
	virtual void SetPhoneNumber(const std::string& phonenumber)
	{
		PhoneNumber = phonenumber;
	}
	virtual void SetLockoutEnd(const std::string& lockoutend)
	{
		LockoutEnd = lockoutend;
	}
	virtual void SetEmailConfirmed(const bool& emailconfirmed)
	{
		EmailConfirmed = emailconfirmed;
	}
	virtual void SetPhoneNumberConfirmed(const bool& phonenumberconfirmed)
	{
		PhoneNumberConfirmed = phonenumberconfirmed;
	}
	virtual void SetTwoFactorEnabled(const bool& twofactorenabled)
	{
		TwoFactorEnabled = twofactorenabled;
	}
	virtual void SetLockoutEnabled(const bool& lockoutenabled)
	{
		LockoutEnabled = lockoutenabled;
	}
	virtual void SetAccessFailedCount(const int& accessfailedcount)
	{
		AccessFailedCount = accessfailedcount;
	}

	virtual ~ApplicationUser();

private:
	std::string Id = {};
	std::string UserName = {};
	std::string NormalizedUserName = {};
	std::string Email = {};
	bool EmailConfirmed = false;
	std::string NormalizedEmail = {};
	std::string PasswordHash = {};
	std::string SecurityStamp = {};
	std::string ConcurrencyStamp = {};
	std::string PhoneNumber = {};
	bool PhoneNumberConfirmed = false;
	bool TwoFactorEnabled = false;
	std::string LockoutEnd = {};
	bool LockoutEnabled = false;
	int AccessFailedCount = 0;
};

