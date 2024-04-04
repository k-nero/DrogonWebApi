#include "pch.h"
#include "ChatParticipantService.h"

std::shared_ptr<ChatParticipant> ChatParticipantService::GetChatParticipantById(const std::string& id)
{
	Query<ChatParticipant> query;
	return query.GetByIdEw(id);
}

std::vector<std::shared_ptr<ChatParticipant>> ChatParticipantService::GetAllChatParticipants()
{
	Query<ChatParticipant> query;
	return std::move(query.GetAllEw());
}

std::shared_ptr<PaginationObject<ChatParticipant>> ChatParticipantService::GetChatParticipantsByPage(int page, int page_size, std::string ApplicationUserId)
{
	Query<ChatParticipant> query;
	return query.GetPaginatedEw(page, page_size, EQ(ApplicationUserId), {"ChatRoom"});
}

std::string ChatParticipantService::CreateChatParticipant(ChatParticipantModel& model)
{
	BaseCommand<ChatParticipant> cmd;
	auto entity = Mapper::Map<ChatParticipantModel, ChatParticipant>(model);
	entity.SetId(CoreHelper::CreateUUID());
	auto result = cmd.Create(entity);
	if (result)
	{
		return entity.GetId();
	}
	throw std::exception("Error creating chat participant");
}

int ChatParticipantService::UpdateChatParticipant(ChatParticipantModel& model, const std::string& Id)
{
	BaseCommand<ChatParticipant> cmd;
	auto entity = Mapper::Map<ChatParticipantModel, ChatParticipant>(model);
	auto result = cmd.Update(entity, EQ(Id));
	if (result == 0)
	{
		throw std::exception("Error updating chat participant");
	}
	return result;
}

int ChatParticipantService::DeleteChatParticipant(const std::string& Id)
{
	BaseCommand<ChatParticipant> cmd;
	auto result = cmd.Delete(EQ(Id));
	if (result == 0)
	{
		throw std::exception("Error deleting chat participant");
	}
	return result;
}
