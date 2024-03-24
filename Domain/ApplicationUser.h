#pragma once
#include "BaseEntity.h"
class DOMAIN_API ApplicationUser : public BaseEntity
{
	BOOST_DESCRIBE_CLASS(ApplicationUser, (BaseEntity), (), (UserName, Email, PasswordHash, PhoneNumber), ())
public:
	ApplicationUser();

	//Copy constructor
	ApplicationUser(const ApplicationUser& applicationuser);

	//Move constructor
	ApplicationUser(ApplicationUser&& applicationuser) noexcept;

	//Copy assignment operator
	ApplicationUser& operator=(const ApplicationUser& applicationuser);

	ApplicationUser(const std::string& id, const std::string& username, const std::string& email, const std::string& passwordhash, const std::string& phonenumber, const std::string& createdDate = "", const std::string& modifedDate = "");

	ApplicationUser(const std::string& username, const std::string& email, const std::string& passwordhash, const std::string& phonenumber, const std::string& createdDate = "", const std::string& modifedDate = "");

	//Move assignment operator
	ApplicationUser& operator=(ApplicationUser&& applicationuser) noexcept;

	virtual std::string ToString();

	//virtual Json::Value ToJson();
	virtual std::string GetUsername()
	{
		return UserName;
	}
	virtual std::string GetEmail()
	{
		return Email;
	}
	virtual std::string GetPasswordHash()
	{
		return PasswordHash;
	}
	virtual std::string GetPhoneNumber()
	{
		return PhoneNumber;
	}
	virtual void SetUsername(const std::string& username)
	{
		UserName = username;
	}
	virtual void SetEmail(const std::string& email)
	{
		Email = email;
	}
	virtual void SetPasswordHash(const std::string& passwordhash)
	{
		PasswordHash = passwordhash;
	}
	virtual void SetPhoneNumber(const std::string& phonenumber)
	{
		PhoneNumber = phonenumber;
	}

	virtual ~ApplicationUser();

protected:
	std::string UserName = {};
	std::string Email = {};
	std::string PasswordHash = {};
	std::string PhoneNumber = {};
};