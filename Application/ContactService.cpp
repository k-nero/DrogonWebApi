#include "pch.h"
#include "ContactService.h"

std::shared_ptr<Contact> ContactService::GetContactById(const std::string& id)
{
	Query<Contact> query;
	return query.GetByIdEw(id);
}

std::vector<std::shared_ptr<Contact>> ContactService::GetAllContacts()
{
	Query<Contact> query;
	return query.GetAllEw();
}

std::shared_ptr<PaginationObject<Contact>> ContactService::GetContactsByPage(int page, int page_size)
{
	Query<Contact> query;
	return query.GetPaginatedEw(page, page_size);
}

std::string ContactService::CreateContact(ContactModel& model)
{
	BaseCommand<Contact> cmd;
	auto entity = Mapper::Map<ContactModel, Contact>(model);
	entity.SetId(CoreHelper::CreateUUID());
	auto result = cmd.Create(entity);
	if (result)
	{
		return entity.GetId();
	}
	throw std::exception("Error creating chat participant");
}

int ContactService::UpdateContact(ContactModel& model, const std::string& Id)
{
	BaseCommand<Contact> cmd;
	auto entity = Mapper::Map<ContactModel, Contact>(model);
	auto result = cmd.Update(entity, EQ(Id));
	if (result == 0)
	{
		throw std::exception("Error updating chat participant");
	}
	return result;
}

int ContactService::DeleteContact(const std::string& Id)
{
	BaseCommand<Contact> cmd;
	auto result = cmd.Delete(EQ(Id));
	if (result == 0)
	{
		throw std::exception("Error deleting chat participant");
	}
	return result;
}
