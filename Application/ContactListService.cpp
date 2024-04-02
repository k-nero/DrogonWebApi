#include "pch.h"
#include "ContactListService.h"

std::shared_ptr<ContactList> ContactListService::GetContactListById(const std::string& id)
{
	Query<ContactList> query;
	return query.GetByIdEw(id);
}

std::vector<std::shared_ptr<ContactList>> ContactListService::GetAllContactLists()
{
	Query<ContactList> query;
	return query.GetAllEw();
}

std::shared_ptr<PaginationObject<ContactList>> ContactListService::GetContactListsByPage(int page, int page_size)
{
	Query<ContactList> query;
	return query.GetPaginatedEw(page, page_size);
}

std::shared_ptr<PaginationObject<ContactList>> ContactListService::GetContactListsByUserId(int page, int page_size, std::string ApplicationUserId)
{
	Query<ContactList> query;
	return query.GetPaginatedEw(page, page_size, EQ(ApplicationUserId));
}

std::string ContactListService::CreateContactList(ContactListModel& model)
{
	BaseCommand<ContactList> cmd;
	auto entity = Mapper::Map<ContactListModel, ContactList>(model);
	entity.SetId(CoreHelper::CreateUUID());
	auto result = cmd.Create(entity);
	if (result)
	{
		return entity.GetId();
	}
	throw std::exception("Error creating chat participant");
}

int ContactListService::UpdateContactList(ContactListModel& model, const std::string& Id)
{
	BaseCommand<ContactList> cmd;
	auto entity = Mapper::Map<ContactListModel, ContactList>(model);
	auto result = cmd.Update(entity, EQ(Id));
	if (result == 0)
	{
		throw std::exception("Error updating chat participant");
	}
	return result;
}

int ContactListService::DeleteContactList(const std::string& Id)
{
	BaseCommand<ContactList> cmd;
	auto result = cmd.Delete(EQ(Id));
	if (result == 0)
	{
		throw std::exception("Error deleting chat participant");
	}
	return result;
}
