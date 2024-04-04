#pragma once

#include "BaseQuery.h"
#include "ChatRoom.h"
#include "PaginationObject.h"
#include "ChatParticipantModel.h"

#include "ApplicationApi.h"
#include "BaseCommand.h"
#include "Mapper.h"

class APPLICATION_API ChatParticipantService
{
public:
	ChatParticipantService() = default;
	std::shared_ptr<ChatParticipant> GetChatParticipantById(const std::string& id);
	std::vector<std::shared_ptr<ChatParticipant>>GetAllChatParticipants();
	std::shared_ptr<PaginationObject<ChatParticipant>> GetChatParticipantsByPage(int page, int page_size, std::string ApplicationUserId);
	std::string CreateChatParticipant(ChatParticipantModel& model);
	int UpdateChatParticipant(ChatParticipantModel& model, const std::string& id);
	int DeleteChatParticipant(const std::string& id);
	~ChatParticipantService() = default;
};

