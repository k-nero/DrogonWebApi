#pragma once
#include "BaseEntity.h"
#include "ApplicationUser.h"
#include "ContactList.h"

class ApplicationUser;
class ContactList;

class DOMAIN_API Contact : public BaseEntity
{
	BOOST_DESCRIBE_CLASS(Contact, (BaseEntity), (), (ApplicationUserId, ContactListId, ApplicationUser, ContactList), ())
public:
	Contact() = default;
	~Contact() = default;
protected:
	std::string ApplicationUserId = "";
	std::string ContactListId = "";

	std::shared_ptr<ApplicationUser> ApplicationUser = {};
	std::shared_ptr<ContactList> ContactList = {};
};

