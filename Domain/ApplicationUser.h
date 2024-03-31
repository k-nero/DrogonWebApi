#pragma once
#include "BaseEntity.h"
#include "ChatParticipant.h"
#include "memory"
#include "vector"
#include "ContactList.h"

class ContactList;
class ChatParticipant;

class DOMAIN_API ApplicationUser : public BaseEntity
{
	BOOST_DESCRIBE_CLASS(ApplicationUser, (BaseEntity), (), (UserName, Email, PasswordHash, PhoneNumber, ContactListId, AvatarUrl, chatParticipants, contactList), ())
public:
	ApplicationUser();

	//Copy constructor
	ApplicationUser(const ApplicationUser& applicationuser);

	//Move constructor
	ApplicationUser(ApplicationUser&& applicationuser) noexcept;

	//Copy assignment operator
	ApplicationUser& operator=(const ApplicationUser& applicationUser);

	ApplicationUser(const std::string& id, const std::string& username, const std::string& email, const std::string& passwordhash, const std::string& phonenumber, const std::string& contactListId, const std::string& createdDate = "", const std::string& modifedDate = "");

	ApplicationUser(const std::string& username, const std::string& email, const std::string& passwordhash, const std::string& phonenumber, const std::string& contactListId, const std::string& createdDate = "", const std::string& modifedDate = "");

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

	virtual std::string GetContactListId()
	{
		return ContactListId;
	}
	virtual void SetContactListId(const std::string& contactListId)
	{
		ContactListId = contactListId;
	}

	virtual std::string GetAvatarUrl()
	{
		return AvatarUrl;
	}
	virtual void SetAvatarUrl(const std::string& avatarUrl)
	{
		AvatarUrl = avatarUrl;
	}

	virtual ~ApplicationUser();

protected:
	std::string UserName = {};
	std::string Email = {};
	std::string PasswordHash = {};
	std::string PhoneNumber = {};
	std::string ContactListId = {};
	std::string AvatarUrl = {};
	std::vector<std::shared_ptr<ChatParticipant>> chatParticipants = {};
	std::shared_ptr<ContactList> contactList = nullptr;
};