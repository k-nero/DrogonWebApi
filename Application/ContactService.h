#pragma once
#include "BaseQuery.h"
#include "Contact.h"
#include "PaginationObject.h"
#include "ContactModel.h"

#include "ApplicationApi.h"
#include "BaseCommand.h"
#include "Mapper.h"
class APPLICATION_API ContactService
{
public:
	ContactService() = default;
	std::shared_ptr<Contact> GetContactById(const std::string& id);
	std::vector<std::shared_ptr<Contact>>GetAllContacts();
	std::shared_ptr<PaginationObject<Contact>> GetContactsByPage(int page, int page_size);
	std::string CreateContact(ContactModel& todo_list_model);
	int UpdateContact(ContactModel& todo_list_model, const std::string& id);
	int DeleteContact(const std::string& id);
	~ContactService() = default;
};

