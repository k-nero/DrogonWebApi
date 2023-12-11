#include "pch.h"
#include "ApplicationUser.h"

ApplicationUser::ApplicationUser() = default;

ApplicationUser::ApplicationUser(const ApplicationUser& applicationuser)
{
	Id = applicationuser.Id;
	UserName = applicationuser.UserName;
	Email = applicationuser.Email;
	PasswordHash = applicationuser.PasswordHash;
	PhoneNumber = applicationuser.PhoneNumber;
	CreatedDate = applicationuser.CreatedDate;
	ModifiedDate = applicationuser.ModifiedDate;
}

ApplicationUser::ApplicationUser(ApplicationUser&& applicationuser) noexcept
{
	Id = std::move(applicationuser.Id);
	UserName = std::move(applicationuser.UserName);
	Email = std::move(applicationuser.Email);
	PasswordHash = std::move(applicationuser.PasswordHash);
	PhoneNumber = std::move(applicationuser.PhoneNumber);
	CreatedDate = std::move(applicationuser.CreatedDate);
	ModifiedDate = std::move(applicationuser.ModifiedDate);
}

ApplicationUser& ApplicationUser::operator=(const ApplicationUser& applicationuser)
= default;

ApplicationUser::ApplicationUser(const std::string& id, const std::string& username, const std::string& email, const std::string& passwordhash, const std::string& phonenumber, const std::string& createdDate, const std::string& modifedDate)
{
	Id = id;
	UserName = username;
	Email = email;
	PasswordHash = passwordhash;
	PhoneNumber = phonenumber;
	CreatedDate = createdDate;
	ModifiedDate = modifedDate;
}

std::string ApplicationUser::ToString()
{
	return "Id: " + Id + "\n" +
		"UserName: " + UserName + "\n" +
		"Email: " + Email + "\n" +
		"PasswordHash: " + PasswordHash + "\n" +
		"PhoneNumber: " + PhoneNumber + "\n"
		"CreatedDate: " + CreatedDate + "\n"
		"ModifiedDate: " + ModifiedDate + "\n";
}

Json::Value ApplicationUser::ToJson()
{
	Json::Value json;
	json["id"] = Id;
	json["username"] = UserName;
	json["email"] = Email;
	json["passwordHash"] = PasswordHash;
	json["phoneNumber"] = PhoneNumber;
	json["createdDate"] = CreatedDate;
	json["modifiedDate"] = ModifiedDate;
	return json;
}

ApplicationUser::~ApplicationUser() = default;