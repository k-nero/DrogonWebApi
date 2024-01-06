#pragma once
#include "TodoItem.h"
#include <SQLAPI.h>
#include <vector>
#include "CoreHelper.h"
#include "ApplicationApi.h"

class APPLICATION_API TodoItemCommand
{
public:
	TodoItemCommand();
	explicit TodoItemCommand(SAConnection* con) { this->con = con; }
	std::shared_ptr<TodoItem> GetTodoItemById(const std::string& id);
	std::vector<std::shared_ptr<TodoItem>>GetAllTodoItems(std::string query = "");
	int CreateTodoItem(TodoItem& todoItem);
	int UpdateTodoItem(TodoItem& todoItem);
	~TodoItemCommand();
private:
	std::shared_ptr<TodoItem> GetTodoItemFromCommand(SACommand& cmd);
	SAConnection* con = nullptr;
};

