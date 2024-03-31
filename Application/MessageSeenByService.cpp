#include "pch.h"
#include "MessageSeenByService.h"

std::shared_ptr<MessageSeenBy> MessageSeenByService::GetMessageSeenByById(const std::string& id)
{
	Query<MessageSeenBy> query;
	return query.GetByIdEw(id);
}

std::vector<std::shared_ptr<MessageSeenBy>> MessageSeenByService::GetAllMessageSeenBys()
{
	Query<MessageSeenBy> query;
	return query.GetAllEw();
}

std::shared_ptr<PaginationObject<MessageSeenBy>> MessageSeenByService::GetMessageSeenBysByPage(int page, int page_size)
{
	Query<MessageSeenBy> query;
	return query.GetPaginatedEw(page, page_size);
}

std::string MessageSeenByService::CreateMessageSeenBy(MessageSeenByModel& model)
{
	BaseCommand<MessageSeenBy> cmd;
	auto entity = Mapper::Map<MessageSeenByModel, MessageSeenBy>(model);
	entity.SetId(CoreHelper::CreateUUID());
	auto result = cmd.Create(entity);
	if (result)
	{
		return entity.GetId();
	}
	throw std::exception("Error creating chat participant");
}

int MessageSeenByService::UpdateMessageSeenBy(MessageSeenByModel& model, const std::string& Id)
{
	BaseCommand<MessageSeenBy> cmd;
	auto entity = Mapper::Map<MessageSeenByModel, MessageSeenBy>(model);
	auto result = cmd.Update(entity, EQ(Id));
	if (result == 0)
	{
		throw std::exception("Error updating chat participant");
	}
	return result;
}

int MessageSeenByService::DeleteMessageSeenBy(const std::string& Id)
{
	BaseCommand<MessageSeenBy> cmd;
	auto result = cmd.Delete(EQ(Id));
	if (result == 0)
	{
		throw std::exception("Error deleting chat participant");
	}
	return result;
}
