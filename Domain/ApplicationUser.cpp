#include "pch.h"
#include "ApplicationUser.h"

ApplicationUser::ApplicationUser() = default;

ApplicationUser::ApplicationUser( const ApplicationUser& applicationuser )
{
	Id = applicationuser.Id;
	UserName = applicationuser.UserName;
	Email = applicationuser.Email;
	PasswordHash = applicationuser.PasswordHash;
	PhoneNumber = applicationuser.PhoneNumber;
}

ApplicationUser::ApplicationUser(ApplicationUser&& applicationuser) noexcept
{
	Id = std::move(applicationuser.Id);
	UserName = std::move(applicationuser.UserName);
	Email = std::move(applicationuser.Email);
	PasswordHash = std::move(applicationuser.PasswordHash);
	PhoneNumber = std::move(applicationuser.PhoneNumber);
}

ApplicationUser& ApplicationUser::operator=( const ApplicationUser& applicationuser )
= default;

ApplicationUser::ApplicationUser( const std::string& id, const std::string& username, const std::string& email, const std::string& passwordhash, const std::string& phonenumber )
{
	Id = id;
	UserName = username;
	Email = email;
	PasswordHash = passwordhash;
	PhoneNumber = phonenumber;
}

std::string ApplicationUser::ToString()
{
	return "Id: " + Id + "\n" +
		"UserName: " + UserName + "\n" +
		"Email: " + Email + "\n" +
		"PasswordHash: " + PasswordHash + "\n" +
		"PhoneNumber: " + PhoneNumber + "\n";
}

Json::Value ApplicationUser::ToJson()
{
	Json::Value json;
	json["Id"] = Id;
	json["UserName"] = UserName;
	json["Email"] = Email;
	json["PasswordHash"] = PasswordHash;
	json["PhoneNumber"] = PhoneNumber;
	return json;
}

ApplicationUser::~ApplicationUser() = default;