#pragma once
#include "BaseEntity.h"
#include "ApplicationUser.h"
#include "ContactList.h"

class Contact : public BaseEntity
{
	BOOST_DESCRIBE_CLASS(Contact, (BaseEntity), (), (ApplicationUserId, ContactListId, ApplicationUser, ContactList), ())
public:
	Contact();
	~Contact() = default;
protected:
	std::string ApplicationUserId = "";
	std::string ContactListId = "";

	std::shared_ptr<ApplicationUser> ApplicationUser = {};
	std::shared_ptr<ContactList> ContactList = {};
};

