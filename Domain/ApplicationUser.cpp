#include "pch.h"
#include "ApplicationUser.h"

ApplicationUser::ApplicationUser() = default;

ApplicationUser::ApplicationUser(const ApplicationUser& applicationuser) : BaseEntity(applicationuser)
{
	UserName = applicationuser.UserName;
	Email = applicationuser.Email;
	PasswordHash = applicationuser.PasswordHash;
	PhoneNumber = applicationuser.PhoneNumber;
	ContactListId = applicationuser.ContactListId;
	// Shalow copy
	chatParticipants = applicationuser.chatParticipants;
	contactList = applicationuser.contactList;
}

ApplicationUser::ApplicationUser(ApplicationUser&& applicationuser) noexcept : BaseEntity(applicationuser)
{
	UserName = std::move(applicationuser.UserName);
	Email = std::move(applicationuser.Email);
	PasswordHash = std::move(applicationuser.PasswordHash);
	PhoneNumber = std::move(applicationuser.PhoneNumber);
	ContactListId = std::move(applicationuser.ContactListId);

	chatParticipants = std::move(applicationuser.chatParticipants);
	contactList = std::move(applicationuser.contactList);
}

ApplicationUser& ApplicationUser::operator=(const ApplicationUser& applicationuser)
{
	BaseEntity::operator=(applicationuser);
	UserName = applicationuser.UserName;
	Email = applicationuser.Email;
	PasswordHash = applicationuser.PasswordHash;
	PhoneNumber = applicationuser.PhoneNumber;
	ContactListId = applicationuser.ContactListId;
	// Shalow copy
	chatParticipants = applicationuser.chatParticipants;
	contactList = applicationuser.contactList;
	return *this;
}

ApplicationUser::ApplicationUser(const std::string& id, const std::string& username, const std::string& email, const std::string& passwordhash, const std::string& phonenumber, const std::string& contactListId, const std::string& createdDate, const std::string& modifedDate) : BaseEntity(id, createdDate, modifedDate)
{
	UserName = username;
	Email = email;
	PasswordHash = passwordhash;
	PhoneNumber = phonenumber;
	ContactListId = contactListId;
}

ApplicationUser::ApplicationUser(const std::string& username, const std::string& email, const std::string& passwordhash, const std::string& phonenumber, const std::string& contactListId, const std::string& createdDate, const std::string& modifedDate)
{
	UserName = username;
	Email = email;
	PasswordHash = passwordhash;
	PhoneNumber = phonenumber;
	ContactListId = contactListId;
	CreatedDate = createdDate;
	ModifiedDate = modifedDate;
}

ApplicationUser& ApplicationUser::operator=(ApplicationUser&& applicationuser) noexcept
{
	BaseEntity::operator=(applicationuser);
	UserName = std::move(applicationuser.UserName);
	Email = std::move(applicationuser.Email);
	PasswordHash = std::move(applicationuser.PasswordHash);
	PhoneNumber = std::move(applicationuser.PhoneNumber);
	ContactListId = std::move(applicationuser.ContactListId);

	chatParticipants = std::move(applicationuser.chatParticipants);
	contactList = std::move(applicationuser.contactList);
	return *this;
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

ApplicationUser::~ApplicationUser() = default;