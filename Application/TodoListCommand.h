#pragma once
#include "TodoList.h"
#include <SQLAPI.h>
#include <vector>
#include "CoreHelper.h"
#include "ApplicationApi.h"
#include "TodoItemCommand.h"

class APPLICATION_API TodoListCommand
{
public:
	TodoListCommand();
	explicit TodoListCommand(SAConnection* con) { this->con = con; }
	std::shared_ptr<TodoList> GetTodoListById(const std::string& id);
	std::vector<std::shared_ptr<TodoList>>GetAllTodoLists();
	int CreateTodoList(TodoList& todoList);
	int UpdateTodoList(TodoList& todoList);
	~TodoListCommand();
private:
	std::shared_ptr<TodoList> GetTodoListFromCommand(SACommand& cmd);
	SAConnection* con = nullptr;
};

