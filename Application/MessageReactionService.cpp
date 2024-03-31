#include "pch.h"
#include "MessageReactionService.h"

std::shared_ptr<MessageReaction> MessageReactionService::GetMessageReactionById(const std::string& id)
{
	Query<MessageReaction> query;
	return query.GetByIdEw(id);
}

std::vector<std::shared_ptr<MessageReaction>> MessageReactionService::GetAllMessageReactions()
{
	Query<MessageReaction> query;
	return query.GetAllEw();
}

std::shared_ptr<PaginationObject<MessageReaction>> MessageReactionService::GetMessageReactionsByPage(int page, int page_size)
{
	Query<MessageReaction> query;
	return query.GetPaginatedEw(page, page_size);
}

std::string MessageReactionService::CreateMessageReaction(MessageReactionModel& model)
{
	BaseCommand<MessageReaction> cmd;
	auto entity = Mapper::Map<MessageReactionModel, MessageReaction>(model);
	entity.SetId(CoreHelper::CreateUUID());
	auto result = cmd.Create(entity);
	if (result)
	{
		return entity.GetId();
	}
	throw std::exception("Error creating chat participant");
}

int MessageReactionService::UpdateMessageReaction(MessageReactionModel& model, const std::string& Id)
{
	BaseCommand<MessageReaction> cmd;
	auto entity = Mapper::Map<MessageReactionModel, MessageReaction>(model);
	auto result = cmd.Update(entity, EQ(Id));
	if (result == 0)
	{
		throw std::exception("Error updating chat participant");
	}
	return result;
}

int MessageReactionService::DeleteMessageReaction(const std::string& Id)
{
	BaseCommand<MessageReaction> cmd;
	auto result = cmd.Delete(EQ(Id));
	if (result == 0)
	{
		throw std::exception("Error deleting chat participant");
	}
	return result;
}
