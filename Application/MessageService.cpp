#include "pch.h"
#include "MessageService.h"

std::shared_ptr<Message> MessageService::GetMessageById(const std::string& id)
{
	Query<Message> query;
	return query.GetByIdEw(id, { "ApplicationUser", "MessageSeenBys", "MessageReactions", "MessageAttachs" });
}

std::vector<std::shared_ptr<Message>> MessageService::GetAllMessages()
{
	Query<Message> query;
	return query.GetAllEw();
}

std::shared_ptr<PaginationObject<Message>> MessageService::GetMessagesByPage(int page, int page_size)
{
	Query<Message> query;
	return query.GetPaginatedEw(page, page_size, "", { "ApplicationUser", "MessageSeenBys", "MessageReactions", "MessageAttachs" });
}

std::shared_ptr<PaginationObject<Message>> MessageService::GetMessagesByChat(int page, int page_size, std::string ChatRoomId)
{
	Query<Message> query;
	return query.GetPaginatedEw(page, page_size, EQ(ChatRoomId), {"ApplicationUser", "MessageSeenBys", "MessageReactions", "MessageAttachs" });
}

std::shared_ptr<PaginationObject<Message>> MessageService::GetMessagesByDate(int page, int page_size, std::string ChatRoomId, std::string CreatedDate)
{
	Query<Message> query;
	return query.GetPaginatedEw(page, page_size, AND(EQ(ChatRoomId), LT(CreatedDate) ), {"ApplicationUser", "MessageSeenBys", "MessageReactions", "MessageAttachs" });
}

std::string MessageService::CreateMessage(MessageModel& model)
{
	BaseCommand<Message> cmd;
	auto entity = Mapper::Map<MessageModel, Message>(model);
	entity.SetId(CoreHelper::CreateUUID());
	auto result = cmd.Create(entity);
	if (result)
	{
		return entity.GetId();
	}
	throw std::exception("Error creating chat participant");
}

int MessageService::UpdateMessage(MessageModel& model, const std::string& Id)
{
	BaseCommand<Message> cmd;
	auto entity = Mapper::Map<MessageModel, Message>(model);
	auto result = cmd.Update(entity, EQ(Id));
	if (result == 0)
	{
		throw std::exception("Error updating chat participant");
	}
	return result;
}

int MessageService::DeleteMessage(const std::string& Id)
{
	BaseCommand<Message> cmd;
	auto result = cmd.Delete(EQ(Id));
	if (result == 0)
	{
		throw std::exception("Error deleting chat participant");
	}
	return result;
}
