#pragma once
#include "BaseQuery.h"
#include "ContactList.h"
#include "PaginationObject.h"
#include "ContactListModel.h"

#include "ApplicationApi.h"
#include "BaseCommand.h"
#include "Mapper.h"
class APPLICATION_API ContactListService
{
public:
	ContactListService() = default;
	std::shared_ptr<ContactList> GetContactListById(const std::string& id);
	std::vector<std::shared_ptr<ContactList>>GetAllContactLists();
	std::shared_ptr<PaginationObject<ContactList>> GetContactListsByPage(int page, int page_size);
	std::string CreateContactList(ContactListModel& todo_list_model);
	int UpdateContactList(ContactListModel& todo_list_model, const std::string& id);
	int DeleteContactList(const std::string& id);
	~ContactListService() = default;
};

