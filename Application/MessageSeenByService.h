#pragma once
#include "BaseQuery.h"
#include "MessageSeenBy.h"
#include "PaginationObject.h"
#include "MessageSeenByModel.h"

#include "ApplicationApi.h"
#include "BaseCommand.h"
#include "Mapper.h"
class APPLICATION_API MessageSeenByService
{
public:
	MessageSeenByService() = default;
	std::shared_ptr<MessageSeenBy> GetMessageSeenByById(const std::string& id);
	std::vector<std::shared_ptr<MessageSeenBy>>GetAllMessageSeenBys();
	std::shared_ptr<PaginationObject<MessageSeenBy>> GetMessageSeenBysByPage(int page, int page_size);
	std::string CreateMessageSeenBy(MessageSeenByModel& todo_list_model);
	int UpdateMessageSeenBy(MessageSeenByModel& todo_list_model, const std::string& id);
	int DeleteMessageSeenBy(const std::string& id);
	~MessageSeenByService() = default;
};

