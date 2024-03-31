#pragma once
#include "BaseQuery.h"
#include "MessageAttach.h"
#include "PaginationObject.h"
#include "MessageAttachModel.h"

#include "ApplicationApi.h"
#include "BaseCommand.h"
#include "Mapper.h"
class APPLICATION_API MessageAttachService
{
public:
	MessageAttachService() = default;
	std::shared_ptr<MessageAttach> GetMessageAttachById(const std::string& id);
	std::vector<std::shared_ptr<MessageAttach>>GetAllMessageAttachs();
	std::shared_ptr<PaginationObject<MessageAttach>> GetMessageAttachsByPage(int page, int page_size);
	std::string CreateMessageAttach(MessageAttachModel& todo_list_model);
	int UpdateMessageAttach(MessageAttachModel& todo_list_model, const std::string& id);
	int DeleteMessageAttach(const std::string& id);
	~MessageAttachService() = default;
};

