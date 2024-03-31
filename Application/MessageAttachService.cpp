#include "pch.h"
#include "MessageAttachService.h"

std::shared_ptr<MessageAttach> MessageAttachService::GetMessageAttachById(const std::string& id)
{
	Query<MessageAttach> query;
	return query.GetByIdEw(id);
}

std::vector<std::shared_ptr<MessageAttach>> MessageAttachService::GetAllMessageAttachs()
{
	Query<MessageAttach> query;
	return query.GetAllEw();
}

std::shared_ptr<PaginationObject<MessageAttach>> MessageAttachService::GetMessageAttachsByPage(int page, int page_size)
{
	Query<MessageAttach> query;
	return query.GetPaginatedEw(page, page_size);
}

std::string MessageAttachService::CreateMessageAttach(MessageAttachModel& model)
{
	BaseCommand<MessageAttach> cmd;
	auto entity = Mapper::Map<MessageAttachModel, MessageAttach>(model);
	entity.SetId(CoreHelper::CreateUUID());
	auto result = cmd.Create(entity);
	if (result)
	{
		return entity.GetId();
	}
	throw std::exception("Error creating chat participant");
}

int MessageAttachService::UpdateMessageAttach(MessageAttachModel& model, const std::string& Id)
{
	BaseCommand<MessageAttach> cmd;
	auto entity = Mapper::Map<MessageAttachModel, MessageAttach>(model);
	auto result = cmd.Update(entity, EQ(Id));
	if (result == 0)
	{
		throw std::exception("Error updating chat participant");
	}
	return result;
}

int MessageAttachService::DeleteMessageAttach(const std::string& Id)
{
	BaseCommand<MessageAttach> cmd;
	auto result = cmd.Delete(EQ(Id));
	if (result == 0)
	{
		throw std::exception("Error deleting chat participant");
	}
	return result;
}
