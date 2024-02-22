#pragma once
#include <TodoItem.h>
#include "CoreHelper.h"
#include "ApplicationApi.h"
#include "TodoItemCommand.h"
#include "TodoItemQuery.h"


class APPLICATION_API TodoItemService
{
public:
	TodoItemService();
	//explicit TodoItemService(SAConnection* con) { this->con = con; }
	std::shared_ptr<TodoItem> GetTodoItemById(const std::string& id);
	std::vector<std::shared_ptr<TodoItem>>GetAllTodoItems();
	std::string CreateTodoItem(TodoItem * TodoItem);
	int UpdateTodoItem(TodoItem* TodoItem);
	~TodoItemService();
private:
	//SAConnection* con;
};

