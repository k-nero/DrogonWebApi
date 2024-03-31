#include "pch.h"
#include "ChatRoomService.h"

std::shared_ptr<ChatRoom> ChatRoomService::GetChatRoomById(const std::string& id)
{
	Query<ChatRoom> query;
	return query.GetByIdEw(id);
}

std::vector<std::shared_ptr<ChatRoom>> ChatRoomService::GetAllChatRooms()
{
	Query<ChatRoom> query;
	return query.GetAllEw();
}

std::shared_ptr<PaginationObject<ChatRoom>> ChatRoomService::GetChatRoomsByPage(int page, int page_size)
{
	Query<ChatRoom> query;
	return query.GetPaginatedEw(page, page_size);
}

std::string ChatRoomService::CreateChatRoom(ChatRoomModel& model)
{
	BaseCommand<ChatRoom> cmd;
	auto entity = Mapper::Map<ChatRoomModel, ChatRoom>(model);
	entity.SetId(CoreHelper::CreateUUID());
	auto result = cmd.Create(entity);
	if (result)
	{
		return entity.GetId();
	}
	throw std::exception("Error creating chat participant");
}

int ChatRoomService::UpdateChatRoom(ChatRoomModel& model, const std::string& Id)
{
	BaseCommand<ChatRoom> cmd;
	auto entity = Mapper::Map<ChatRoomModel, ChatRoom>(model);
	auto result = cmd.Update(entity, EQ(Id));
	if (result == 0)
	{
		throw std::exception("Error updating chat participant");
	}
	return result;
}

int ChatRoomService::DeleteChatRoom(const std::string& Id)
{
	BaseCommand<ChatRoom> cmd;
	auto result = cmd.Delete(EQ(Id));
	if (result == 0)
	{
		throw std::exception("Error deleting chat participant");
	}
	return result;
}
