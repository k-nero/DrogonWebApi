#pragma once
#include "BaseQuery.h"
#include "MessageReaction.h"
#include "PaginationObject.h"
#include "MessageReactionModel.h"

#include "ApplicationApi.h"
#include "BaseCommand.h"
#include "Mapper.h"
class APPLICATION_API MessageReactionService
{
public:
	MessageReactionService() = default;
	std::shared_ptr<MessageReaction> GetMessageReactionById(const std::string& id);
	std::vector<std::shared_ptr<MessageReaction>>GetAllMessageReactions();
	std::shared_ptr<PaginationObject<MessageReaction>> GetMessageReactionsByPage(int page, int page_size);
	std::string CreateMessageReaction(MessageReactionModel& todo_list_model);
	int UpdateMessageReaction(MessageReactionModel& todo_list_model, const std::string& id);
	int DeleteMessageReaction(const std::string& id);
	~MessageReactionService() = default;
};

