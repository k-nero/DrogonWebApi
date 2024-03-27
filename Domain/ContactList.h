#pragma once
#include "BaseEntity.h"
#include "Contact.h"

class Contact;

class ContactList : public BaseEntity
{
	BOOST_DESCRIBE_CLASS(ContactList, (BaseEntity), (), (ApplicationUserId, Contacts), ())
public:
	ContactList();
	~ContactList() = default;

protected:
	std::string ApplicationUserId = "";
	std::vector<std::shared_ptr<Contact>> Contacts = {};
};

