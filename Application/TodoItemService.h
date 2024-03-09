#pragma once
#include <TodoItem.h>
#include "CoreHelper.h"
#include "ApplicationApi.h"
#include "TodoItemCommand.h"
#include "TodoItemQuery.h"
#include "TodoItemModel.h"
#include "Mapper.h"
#include "RedisContext.h"

class APPLICATION_API TodoItemService
{
public:
	TodoItemService();
	//explicit TodoItemService(SAConnection* con) { this->con = con; }
	std::shared_ptr<TodoItem> GetTodoItemById(const std::string& id);
	std::vector<std::shared_ptr<TodoItem>>GetAllTodoItems();
	std::string CreateTodoItem(TodoItemModel& todo_item_model);
	int UpdateTodoItem(TodoItemModel& todo_item_model, const std::string& id);
	int DeleteTodoItem(const std::string& id);
	~TodoItemService();
private:
	//SAConnection* con;
};

