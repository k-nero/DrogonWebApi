#pragma once
#include "BaseEntity.h"
#include "Contact.h"
#include "ApplicationUser.h"

class Contact;
class ApplicationUser;

class DOMAIN_API ContactList : public BaseEntity
{
	BOOST_DESCRIBE_CLASS(ContactList, (BaseEntity), (), (ApplicationUserId, Contacts, Name), ())
public:
	ContactList() = default;
	~ContactList() = default;

protected:
	std::string ApplicationUserId = "";
	std::string Name = "";
	std::vector<std::shared_ptr<Contact>> Contacts = {};
};

