#pragma once
#include "BaseQuery.h"
#include "ChatRoom.h"
#include "PaginationObject.h"
#include "ChatRoomModel.h"

#include "ApplicationApi.h"
#include "BaseCommand.h"
#include "Mapper.h"
class APPLICATION_API ChatRoomService
{
public:
	ChatRoomService() = default;
	std::shared_ptr<ChatRoom> GetChatRoomById(const std::string& id);
	std::vector<std::shared_ptr<ChatRoom>>GetAllChatRooms();
	std::shared_ptr<PaginationObject<ChatRoom>> GetChatRoomsByPage(int page, int page_size);
	std::string CreateChatRoom(ChatRoomModel& todo_list_model);
	int UpdateChatRoom(ChatRoomModel& todo_list_model, const std::string& id);
	int DeleteChatRoom(const std::string& id);
	~ChatRoomService() = default;
};

