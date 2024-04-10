#pragma once
#include "BaseQuery.h"
#include "Message.h"
#include "PaginationObject.h"
#include "MessageModel.h"

#include "ApplicationApi.h"
#include "BaseCommand.h"
#include "Mapper.h"
class APPLICATION_API MessageService
{
public:
	MessageService() = default;
	std::shared_ptr<Message> GetMessageById(const std::string& id);
	std::vector<std::shared_ptr<Message>>GetAllMessages();
	std::shared_ptr<PaginationObject<Message>> GetMessagesByPage(int page, int page_size);
	std::shared_ptr<PaginationObject<Message>> GetMessagesByChat(int page, int page_size, std::string chat_id);
	std::shared_ptr<PaginationObject<Message>> GetMessagesByDate(int page, int page_size, std::string chat_id, std::string date);
	std::string CreateMessage(MessageModel& todo_list_model);
	int UpdateMessage(MessageModel& todo_list_model, const std::string& id);
	int DeleteMessage(const std::string& id);
	~MessageService() = default;
};

